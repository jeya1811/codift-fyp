#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <set>
#include <vector>
using namespace llvm;

namespace {
struct InjectPass : public FunctionPass {
  static char ID;
  InjectPass() : FunctionPass(ID) {}

  bool runOnFunction(Function& F) override {
    static const std::set<std::string> WL = {
        "read_input", "verify", "main"};
    if (!WL.count(F.getName().str()))
      return false;
    errs() << "[Inject] " << F.getName() << "\n";

    Module* M = F.getParent();
    bool isRI = (F.getName() == "read_input");

    if (isRI) {
      Instruction* first = &*F.getEntryBlock().getFirstInsertionPt();
      IRBuilder<> B(first);
      FunctionCallee initFn = M->getOrInsertFunction("codift_init",
                                                     FunctionType::get(Type::getVoidTy(M->getContext()), {}, false));
      B.CreateCall(initFn, {});
      errs() << "[Inject]  auto-injected codift_init() at entry\n";
    }

    GlobalVariable* sGV = nullptr;
    for (auto& G : M->globals()) {
      StringRef n = G.getName();
      if (n == "s" || n.startswith("s.")) {
        sGV = &G;
        break;
      }
    }

    std::vector<Instruction*> work;
    for (auto& BB : F)
      for (auto& I : BB) {
        if (auto* SI = dyn_cast<StoreInst>(&I)) {
          if (isa<Constant>(SI->getValueOperand()))
            continue;
          if (isRI) {
            if (SI->getPointerOperand()->getType()->isPointerTy())
              work.push_back(&I);
          } else if (sGV) {
            if (tracesTo(SI->getPointerOperand(), sGV))
              work.push_back(&I);
          }
        }
      }

    bool changed = isRI;
    int sc = 0, xc = 0;
    for (auto* I : work) {
      if (auto* SI = dyn_cast<StoreInst>(I)) {
        if (tagStore(SI, M)) {
          changed = true;
          sc++;
        }
      }
    }
    if (sc || xc)
      errs() << "[Inject]  " << sc << " store(s) tagged TAINTED\n";
    return changed;
  }

  private:
  bool tracesTo(Value* v, GlobalVariable* gv) {
    if (!v)
      return false;
    if (auto* G = dyn_cast<GlobalVariable>(v))
      return G == gv;
    if (auto* G = dyn_cast<GetElementPtrInst>(v))
      return tracesTo(G->getPointerOperand(), gv);
    if (auto* B = dyn_cast<BitCastInst>(v))
      return tracesTo(B->getOperand(0), gv);
    if (auto* C = dyn_cast<ConstantExpr>(v)) {
      unsigned op = C->getOpcode();
      if (op == Instruction::BitCast || op == Instruction::GetElementPtr ||
          op == Instruction::AddrSpaceCast)
        return tracesTo(C->getOperand(0), gv);
    }
    return false;
  }
  bool tagStore(StoreInst* SI, Module* M) {
    Instruction* nx = SI->getNextNode();
    if (!nx)
      return false;
    IRBuilder<> B(nx);
    LLVMContext& C = M->getContext();
    Type *i8 = Type::getInt8Ty(C), *i8p = PointerType::getUnqual(i8);
    Value* d = B.CreateBitCast(SI->getPointerOperand(), i8p, "cd.p");
    FunctionCallee wf = M->getOrInsertFunction("ramWriteFunc",
                                               FunctionType::get(Type::getVoidTy(C), {i8p, i8}, false));
    B.CreateCall(wf, {d, B.getInt8(1)});
    return true;
  }
};
} // namespace
char InjectPass::ID = 0;
static RegisterPass<InjectPass> Z("inject", "CO-DIFT Inject", false, false);
