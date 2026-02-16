// codift_check_pass.cpp - SELECTIVE VERSION
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <set>

using namespace llvm;

namespace {
class SelectiveCodiftCheckPass : public FunctionPass {
  public:
  static char ID;
  SelectiveCodiftCheckPass() : FunctionPass(ID) {}

  bool runOnFunction(Function& F) override {
    bool modified = false;

    // Only check functions that handle security operations
    if (!isSecurityRelevantFunction(F)) {
      return false;
    }

    errs() << "[SELECTIVE CHECK] Processing security function: " << F.getName() << "\n";

    for (auto& BB : F) {
      for (auto& I : BB) {
        // ========== 1. Check conditional branches ==========
        if (auto* BI = dyn_cast<BranchInst>(&I)) {
          if (BI->isConditional()) {
            Value* condition = BI->getCondition();
            // Only check if condition might be tainted
            if (mightBeTainted(condition, &F)) {
              if (injectSecurityCheck(condition, &I, &F)) {
                modified = true;
              }
            }
          }
        }

        // ========== 2. Check security-sensitive calls ==========
        else if (auto* CI = dyn_cast<CallInst>(&I)) {
          Function* calledFunc = CI->getCalledFunction();
          if (calledFunc && isSecuritySink(calledFunc)) {
            // Check all arguments to this security sink
            for (unsigned i = 0; i < CI->arg_size(); ++i) {
              if (mightBeTainted(CI->getArgOperand(i), &F)) {
                if (injectSecurityCheck(CI->getArgOperand(i), &I, &F)) {
                  modified = true;
                }
              }
            }
          }
        }

        // ========== 3. Check returns from sensitive functions ==========
        else if (auto* RI = dyn_cast<ReturnInst>(&I)) {
          if (RI->getReturnValue() && mightBeTainted(RI->getReturnValue(), &F)) {
            if (injectSecurityCheck(RI->getReturnValue(), &I, &F)) {
              modified = true;
            }
          }
        }
      }
    }

    if (modified) {
      errs() << "[SELECTIVE CHECK] Modified: " << F.getName() << "\n";
    }

    return modified;
  }

  private:
  // ============== Security Policy ==============

  bool isSecurityRelevantFunction(Function& F) {
    StringRef name = F.getName();

    // List of security-relevant functions in smart lock
    std::set<std::string> securityFunctions = {
        "verify_pin",
        "activate_lock",
        "grant_access",
        "sound_alarm",
        "display_access_message",
        "check_authentication",
        "main"}; // -> main added only for debugging

    return securityFunctions.count(name.str()) > 0;
  }

  bool isSecuritySink(Function* F) {
    if (!F)
      return false;
    StringRef name = F->getName();

    // Functions that perform security-critical operations
    return name.contains("lock") ||
           name.contains("grant") ||
           name.contains("alarm") ||
           name.contains("access") ||
           name.contains("authenticate");
  }

  bool mightBeTainted(Value* V, Function* F) {
    // Simple taint analysis: check if value comes from untrusted source
    if (isa<Constant>(V)) {
      return false;
    }

    // Check if it's a load from tainted memory region
    if (auto* LI = dyn_cast<LoadInst>(V)) {
      // In real implementation, track tainted memory regions
      return true;
    }

    // Check if it comes from an untrusted function call
    if (auto* CI = dyn_cast<CallInst>(V)) {
      Function* calledFunc = CI->getCalledFunction();
      if (calledFunc && isUntrustedSource(calledFunc)) {
        return true;
      }
    }

    // Check operands
    if (auto* I = dyn_cast<Instruction>(V)) {
      for (auto& Op : I->operands()) {
        if (mightBeTainted(Op.get(), F)) {
          return true;
        }
      }
    }

    return false;
  }

  bool isUntrustedSource(Function* F) {
    if (!F)
      return false;
    StringRef name = F->getName();

    // Functions that read external input
    return name.contains("keypad") ||
           name.contains("read") ||
           name.contains("input") ||
           name.contains("serial") ||
           name.contains("network");
  }

  // ============== Check Injection ==============

  bool injectSecurityCheck(Value* valueToCheck, Instruction* location, Function* parentFunc) {
    IRBuilder<> Builder(location);

    // Get or create runtime functions
    Module* M = parentFunc->getParent();
    LLVMContext& Context = M->getContext();

    // ramReadFunc declaration
    FunctionType* readFuncType = FunctionType::get(
        Type::getInt32Ty(Context),
        {Type::getInt8PtrTy(Context)},
        false);
    FunctionCallee readFunc = M->getOrInsertFunction("ramReadFunc", readFuncType);

    // secExcFunc declaration
    FunctionType* secFuncType = FunctionType::get(
        Type::getVoidTy(Context),
        {Type::getInt32Ty(Context)},
        false);
    FunctionCallee secFunc = M->getOrInsertFunction("secExcFunc", secFuncType);

    // Convert value to pointer for tag reading
    Value* valueAsPtr;
    if (valueToCheck->getType()->isPointerTy()) {
      valueAsPtr = Builder.CreatePointerCast(valueToCheck, Type::getInt8PtrTy(Context));
    } else {
      // Create temporary allocation
      AllocaInst* tempAlloca = Builder.CreateAlloca(
          valueToCheck->getType(),
          nullptr,
          "codift_check_temp");
      Builder.CreateStore(valueToCheck, tempAlloca);
      valueAsPtr = Builder.CreatePointerCast(tempAlloca, Type::getInt8PtrTy(Context));
    }

    // Insert the check
    Value* tag = Builder.CreateCall(readFunc, {valueAsPtr});
    Builder.CreateCall(secFunc, {tag});

    errs() << "[SELECTIVE CHECK] Injected check for: " << *valueToCheck << "\n";
    return true;
  }
};

char SelectiveCodiftCheckPass::ID = 1;
static RegisterPass<SelectiveCodiftCheckPass>
    X("codift-check-selective", "Selective CO-DIFT Check Pass");
} // namespace