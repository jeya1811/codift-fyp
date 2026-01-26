#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class CodiftPass : public FunctionPass {
public:
  static char ID;
  CodiftPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    bool modified = false;

    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *BI = dyn_cast<BranchInst>(&I)) {
          if (BI->isConditional()) {
            Value *condition = BI->getCondition();
            if (injectSecurityCheck(condition, &I, &F)) {
              modified = true;
            }
          }
        } else if (auto *SI = dyn_cast<SwitchInst>(&I)) {
          Value *condition = SI->getCondition();
          if (injectSecurityCheck(condition, &I, &F)) {
            modified = true;
          }
        } else if (auto *CI = dyn_cast<CallInst>(&I)) {
          Value *calledValue = CI->getCalledOperand();
          if (injectSecurityCheck(calledValue, &I, &F)) {
            modified = true;
          }
        } else if (auto *RI = dyn_cast<ReturnInst>(&I)) {
          errs() << "[CODIFT] Found return instruction in function: "
                 << F.getName() << "\n";
        }
      }
    }
    return modified;
  }

private:
  bool injectSecurityCheck(Value *valueToCheck, Instruction *location,
                           Function *parentFunc) {
    IRBuilder<> Builder(location);
    FunctionType *readFuncType = FunctionType::get(
        Builder.getInt32Ty(), {Builder.getInt8PtrTy()}, false);
    FunctionCallee readFunc = parentFunc->getParent()->getOrInsertFunction(
        "ramReadFunc", readFuncType);

    FunctionType *secFuncType =
        FunctionType::get(Builder.getVoidTy(), {Builder.getInt32Ty()}, false);
    FunctionCallee secFunc =
        parentFunc->getParent()->getOrInsertFunction("secExcFunc", secFuncType);
    Value *valueAsVoidPtr;
    if (valueToCheck->getType()->isPointerTy()) {
      valueAsVoidPtr =
          Builder.CreatePointerCast(valueToCheck, Builder.getInt8PtrTy());
    } else {
      AllocaInst *tempAlloca =
          Builder.CreateAlloca(valueToCheck->getType(), nullptr, "codift_temp");
      Builder.CreateStore(valueToCheck, tempAlloca);
      valueAsVoidPtr =
          Builder.CreatePointerCast(tempAlloca, Builder.getInt8PtrTy());
    }
    Builder.SetInsertPoint(location);
    CallInst *tagValue = Builder.CreateCall(readFunc, {valueAsVoidPtr});
    Builder.CreateCall(secFunc, {tagValue});
    errs() << "[CODIFT] Injected security check before instruction in "
           << parentFunc->getName() << "\n";
    return true;
  }
};
} // namespace

char CodiftPass::ID = 0;
static RegisterPass<CodiftPass> X("codift", "CODIFT check pass");