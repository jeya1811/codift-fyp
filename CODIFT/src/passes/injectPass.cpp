#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include <vector>

using namespace llvm;

namespace {
class SelectiveCodiftInjectPass : public FunctionPass {
  public:
  static char ID;
  SelectiveCodiftInjectPass() : FunctionPass(ID) {}

  bool runOnFunction(Function& F) override {
    bool modified = false;
    std::vector<Instruction*> WorkList;

    // Scan phase
    for (auto& BB : F) {
      for (auto& I : BB) {
        if (auto* BO = dyn_cast<BinaryOperator>(&I)) {
          if (shouldInstrumentBinaryOp(BO) && shouldTrackBinaryOperation(BO, &F))
            WorkList.push_back(&I);
        } else if (auto* LI = dyn_cast<LoadInst>(&I)) {
          WorkList.push_back(&I);
        } else if (auto* SI = dyn_cast<StoreInst>(&I)) {
          if (shouldTrackStore(SI, &F))
            WorkList.push_back(&I);
        } else if (auto* AI = dyn_cast<AllocaInst>(&I)) {
          if (shouldInitializeAlloca(AI, &F))
            WorkList.push_back(&I);
        } else if (auto* CI = dyn_cast<CallInst>(&I)) {
          Function* calledFunc = CI->getCalledFunction();
          if (calledFunc && isUntrustedSource(calledFunc))
            WorkList.push_back(&I);
        }
      }
    }

    // Injection phase
    for (Instruction* I : WorkList) {
      if (auto* BO = dyn_cast<BinaryOperator>(I))
        modified |= injectBinaryTagPropagation(BO, &F);
      else if (auto* LI = dyn_cast<LoadInst>(I))
        modified |= injectLoadTagPropagation(LI, &F);
      else if (auto* SI = dyn_cast<StoreInst>(I))
        modified |= injectStoreTagPropagation(SI, &F);
      else if (auto* AI = dyn_cast<AllocaInst>(I))
        modified |= injectAllocaInitialization(AI, &F);
      else if (auto* CI = dyn_cast<CallInst>(I))
        modified |= injectTaintSource(CI, &F);
    }

    return modified;
  }

  private:
  // ============== Selection Logic ==============

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

  bool shouldTrackBinaryOperation(BinaryOperator* BO, Function* F) {
    Value* op1 = BO->getOperand(0);
    Value* op2 = BO->getOperand(1);

    if (isa<Constant>(op1) && isa<Constant>(op2))
      return false;

    if (isUsedInSecurityContext(BO))
      return true;

    return mightBeTainted(op1, F) || mightBeTainted(op2, F);
  }

  bool shouldTrackStore(StoreInst* SI, Function* F) {
    return mightBeTainted(SI->getValueOperand(), F);
  }

  bool shouldInitializeAlloca(AllocaInst* AI, Function* F) {
    return isUsedInSecurityContext(AI);
  }

  // ============== Taint Analysis ==============

  bool mightBeTainted(Value* V, Function* F) {
    if (isa<Constant>(V))
      return false;

    if (auto* LI = dyn_cast<LoadInst>(V))
      return true;

    if (auto* CI = dyn_cast<CallInst>(V)) {
      Function* calledFunc = CI->getCalledFunction();
      if (calledFunc && isUntrustedSource(calledFunc))
        return true;
    }

    if (auto* I = dyn_cast<Instruction>(V)) {
      for (auto& Op : I->operands()) {
        if (mightBeTainted(Op.get(), F))
          return true;
      }
    }

    return false;
  }

  bool isUntrustedSource(Function* F) {
    if (!F)
      return false;
    StringRef name = F->getName();
    return name.contains("scanf") ||
           name.contains("read") ||
           name.contains("recv") ||
           name.contains("input") ||
           name.contains("keypad") ||
           name.contains("serial") ||
           name.contains("network");
  }

  bool isUsedInSecurityContext(Instruction* I) {
    for (auto* U : I->users()) {
      if (isa<BranchInst>(U) || isa<ReturnInst>(U) ||
          isa<CallInst>(U) || isa<StoreInst>(U))
        return true;
    }
    return false;
  }

  // ============== Instrumentation ==============

  bool injectBinaryTagPropagation(BinaryOperator* BO, Function* F) {
    IRBuilder<> Builder(BO);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* tag1 = getTagForValue(BO->getOperand(0), Builder, readFunc, F);
    Value* tag2 = getTagForValue(BO->getOperand(1), Builder, readFunc, F);

    if (isConstantZero(tag1) && isConstantZero(tag2))
      return false;

    Value* resultTag = Builder.CreateOr(tag1, tag2, "codift.merge");
    Value* resultPtr = getSafePtr(BO, Builder, F);
    Builder.CreateCall(writeFunc, {resultPtr, resultTag});
    return true;
  }

  bool injectLoadTagPropagation(LoadInst* LI, Function* F) {
    IRBuilder<> Builder(LI);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* srcTag = Builder.CreateCall(readFunc, {LI->getPointerOperand()});
    Builder.CreateCall(writeFunc, {getSafePtr(LI, Builder, F), srcTag});
    return true;
  }

  bool injectStoreTagPropagation(StoreInst* SI, Function* F) {
    IRBuilder<> Builder(SI);
    FunctionCallee readFunc = getOrCreateReadFunc(F);
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    Value* valueTag = getTagForValue(SI->getValueOperand(), Builder, readFunc, F);
    if (isConstantZero(valueTag))
      return false;

    Builder.CreateCall(writeFunc, {SI->getPointerOperand(), valueTag});
    return true;
  }

  bool injectAllocaInitialization(AllocaInst* AI, Function* F) {
    IRBuilder<> Builder(AI->getNextNode());
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);
    Builder.CreateCall(writeFunc, {AI, Builder.getInt8(0)});
    return true;
  }

  bool injectTaintSource(CallInst* CI, Function* F) {
    IRBuilder<> Builder(CI->getNextNode());
    FunctionCallee writeFunc = getOrCreateWriteFunc(F);

    bool modified = false;
    for (unsigned i = 0; i < CI->arg_size(); ++i) {
      Value* arg = CI->getArgOperand(i);
      if (arg->getType()->isPointerTy()) {
        Builder.CreateCall(writeFunc, {arg, Builder.getInt8(1)});
        modified = true;
      }
    }
    return modified;
  }

  // ============== Helpers ==============

  Value* getTagForValue(Value* val, IRBuilder<>& Builder,
                        FunctionCallee readFunc, Function* F) {
    if (isa<Constant>(val))
      return Builder.getInt8(0);
    if (!mightBeTainted(val, F))
      return Builder.getInt8(0);
    return Builder.CreateCall(readFunc, {getSafePtr(val, Builder, F)});
  }

  bool isConstantZero(Value* V) {
    if (auto* CI = dyn_cast<ConstantInt>(V))
      return CI->isZero();
    return false;
  }

  FunctionCallee getOrCreateReadFunc(Function* F) {
    Module* M = F->getParent();
    LLVMContext& Context = M->getContext();
    FunctionType* readFT = FunctionType::get(Type::getInt8Ty(Context),
                                             {Type::getInt8PtrTy(Context)},
                                             false);
    return M->getOrInsertFunction("ramReadFunc", readFT);
  }

  FunctionCallee getOrCreateWriteFunc(Function* F) {
    Module* M = F->getParent();
    LLVMContext& Context = M->getContext();
    FunctionType* writeFT = FunctionType::get(Type::getVoidTy(Context),
                                              {Type::getInt8PtrTy(Context),
                                               Type::getInt8Ty(Context)},
                                              false);
    return M->getOrInsertFunction("ramWriteFunc", writeFT);
  }

  // Dominance-safe pointer retrieval
  Value* getSafePtr(Value* V, IRBuilder<>& B, Function* F) {
    if (V->getType()->isPointerTy())
      return B.CreatePointerCast(V, Type::getInt8PtrTy(F->getContext()));

    BasicBlock& Entry = F->getEntryBlock();
    IRBuilder<> TmpB(&Entry, Entry.getFirstInsertionPt());
    AllocaInst* A = TmpB.CreateAlloca(V->getType(), nullptr, "codift_tmp");

    if (Instruction* I = dyn_cast<Instruction>(V)) {
      IRBuilder<> StoreB(I->getNextNode());
      StoreB.CreateStore(V, A);
    } else {
      B.CreateStore(V, A);
    }

    return B.CreatePointerCast(A, Type::getInt8PtrTy(F->getContext()));
  }
};
} // namespace

char SelectiveCodiftInjectPass::ID = 0;
static RegisterPass<SelectiveCodiftInjectPass>
    Z("inject", "Selective CODIFT Inject Pass");