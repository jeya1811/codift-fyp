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

    for (auto& BB : F) {
      for (auto& I : BB) {
        if (isa<BinaryOperator>(&I) || isa<LoadInst>(&I) || isa<StoreInst>(&I)) {
          WorkList.push_back(&I);
        } else if (auto* CI = dyn_cast<CallInst>(&I)) {
          Function* f = CI->getCalledFunction();
          // RESTORED: Catching scanf and other untrusted sources
          if (f && isUntrustedSource(f)) {
            WorkList.push_back(&I);
          }
        }
      }
    }

    for (Instruction* I : WorkList) {
      if (auto* BO = dyn_cast<BinaryOperator>(I))
        injectBinary(BO, &F);
      else if (auto* LI = dyn_cast<LoadInst>(I))
        injectLoad(LI, &F);
      else if (auto* SI = dyn_cast<StoreInst>(I))
        injectStore(SI, &F);
      else if (auto* CI = dyn_cast<CallInst>(I))
        injectTaintSource(CI, &F);
      modified = true;
    }
    return modified;
  }

  private:
  // RESTORED: Your original untrusted source logic
  bool isUntrustedSource(Function* F) {
    if (!F)
      return false;
    StringRef name = F->getName();
    return name.contains("scanf") || name.contains("read") ||
           name.contains("recv") || name.contains("input") ||
           name.contains("keypad") || name.contains("serial");
  }

  void injectTaintSource(CallInst* CI, Function* F) {
    // Taint after the call finishes so we don't overwrite the data
    IRBuilder<> B(CI->getNextNode());

    // RESTORED: Taint all pointer arguments (where scanf writes its data)
    for (unsigned i = 0; i < CI->arg_size(); ++i) {
      Value* arg = CI->getArgOperand(i);
      if (arg->getType()->isPointerTy()) {
        B.CreateCall(getWrite(F), {arg, B.getInt8(1)});
      }
    }
  }

  void injectBinary(BinaryOperator* BO, Function* F) {
    IRBuilder<> B(BO);
    Value* t1 = B.CreateCall(getRead(F), {getPtr(BO->getOperand(0), B, F)});
    Value* t2 = B.CreateCall(getRead(F), {getPtr(BO->getOperand(1), B, F)});
    B.CreateCall(getWrite(F), {getPtr(BO, B, F), B.CreateOr(t1, t2)});
  }

  void injectLoad(LoadInst* LI, Function* F) {
    IRBuilder<> B(LI);
    Value* tag = B.CreateCall(getRead(F), {LI->getPointerOperand()});
    B.CreateCall(getWrite(F), {getPtr(LI, B, F), tag});
  }

  void injectStore(StoreInst* SI, Function* F) {
    IRBuilder<> B(SI);
    Value* tag = B.CreateCall(getRead(F), {getPtr(SI->getValueOperand(), B, F)});
    B.CreateCall(getWrite(F), {SI->getPointerOperand(), tag});
  }

  FunctionCallee getRead(Function* F) {
    return F->getParent()->getOrInsertFunction("ramReadFunc", FunctionType::get(Type::getInt8Ty(F->getContext()), {Type::getInt8PtrTy(F->getContext())}, false));
  }

  FunctionCallee getWrite(Function* F) {
    return F->getParent()->getOrInsertFunction("ramWriteFunc", FunctionType::get(Type::getVoidTy(F->getContext()), {Type::getInt8PtrTy(F->getContext()), Type::getInt8Ty(F->getContext())}, false));
  }

  // FIXED: Domination-safe pointer retrieval
  Value* getPtr(Value* V, IRBuilder<>& B, Function* F) {
    if (V->getType()->isPointerTy())
      return B.CreatePointerCast(V, B.getInt8PtrTy());

    BasicBlock& Entry = F->getEntryBlock();
    IRBuilder<> TmpB(&Entry, Entry.getFirstInsertionPt());
    AllocaInst* a = TmpB.CreateAlloca(V->getType(), nullptr, "prop_tmp");

    if (Instruction* I = dyn_cast<Instruction>(V)) {
      IRBuilder<> StoreB(I->getNextNode());
      StoreB.CreateStore(V, a);
    } else if (Argument* Arg = dyn_cast<Argument>(V)) {
      TmpB.CreateStore(V, a);
    } else {
      B.CreateStore(V, a);
    }
    return B.CreatePointerCast(a, B.getInt8PtrTy());
  }
};
} // namespace
char SelectiveCodiftInjectPass::ID = 0;
static RegisterPass<SelectiveCodiftInjectPass> Z("inject-selective", "Selective CODIFT Inject Pass");