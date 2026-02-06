#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class SelectiveCodiftInjectPass : public FunctionPass {
  public:
  static char ID;
  SelectiveCodiftInjectPass() : FunctionPass(ID) {}

  bool runOnFunction(Function& F) override {
    bool modified = false;
    errs() << "[SELECTIVE CO-DIFT INJECT] Processing: " << F.getName() << "\n";

    for (auto& BB : F) {
      for (auto& I : BB) {
        if (auto* BO = dyn_cast<BinaryOperator>(&I)) {
          if (shouldInstrumentBinaryOp(BO)) {
            // ============================================
            // NEW: Check if we should track this operation
            // ============================================
            if (!shouldTrackBinaryOperation(BO, &F)) {
              continue; // Skip - not security critical
            }
            if (injectBinaryTagPropagation(BO, &F)) {
              modified = true;
            }
          }
        } else if (auto* LI = dyn_cast<LoadInst>(&I)) {
          // ============================================
          // NEW: Check if load is from tainted source
          // ============================================
          if (shouldTrackLoad(LI, &F)) {
            if (injectLoadTagPropagation(LI, &F)) {
              modified = true;
            }
          }
        } else if (auto* SI = dyn_cast<StoreInst>(&I)) {
          // ============================================
          // NEW: Check if storing tainted data
          // ============================================
          if (shouldTrackStore(SI, &F)) {
            if (injectStoreTagPropagation(SI, &F)) {
              modified = true;
            }
          }
        } else if (auto* AI = dyn_cast<AllocaInst>(&I)) {
          // ============================================
          // NEW: Only initialize if in security context
          // ============================================
          if (shouldInitializeAlloca(AI, &F)) {
            if (injectAllocaInitialization(AI, &F)) {
              modified = true;
            }
          }
        }
      }
    }

    if (modified) {
      errs() << "[SELECTIVE CO-DIFT INJECT] Modified: " << F.getName() << "\n";
    }
    return modified;
  }

  private:
  bool shouldInstrumentBinaryOp(BinaryOperator* BO) {
    switch (BO->getOpcode()) {
    case Instruction::Add:
    case Instruction::Sub:
    case Instruction::Mul:
    case Instruction::UDiv:
    case Instruction::SDiv:
    case Instruction::URem:
    case Instruction::SRem:
    case Instruction::Shl:
    case Instruction::LShr:
    case Instruction::AShr:
    case Instruction::And:
    case Instruction::Or:
    case Instruction::Xor:
      return true;
    default:
      return false;
    }
  }

  // ============================================
  // NEW: Selective instrumentation functions
  // ============================================

  bool shouldTrackBinaryOperation(BinaryOperator* BO, Function* F) {
    Value* op1 = BO->getOperand(0);
    Value* op2 = BO->getOperand(1);

    // RULE 1: Skip if both operands are constants
    if (isa<Constant>(op1) && isa<Constant>(op2)) {
      errs() << "  [SELECTIVE] Skipping: " << *BO << " (both constants)\n";
      return false;
    }

    // RULE 2: Check if used in security context
    if (isUsedInSecurityContext(BO, F)) {
      errs() << "  [SELECTIVE] Tracking (security context): " << *BO << "\n";
      return true;
    }

    // RULE 3: Check if either operand might be tainted
    if (mightBeTainted(op1, F) || mightBeTainted(op2, F)) {
      errs() << "  [SELECTIVE] Tracking (possible taint): " << *BO << "\n";
      return true;
    }

    // RULE 4: Default - skip (optimization!)
    errs() << "  [SELECTIVE] Skipping (no taint): " << *BO << "\n";
    return false;
  }

  bool shouldTrackLoad(LoadInst* LI, Function* F) {
    // RULE: Always track loads (they could be from tainted memory)
    // But we can optimize further by checking source
    errs() << "  [SELECTIVE] Tracking load: " << *LI << "\n";
    return true;
  }

  bool shouldTrackStore(StoreInst* SI, Function* F) {
    Value* storedValue = SI->getValueOperand();

    // RULE: Only track if storing potentially tainted data
    if (mightBeTainted(storedValue, F)) {
      errs() << "  [SELECTIVE] Tracking store (tainted data): " << *SI << "\n";
      return true;
    }

    errs() << "  [SELECTIVE] Skipping store (clean data): " << *SI << "\n";
    return false;
  }

  bool shouldInitializeAlloca(AllocaInst* AI, Function* F) {
    // RULE: Initialize only if alloca is used in security context
    if (isUsedInSecurityContext(AI, F)) {
      errs() << "  [SELECTIVE] Initializing alloca (security context): " << *AI << "\n";
      return true;
    }

    // OPTIONAL: Skip initialization for local temporaries
    errs() << "  [SELECTIVE] Skipping alloca init (local temp): " << *AI << "\n";
    return false;
  }

  bool mightBeTainted(Value* V, Function* F) {
    // RULE 1: Constants are never tainted
    if (isa<Constant>(V)) {
      return false;
    }

    // RULE 2: Check if comes from untrusted source
    if (Instruction* I = dyn_cast<Instruction>(V)) {
      // Check for calls to untrusted functions
      if (CallInst* CI = dyn_cast<CallInst>(I)) {
        Function* calledFunc = CI->getCalledFunction();
        if (calledFunc && isUntrustedFunction(calledFunc)) {
          return true;
        }
      }

      // Check for loads from potentially tainted memory
      if (isa<LoadInst>(I)) {
        return true; // Conservative: assume loads might be tainted
      }
    }

    // RULE 3: Check if derived from tainted value
    for (auto* U : V->users()) {
      if (Instruction* userInst = dyn_cast<Instruction>(U)) {
        if (mightBeTainted(userInst, F)) {
          return true;
        }
      }
    }

    return false; // Assume clean by default (optimization!)
  }

  bool isUntrustedFunction(Function* F) {
    if (!F)
      return false;

    StringRef name = F->getName();
    // List of functions that return untrusted data
    return name.contains("scanf") || name.contains("read") ||
           name.contains("recv") || name.contains("input") ||
           name.contains("untrusted") || name.contains("network");
  }

  bool isUsedInSecurityContext(Instruction* I, Function* F) {
    // Check if instruction is used in security-sensitive operations
    for (auto* U : I->users()) {
      if (isa<BranchInst>(U) || isa<ReturnInst>(U) || isa<CallInst>(U)) {
        return true;
      }
      if (StoreInst* SI = dyn_cast<StoreInst>(U)) {
        // Storing to memory that might be read later
        return true;
      }
    }
    return false;
  }

  // ============================================
  // Original instrumentation functions (modified for selective)
  // ============================================

  bool injectBinaryTagPropagation(BinaryOperator* BO, Function* F) {
    IRBuilder<> Builder(BO);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* op1 = BO->getOperand(0);
    Value* op2 = BO->getOperand(1);

    // ============================================
    // NEW: Get tags selectively
    // ============================================
    Value* tag1 = getTagForValueSelective(op1, Builder, readFunc, F);
    Value* tag2 = getTagForValueSelective(op2, Builder, readFunc, F);

    // ============================================
    // NEW: Skip if both tags are zero (clean)
    // ============================================
    if (isConstantZero(tag1) && isConstantZero(tag2)) {
      errs() << "  [SELECTIVE] Skipping tag propagation (both clean)\n";
      return false;
    }

    Value* resultTag = Builder.CreateOr(tag1, tag2, "codift-merge");
    Value* resultPtr = getPointerForValue(BO, Builder, F);
    Builder.CreateCall(writeFunc, {resultPtr, resultTag});

    errs() << "  [SELECTIVE] Injected binary op: " << getOpcodeName(BO->getOpcode()) << "\n";
    return true;
  }

  bool injectLoadTagPropagation(LoadInst* LI, Function* F) {
    IRBuilder<> Builder(LI);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* srcPtr = LI->getPointerOperand();
    Value* srcTag = Builder.CreateCall(readFunc, {srcPtr});

    Value* resultPtr = getPointerForValue(LI, Builder, F);
    Builder.CreateCall(writeFunc, {resultPtr, srcTag});

    errs() << "  [SELECTIVE] Injected load\n";
    return true;
  }

  bool injectStoreTagPropagation(StoreInst* SI, Function* F) {
    IRBuilder<> Builder(SI);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* storedValue = SI->getValueOperand();
    Value* destPtr = SI->getPointerOperand();
    Value* valueTag = getTagForValueSelective(storedValue, Builder, readFunc, F);

    // ============================================
    // NEW: Skip if tag is zero (clean data)
    // ============================================
    if (isConstantZero(valueTag)) {
      errs() << "  [SELECTIVE] Skipping store (clean data)\n";
      return false;
    }

    Builder.CreateCall(writeFunc, {destPtr, valueTag});
    errs() << "  [SELECTIVE] Injected store\n";
    return true;
  }

  bool injectAllocaInitialization(AllocaInst* AI, Function* F) {
    Instruction* nextInst = AI->getNextNode();
    if (!nextInst) {
      return false;
    }
    IRBuilder<> Builder(nextInst);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* zeroTag = Builder.getInt32(0);
    Builder.CreateCall(writeFunc, {AI, zeroTag});

    errs() << "  [SELECTIVE] Initialized alloca\n";
    return true;
  }

  // ============================================
  // NEW: Selective tag retrieval
  // ============================================
  Value* getTagForValueSelective(Value* val, IRBuilder<>& Builder,
                                 FunctionCallee readFunc, Function* F) {
    // Constants are always CLEAN (tag 0)
    if (isa<Constant>(val)) {
      return Builder.getInt32(0);
    }

    // Check if we already know this value is clean
    if (!mightBeTainted(val, F)) {
      return Builder.getInt32(0); // Assume clean (optimization!)
    }

    // Otherwise, read tag from memory
    Value* valPtr = getPointerForValue(val, Builder, F);
    return Builder.CreateCall(readFunc, {valPtr});
  }

  bool isConstantZero(Value* V) {
    if (auto* CI = dyn_cast<ConstantInt>(V)) {
      return CI->isZero();
    }
    return false;
  }

  // ============================================
  // Original helper functions (unchanged)
  // ============================================

  FunctionCallee getOrCreateReadFunc(Function* F) {
    Module* M = F->getParent();
    LLVMContext& Context = M->getContext();
    IRBuilder<> Builder(Context);
    FunctionType* readFuncType = FunctionType::get(
        Builder.getInt32Ty(),
        {Builder.getInt8PtrTy()},
        false);
    return M->getOrInsertFunction("ramReadFunc", readFuncType);
  }

  FunctionCallee getOrCreateWriteFunc(Function* F) {
    Module* M = F->getParent();
    LLVMContext& Context = M->getContext();
    IRBuilder<> Builder(Context);

    FunctionType* writeFuncType = FunctionType::get(
        Builder.getVoidTy(),
        {Builder.getInt8PtrTy(), Builder.getInt32Ty()},
        false);
    return M->getOrInsertFunction("ramWriteFunc", writeFuncType);
  }

  Value* getPointerForValue(Value* val, IRBuilder<>& Builder, Function* F) {
    if (val->getType()->isPointerTy()) {
      return Builder.CreatePointerCast(val, Builder.getInt8PtrTy());
    }

    BasicBlock* entryBB = &F->getEntryBlock();
    IRBuilder<> AllocaBuilder(entryBB, entryBB->getFirstInsertionPt());
    AllocaInst* tempAlloca = AllocaBuilder.CreateAlloca(
        val->getType(), nullptr, "codift_inject_temp");

    if (Instruction* valInst = dyn_cast<Instruction>(val)) {
      IRBuilder<> StoreBuilder(valInst->getNextNode());
      StoreBuilder.CreateStore(val, tempAlloca);
    } else {
      Builder.CreateStore(val, tempAlloca);
    }

    return Builder.CreatePointerCast(tempAlloca, Builder.getInt8PtrTy());
  }

  std::string getOpcodeName(unsigned opcode) {
    switch (opcode) {
    case Instruction::Add:
      return "add";
    case Instruction::Sub:
      return "sub";
    case Instruction::Mul:
      return "mul";
    case Instruction::UDiv:
      return "udiv";
    case Instruction::SDiv:
      return "sdiv";
    case Instruction::And:
      return "and";
    case Instruction::Or:
      return "or";
    case Instruction::Xor:
      return "xor";
    default:
      return "unknown";
    }
  }
};
} // namespace

char SelectiveCodiftInjectPass::ID = 0;
static RegisterPass<SelectiveCodiftInjectPass>
    Z("codift-inject-selective", "SELECTIVE CO-DIFT inject pass");