#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace{
    class CountFunc: public FunctionPass{
        public:
        static char ID;
        CountFunc(): FunctionPass(ID){}

        bool runOnFunction(Function &F) override{
            errs()<< "Hello from function: "<< F.getName()<< "\n";
            int blockCount= 0;
            for(auto &BB: F){
                blockCount++;
            }
            errs()<< " Has "<< blockCount<< " Basic blocks\n";
            return false;
        }
    };
}
char CountFunc::ID = 0;

static RegisterPass<CountFunc> X("count", "counting the function....");