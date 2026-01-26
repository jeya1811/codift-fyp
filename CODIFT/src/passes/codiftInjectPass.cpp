#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class CodiftInjectPass : public FunctionPass {
  public:
  static char ID;
  CodiftInjectPass() : FunctionPass(ID) {}
  bool runOnFunction(Function& F) override {
    bool modified = false;
    errs() << "[CO-DIFT INJECT] Processing: " << F.getName() << "\n";

    for (auto& BB : F) {
      for (auto& I : BB) {
        if (auto* BO = dyn_cast<BinaryOperator>(&I)) {
          if (shouldInstrumentBinaryOp(BO)) {
            if (injectBinaryTagPropagation(BO, &F)) {
              modified = true;
            }
          }
        } else if (auto* LI = dyn_cast<LoadInst>(&I)) {
          if (injectLoadTagPropagation(LI, &F)) {
            modified = true;
          }
        } else if (auto* SI = dyn_cast<StoreInst>(&I)) {
          if (injectStoreTagPropagation(SI, &F)) {
            modified = true;
          }
        } else if (auto* AI = dyn_cast<AllocaInst>(&I)) {
          if (injectAllocaInitialization(AI, &F)) {
            modified = true;
          }
        }
      }
    }
    if (modified) {
      errs() << "[CO-DIFT INJECT] Modified: " << F.getName() << "\n";
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
  bool injectBinaryTagPropagation(BinaryOperator* BO, Function* F) {
    IRBuilder<> Builder(BO);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* op1 = BO->getOperand(0);
    Value* op2 = BO->getOperand(1);

    Value* tag1 = getTagForValue(op1, Builder, readFunc, F);
    Value* tag2 = getTagForValue(op2, Builder, readFunc, F);

    Value* resultTag = Builder.CreateOr(tag1, tag2, "codift-merge");

    Value* resultPtr = getPointerForValue(BO, Builder, F);
    Builder.CreateCall(writeFunc, {resultPtr, resultTag});

    errs() << "[INJECT] Binary op: " << getOpcodeName(BO->getOpcode()) << "\n";
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

    errs() << "[INJECT] Load\n";
    return true;
  }
  bool injectStoreTagPropagation(StoreInst* SI, Function* F) {
    IRBuilder<> Builder(SI);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* storedValue = SI->getValueOperand();
    Value* destPtr = SI->getPointerOperand();
    Value* valueTag = getTagForValue(storedValue, Builder, readFunc, F);

    Builder.CreateCall(writeFunc, {destPtr, valueTag});
    errs() << "[INJECT] Store\n";
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

    errs() << "[INJECT] Alloca initialized\n";
    return true;
  }
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
  Value* getTagForValue(Value* val, IRBuilder<>& Builder, FunctionCallee readFunc, Function* F) {
    if (isa<Constant>(val)) {
      return Builder.getInt32(0);
    }
    Value* valPtr = getPointerForValue(val, Builder, F);
    return Builder.CreateCall(readFunc, {valPtr});
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

char CodiftInjectPass::ID = 0;
static RegisterPass<CodiftInjectPass> Y("codift-inject", "CODIFT inject pass");