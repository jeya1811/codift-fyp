```bash
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ sudo apt update && sudo apt upgrade -y
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ sudo apt install -y git wget curl
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ sudo apt install -y build-essential cmake ninja-build python3 python3-pip
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ wget https://apt.llvm.org/llvm.sh
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ chmod +x llvm.sh
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ sudo ./llvm.sh 15
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ sudo apt install -y llvm-15-dev llvm-15-tools clang-15 libclang-15-dev
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ git --version && gcc --version && cmake --version && clang-15 --version && llvm-config-15 --version
git version 2.43.0
gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO 
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

cmake version 3.28.3

CMake suite maintained and supported by Kitware (kitware.com/cmake).
Ubuntu clang version 15.0.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
15.0.7
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang++-15 -S -emit-llvm src/code/test.cpp -o llvmIR/test.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cmake -DLLVM_DIR=/usr/lib/llvm-15/cmake ../src
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features       
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test HAVE_FFI_CALL
-- Performing Test HAVE_FFI_CALL - Success
-- Found FFI: /usr/lib/x86_64-linux-gnu/libffi.so  
-- Performing Test Terminfo_LINKABLE
-- Performing Test Terminfo_LINKABLE - Success
-- Found Terminfo: /usr/lib/x86_64-linux-gnu/libtinfo.so  
-- Found ZLIB: /usr/lib/x86_64-linux-gnu/libz.so (found version "1.3")  
-- Could NOT find zstd (missing: zstd_LIBRARY zstd_INCLUDE_DIR) 
-- Found LibXml2: /usr/lib/x86_64-linux-gnu/libxml2.so (found version "2.9.14") 
-- Configuring done (22.6s)
-- Generating done (0.5s)
-- Build files have been written to: /mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
-- Could NOT find zstd (missing: zstd_LIBRARY zstd_INCLUDE_DIR) 
-- Configuring done (2.7s)
-- Generating done (0.3s)
-- Build files have been written to: /mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build
[ 50%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[100%] Linking CXX shared module CountFunc.so
[100%] Built target CountFunc
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[ 50%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[100%] Linking CXX shared module CountFunc.so
[100%] Built target CountFunc
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CountFunc.so -count -enable-new-pm=0 -disable-output llvmIR/test.ll
Hello from function: main
 Has 1 Basic blocks
Hello from function: _Z9conditionib
 Has 4 Basic blocks
Hello from function: _Z4loopi      
 Has 5 Basic blocks
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ gcc -c -fPIC src/runtime/codift_runtime.c -o codift_runtime.o
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ gcc -shared codift_runtime.o -o libcodiftruntime.so
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ gcc tests/test_runtime.c -L. -lcodiftruntime -o test_runtime
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./test_runtime
===CO-DIFT Runtime Test===

[CODIFT] Initialized
(tag memory size: 262144 bytes)
1.Testing with clean data:
Value: 42
[CODIFT] Cleaning region: 0x7ffe6a6e254c (size: 4)
[CODIFT] Write Tag: address= 0x7ffe6a6e254c <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe6a6e254d <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe6a6e254e <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe6a6e254f <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe6a6e254c -> tag= 0
    Tag read back: 0

[CODIFT SECURITY CHECK PASSED]
Processing normal input: 42

2.Testing with Tainted data:
Value: 200 (simulating untrusted input)
[CODIFT] Tainting region: 0x7ffe6a6e2550 (size: 4)
[CODIFT] Write Tag: address= 0x7ffe6a6e2550 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffe6a6e2551 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffe6a6e2552 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffe6a6e2553 <- tag= 1
[CODIFT] Read Tag: address= 0x7ffe6a6e2550 -> tag= 1
    Tag read back: 1
    Attempting to process tainted data...

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
Processing large input: 200

3.Cleaning the tainted data:
[CODIFT] Cleaning region: 0x7ffe6a6e2550 (size: 4)
[CODIFT] Write Tag: address= 0x7ffe6a6e2550 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe6a6e2551 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe6a6e2552 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe6a6e2553 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe6a6e2550 -> tag= 0
    Tag after cleaning: 0
Processing large input: 200

===Test Complete===
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 -S -emit-llvm src/code/simple_test.c -o llvmIR/simple_test.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[ 25%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 50%] Linking CXX shared module libCountFunc.so
[ 50%] Built target CountFunc
[ 75%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift -enable-new-pm=0 llvmIR/simple_test.ll -S -o llvmIR/simple_test_protected.ll
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-format --style=llvm -dump-config > .clang-format
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
-- Configuring done (2.6s)
-- Generating done (0.8s)
-- Build files have been written to: /mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build
make[1]: Warning: File 'CMakeFiles/Makefile2' has modification time 2.3 s in the future
make[2]: Warning: File 'CMakeFiles/CountFunc.dir/progress.make' has modification time 2.2 s in the future
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
make[2]: Warning: File 'CMakeFiles/CountFunc.dir/progress.make' has modification time 2 s in the future
[ 25%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 50%] Linking CXX shared module libCountFunc.so
make[2]: warning:  Clock skew detected.  Your build may be incomplete.
[ 50%] Built target CountFunc
[ 75%] Building CXX object CMakeFiles/CodiftCheckPass.dir/passes/codiftCheckPass.cpp.o
[100%] Linking CXX shared module CodiftCheckPass.so
[100%] Built target CodiftCheckPass
make[1]: warning:  Clock skew detected.  Your build may be incomplete.

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ rm -r build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ mkdir build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cmake -DLLVM_DIR=/usr/lib/llvm-15/cmake ../src
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test HAVE_FFI_CALL
-- Performing Test HAVE_FFI_CALL - Success
-- Found FFI: /usr/lib/x86_64-linux-gnu/libffi.so  
-- Performing Test Terminfo_LINKABLE
-- Performing Test Terminfo_LINKABLE - Success
-- Found Terminfo: /usr/lib/x86_64-linux-gnu/libtinfo.so  
-- Found ZLIB: /usr/lib/x86_64-linux-gnu/libz.so (found version "1.3")  
-- Found zstd: /usr/lib/x86_64-linux-gnu/libzstd.so  
-- Found LibXml2: /usr/lib/x86_64-linux-gnu/libxml2.so (found version "2.9.14") 
-- Configuring done (16.3s)
-- Generating done (0.4s)
-- Build files have been written to: /mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make 
[ 25%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 50%] Linking CXX shared module libCountFunc.so
[ 50%] Built target CountFunc
[ 75%] Building CXX object CMakeFiles/CodiftCheckPass.dir/passes/codiftCheckPass.cpp.o
[100%] Linking CXX shared module CodiftCheckPass.so
[100%] Built target CodiftCheckPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftCheckPass.so -codift-check -enable-new-pm=0 llvmIR/simple_test.ll -S -o llvmIR/simple_test_protected.ll
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[ 20%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 40%] Linking CXX shared module libCountFunc.so
[ 40%] Built target CountFunc
[ 60%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftCheckPass.cpp.o
[ 80%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftInjectPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ opt-15 -load ./CodiftPass.so --help-list 2>&1 | grep -i "codift"
      --codift-check                                                       - CODIFT check pass
      --codift-inject                                                      - CODIFT inject pass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-inject -enable-new-pm=0 llvmIR/simple_test.ll -S -o llvmIR/simple_test_injectOnly.ll
[CO-DIFT INJECT] Processing: main
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject llvmIR/simple_test.ll -enable-new-pm=0 -S -o llvmIR/simple_test_checkAndInject.ll
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main        
[CO-DIFT INJECT] Processing: main
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Alloca initialized
[INJECT] Store
[CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ echo "ramReadFunc:" $(grep -c "ramReadFunc" llvmIR/simple_test_checkAndInject.ll)
ramReadFunc: 6
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ echo "ramWriteFunc:" $(grep -c "ramWriteFunc" llvmIR/simple_test_checkAndInject.ll)
ramWriteFunc: 9
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ echo "secExcFunc:" $(grep -c "secExcFunc" llvmIR/simple_test_checkAndInject.ll)
secExcFunc: 3
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ echo "Total operations:" $(grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/simple_test_checkAndInject.ll)
Total operations: 18
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ gcc -c -fPIC src/runtime/selective_codift_runtime.c -o selective_codift_runtime.o
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ gcc -shared selective_codift_runtime.o -o libselcodiftruntime.so
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 -S -emit-llvm src/code/selective_test.c -o llvmIR/selective_test.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[ 16%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 33%] Linking CXX shared module libCountFunc.so
[ 33%] Built target CountFunc
[ 50%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftCheckPass.cpp.o
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftInjectPass.cpp.o
[ 83%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftInjectPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ opt-15 -load ./CODIFTPass.so --help-list 2>&1 | grep -i "codift"
      --codift-check                                                       - CODIFT check pass
      --codift-inject                                                      - CODIFT inject pass
      --codift-inject-selective                                            - SELECTIVE CO-DIFT inject pass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject llvmIR/selective_test.ll -enable-new-pm=0 -S -o llvmIR/selective_test_checkAndInject.ll
[CODIFT] Found return instruction in function: get_untrusted_input
[CO-DIFT INJECT] Processing: get_untrusted_input
[CODIFT] Found return instruction in function: get_trusted_data
[CO-DIFT INJECT] Processing: get_trusted_data
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
[CO-DIFT INJECT] Processing: main
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Binary op: add
[INJECT] Store
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Binary op: add
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Alloca initialized
[INJECT] Store
[CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/selective_test_checkAndInject.ll
87
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject-selective llvmIR/selective_test.ll -enable-new-pm=0 -S -o llvmIR/selective_test_selectiveCheckAndInject.ll
[CODIFT] Found return instruction in function: get_untrusted_input
[SELECTIVE CO-DIFT INJECT] Processing: get_untrusted_input
[CODIFT] Found return instruction in function: get_trusted_data
[SELECTIVE CO-DIFT INJECT] Processing: get_trusted_data
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
[SELECTIVE CO-DIFT INJECT] Processing: main
  [SELECTIVE] Initializing alloca (security context):   %1 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %2 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %3 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %4 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %5 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %6 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Skipping alloca init (local temp):   %7 = alloca [5 x i32], align 16
  [SELECTIVE] Initializing alloca (security context):   %8 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %9 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Skipping store (clean data):   store i32 0, ptr %1, align 4
  [SELECTIVE] Skipping store (clean data):   store i32 10, ptr %2, align 4
  [SELECTIVE] Skipping store (clean data):   store i32 20, ptr %3, align 4
  [SELECTIVE] Tracking load:   %10 = load i32, ptr %2, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %11, ptr %codift_inject_temp, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking load:   %13 = load i32, ptr %3, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %14, ptr %codift_inject_temp3, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking (security context):   %16 = add nsw i32 %11, %14
  [SELECTIVE] Injected binary op: add
  [SELECTIVE] Skipping store (clean data):   store i32 %18, ptr %codift_inject_temp7, align 4
  [SELECTIVE] Skipping store (clean data):   store i32 %18, ptr %4, align 4
  [SELECTIVE] Tracking store (tainted data):   store i32 %20, ptr %5, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking load:   %22 = load i32, ptr %4, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %23, ptr %codift_inject_temp9, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking load:   %25 = load i32, ptr %5, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %26, ptr %codift_inject_temp11, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking (security context):   %28 = add nsw i32 %23, %26
  [SELECTIVE] Injected binary op: add
  [SELECTIVE] Skipping store (clean data):   store i32 %30, ptr %codift_inject_temp16, align 4
  [SELECTIVE] Skipping store (clean data):   store i32 %30, ptr %6, align 4
  [SELECTIVE] Tracking load:   %31 = load i32, ptr %6, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %32, ptr %codift_inject_temp17, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking store (tainted data):   store i32 %32, ptr %34, align 16
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking load:   %37 = load i32, ptr %36, align 16
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %38, ptr %codift_inject_temp20, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking store (tainted data):   store i32 %38, ptr %8, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking load:   %41 = load i32, ptr %8, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %42, ptr %codift_inject_temp23, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Initializing alloca (security context):   %codift_temp = alloca i1, align 1
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Skipping store (clean data):   store i1 %44, ptr %codift_temp, align 1
  [SELECTIVE] Skipping store (clean data):   store i32 %51, ptr %9, align 4
  [SELECTIVE] Tracking load:   %52 = load i32, ptr %9, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %53, ptr %codift_inject_temp25, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Initializing alloca (security context):   %codift_temp1 = alloca i1, align 1
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Skipping store (clean data):   store i1 %55, ptr %codift_temp1, align 1
[SELECTIVE CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/selective_test_selectiveCheckAndInject.ll
69
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ gcc src/code/manualDetecting_test.c -L. -lcodiftruntime -I. -o direct_test
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./direct_test
[CODIFT] Initialized 
(tag memory size: 262144 bytes)
=== Direct Security Test ===

TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7fff6073b018 -> tag= 0
  Tag: 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
[CODIFT] Tainting region: 0x7fff6073b01c (size: 4)
[CODIFT] Write Tag: address= 0x7fff6073b01c <- tag= 1
[CODIFT] Write Tag: address= 0x7fff6073b01d <- tag= 1
[CODIFT] Write Tag: address= 0x7fff6073b01e <- tag= 1
[CODIFT] Write Tag: address= 0x7fff6073b01f <- tag= 1
[CODIFT] Read Tag: address= 0x7fff6073b01c -> tag= 1
  Tag: 1

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 -S -emit-llvm src/code/manualDetecting_test.c -o llvmIR/manualDetecting_test.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject-selective -enable-new-pm=0 llvmIR/manualDetecting_test.ll -S -o llvmIR/manualDetecting_test_selectiveCheckAndInject.ll
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
[SELECTIVE CO-DIFT INJECT] Processing: main
  [SELECTIVE] Initializing alloca (security context):   %1 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %2 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %3 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %4 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Initializing alloca (security context):   %5 = alloca i32, align 4
  [SELECTIVE] Initialized alloca
  [SELECTIVE] Skipping store (clean data):   store i32 0, ptr %1, align 4
  [SELECTIVE] Skipping store (clean data):   store i32 42, ptr %2, align 4
  [SELECTIVE] Skipping store (clean data):   store i32 %12, ptr %3, align 4
  [SELECTIVE] Tracking load:   %13 = load i32, ptr %3, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %14, ptr %codift_inject_temp, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking load:   %18 = load i32, ptr %3, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %19, ptr %codift_inject_temp2, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Skipping store (clean data):   store i32 999, ptr %4, align 4
  [SELECTIVE] Skipping store (clean data):   store i32 %26, ptr %5, align 4
  [SELECTIVE] Tracking load:   %27 = load i32, ptr %5, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %28, ptr %codift_inject_temp4, align 4
  [SELECTIVE] Injected store
  [SELECTIVE] Tracking load:   %32 = load i32, ptr %5, align 4
  [SELECTIVE] Injected load
  [SELECTIVE] Tracking store (tainted data):   store i32 %33, ptr %codift_inject_temp6, align 4
  [SELECTIVE] Injected store
[SELECTIVE CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/manualDetecting_test_selectiveCheckAndInject.ll -L. -lcodiftruntime -o manual_test_protected
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./manual_test_protected
[CODIFT] Write Tag: address= 0x7ffc4c5e627c <- tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6278 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6274 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6270 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e626c <- tag= 0
[CODIFT] Read Tag: address= 0x7e0aa1ef1283 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Initialized
(tag memory size: 262144 bytes)
[CODIFT] Read Tag: address= 0x7e0aa1c60100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
=== Direct Security Test ===

[CODIFT] Read Tag: address= 0x7e0aa1c60100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7e0aa1ef1196 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7ffc4c5e6278 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffc4c5e6274 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6280 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffc4c5e6284 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6280 <- tag= 0
[CODIFT] Read Tag: address= 0x7e0aa1c60100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffc4c5e6274 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6288 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffc4c5e628c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6288 <- tag= 0
[CODIFT] Read Tag: address= 0x7e0aa1ef124b -> tag= 0

[CODIFT SECURITY CHECK PASSED]

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7e0aa1c60100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
[CODIFT] Read Tag: address= 0x7e0aa1ef12c0 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Tainting region: 0x7ffc4c5e6270 (size: 4)
[CODIFT] Write Tag: address= 0x7ffc4c5e6270 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffc4c5e6271 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffc4c5e6272 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffc4c5e6273 <- tag= 1
[CODIFT] Read Tag: address= 0x7e0aa1ef1196 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7ffc4c5e6270 -> tag= 1
[CODIFT] Read Tag: address= 0x7ffc4c5e626c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6290 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffc4c5e6294 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6290 <- tag= 0
[CODIFT] Read Tag: address= 0x7e0aa1c60100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
  Tag: 1
[CODIFT] Read Tag: address= 0x7ffc4c5e626c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6298 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffc4c5e629c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffc4c5e6298 <- tag= 0
[CODIFT] Read Tag: address= 0x7e0aa1ef124b -> tag= 0

[CODIFT SECURITY CHECK PASSED]

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[ 16%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 33%] Linking CXX shared module libCountFunc.so
[ 33%] Built target CountFunc
[ 50%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftCheckPass.cpp.o
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftInjectPass.cpp.o
[ 83%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftInjectPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 -S -emit-llvm src/code/automaticDetecting_test.c -o llvmIR/automaticDetecting_test.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject -enable-new-pm=0 llvmIR/automaticDetecting_test.ll -S -o llvmIR/automaticDetecting_test_injectAndCheck.ll
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
[CO-DIFT INJECT] Processing: main
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/automaticDetecting_test_injectAndCheck.ll -L. -lcodiftruntime -o automatic_test_normalProtection
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./automatic_test_normalProtection
[CODIFT] Write Tag: address= 0x7ffe865c3f24 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f20 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f1c <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f18 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f14 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f24 <- tag= 0
[CODIFT] Read Tag: address= 0x755f46b9d283 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Initialized
(tag memory size: 262144 bytes)
[CODIFT] Read Tag: address= 0x755f46860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
=== Direct Security Test ===

[CODIFT] Read Tag: address= 0x755f46860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
TEST 1: Clean data
[CODIFT] Write Tag: address= 0x7ffe865c3f20 <- tag= 0
[CODIFT] Read Tag: address= 0x755f46b9d196 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7ffe865c3f20 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f28 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f1c <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f1c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f2c <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f30 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f2c <- tag= 0
[CODIFT] Read Tag: address= 0x755f46860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffe865c3f1c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f34 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f38 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f34 <- tag= 0
[CODIFT] Read Tag: address= 0x755f46b9d24b -> tag= 0

[CODIFT SECURITY CHECK PASSED]

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x755f46860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
[CODIFT] Read Tag: address= 0x755f4685fe10 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
99999
[CODIFT] Read Tag: address= 0x755f46b9d196 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7ffe865c3f18 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f3c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f14 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f14 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f40 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f44 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f40 <- tag= 0
[CODIFT] Read Tag: address= 0x755f46860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffe865c3f14 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f48 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffe865c3f4c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffe865c3f48 <- tag= 0
[CODIFT] Read Tag: address= 0x755f46b9d24b -> tag= 0

[CODIFT SECURITY CHECK PASSED]

[CODIFT SECURITY CHECK PASSED]
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetecting_test_injectAndCheck.ll
56

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject-selective -enable-new-pm=0 llvmIR/automaticDetecting_test.ll -S -o llvmIR/automaticDetecting_test_selectiveInjectAndCheck.ll    
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
[SELECTIVE CO-DIFT INJECT] Processing: main
  [SELECTIVE] Tainted memory argument: @.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
  [SELECTIVE] Tainted memory argument:   %4 = alloca i32, align 4
[SELECTIVE CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/automaticDetecting_test_selectiveInjectAndCheck.ll -L. -lcodiftruntime -o automatic_test_protected
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./automatic_test_protected
[CODIFT] Write Tag: address= 0x7ffdbbff5d1c <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d18 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d14 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d10 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d0c <- tag= 0
[CODIFT] Read Tag: address= 0x7a1088a74283 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Initialized 
(tag memory size: 262144 bytes)
[CODIFT] Read Tag: address= 0x7a1088860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
=== Direct Security Test ===

[CODIFT] Read Tag: address= 0x7a1088860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7a1088a74196 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7ffdbbff5d18 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffdbbff5d14 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d20 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdbbff5d24 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d20 <- tag= 0
[CODIFT] Read Tag: address= 0x7a1088860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffdbbff5d14 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d28 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdbbff5d2c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d28 <- tag= 0
[CODIFT] Read Tag: address= 0x7a1088a7424b -> tag= 0

[CODIFT SECURITY CHECK PASSED]

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7a1088860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
[CODIFT] Read Tag: address= 0x7a108885fe10 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
99999
[CODIFT] Write Tag: address= 0x605ac472c059 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffdbbff5d10 <- tag= 1
[CODIFT] Read Tag: address= 0x7a1088a74196 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7ffdbbff5d10 -> tag= 1
[CODIFT] Read Tag: address= 0x7ffdbbff5d0c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d30 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdbbff5d34 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d30 <- tag= 0
[CODIFT] Read Tag: address= 0x7a1088860100 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
  Tag: 1
[CODIFT] Read Tag: address= 0x7ffdbbff5d0c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d38 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdbbff5d3c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdbbff5d38 <- tag= 0
[CODIFT] Read Tag: address= 0x7a1088a7424b -> tag= 0

[CODIFT SECURITY CHECK PASSED]

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetecting_test_selectiveInjectAndCheck.ll
52
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build 
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[ 14%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 28%] Linking CXX shared module libCountFunc.so
[ 28%] Built target CountFunc
[ 42%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftCheckPass.cpp.o
[ 57%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftInjectPass.cpp.o
[ 71%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftCheckPass.cpp.o
[ 85%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftInjectPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ opt-15 -load ./CODIFTPass.so --help-list 2>&1 | grep -i "codift"
      --codift-check                                                       - CODIFT check pass
      --codift-check-selective                                             - Selective CO-DIFT Check Pass
      --codift-inject                                                      - CODIFT inject pass
      --codift-inject-selective                                            - SELECTIVE CO-DIFT inject pass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 -S -emit-llvm src/code/automaticDetecting_test.c -o llvmIR/automaticDetecting_test.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check-selective -codift-inject-selective -enable-new-pm=0 llvmIR/automaticDetecting_test.ll -S -o llvmIR/automaticDetecting_test_selectiveInjectAndSelectiveCheck.ll
[SELECTIVE CO-DIFT INJECT] Processing: main
  [SELECTIVE] Tainted memory argument: @.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
  [SELECTIVE] Tainted memory argument:   %4 = alloca i32, align 4
[SELECTIVE CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/automaticDetecting_test_selectiveInjectAndSelectiveCheck.ll -L. -lcodiftruntime -o automatic_test_protected
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./automatic_test_protected
[CODIFT] Write Tag: address= 0x7ffce6482acc <- tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ac8 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ac4 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ac0 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482abc <- tag= 0
[CODIFT] Initialized
(tag memory size: 262144 bytes)
=== Direct Security Test ===

TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7ffce6482ac8 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffce6482ac4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ad0 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffce6482ad4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ad0 <- tag= 0
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffce6482ac4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ad8 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffce6482adc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ad8 <- tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
99999
[CODIFT] Write Tag: address= 0x5826d96c3059 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffce6482ac0 <- tag= 1
[CODIFT] Read Tag: address= 0x7ffce6482ac0 -> tag= 1
[CODIFT] Read Tag: address= 0x7ffce6482abc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ae0 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffce6482ae4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ae0 <- tag= 0
  Tag: 1
[CODIFT] Read Tag: address= 0x7ffce6482abc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ae8 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffce6482aec -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce6482ae8 <- tag= 0

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetecting_test_selectiveInjectAndSelectiveCheck.ll
30
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ find . -type f \( -name "*.cpp" -o -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make                                               t
[ 14%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 28%] Linking CXX shared module libCountFunc.so
[ 28%] Built target CountFunc                                                                                                        t
[ 42%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftCheckPass.cpp.o
[ 57%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftInjectPass.cpp.o
[ 71%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftCheckPass.cpp.o
[ 85%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftInjectPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check-selective -codift-inject-selective -enable-new-pm=0 llvmIR/automaticDetecting_test.ll -S -o llvmIR/automaticDetecting_test_selectiveInjectAndSelectiveCheck.ll
[SELECTIVE CHECK] Processing security function: main
[SELECTIVE CO-DIFT INJECT] Processing: main
  [SELECTIVE] Tainted memory argument: @.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
  [SELECTIVE] Tainted memory argument:   %4 = alloca i32, align 4
[SELECTIVE CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/automaticDetecting_test_selectiveInjectAndSelectiveCheck.ll -L. -lcodiftruntime -o automatic_test_protected
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./automatic_test_protected
[CODIFT] Write Tag: address= 0x7ffcfaa485ec <- tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485e8 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485e4 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485e0 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485dc <- tag= 0
[CODIFT] Initialized
(tag memory size: 262144 bytes)
=== Direct Security Test ===

TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7ffcfaa485e8 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffcfaa485e4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485f0 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcfaa485f4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485f0 <- tag= 0
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffcfaa485e4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485f8 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcfaa485fc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa485f8 <- tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
9999
[CODIFT] Write Tag: address= 0x5ff08d210059 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffcfaa485e0 <- tag= 1
[CODIFT] Read Tag: address= 0x7ffcfaa485e0 -> tag= 1
[CODIFT] Read Tag: address= 0x7ffcfaa485dc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa48600 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcfaa48604 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa48600 <- tag= 0
  Tag: 1
[CODIFT] Read Tag: address= 0x7ffcfaa485dc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa48608 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcfaa4860c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcfaa48608 <- tag= 0

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetecting_test_selectiveInjectAndSelectiveCheck.ll
30
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 -S -emit-llvm src/code/automaticDetecting_test.c -o llvmIR/automaticDetectingTest.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check-selective -enable-new-pm=0 llvmIR/automaticDetectingTest.ll -S -o llvmIR/automaticDetectingTest_selectiveCheck.ll
[SELECTIVE CHECK] Processing security function: main
[SELECTIVE CHECK] Injected check for:   %14 = load i32, ptr %4, align 4
[SELECTIVE CHECK] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make clean
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[ 11%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 22%] Linking CXX shared module libCountFunc.so
[ 22%] Built target CountFunc
[ 33%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftCheckPass.cpp.o
[ 44%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftInjectPass.cpp.o
[ 55%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftCheckPass.cpp.o
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftInjectPass.cpp.o
[ 77%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selectiveCodift_checkPass.cpp.o
[ 88%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selectiveCodift_injectPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ opt-15 -load ./CodiftPass.so --help-list 2>&1 | grep -i "codift"
      --check-selective                                                    - Selective CODIFT Check Pass
      --codift-check                                                       - CODIFT check pass
      --codift-check-selective                                             - Selective CO-DIFT Check Pass
      --codift-inject                                                      - CODIFT inject pass
      --codift-inject-selective                                            - SELECTIVE CO-DIFT inject pass
      --inject-selective                                                   - Selective CODIFT Inject Pass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -check-selective -inject-selective -enable-new-pm=0 llvmIR/automaticDetecting_test.ll -S -o llvmIR/automaticDetecting_testProtected.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/automaticDetecting_testProtected.ll -L. -lcodiftruntime -o automatic_test_protected
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./automatic_test_protected
[CODIFT] Read Tag: address= 0x7ffce150e3f0 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e3ec <- tag= 0
[CODIFT] Initialized
(tag memory size: 262144 bytes)
=== Direct Security Test ===

TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7ffce150e3f4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e3e8 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffce150e3e8 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffce150e3f8 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e3e4 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffce150e3e4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e3fc <- tag= 0
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffce150e3e4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e400 <- tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
99999
[CODIFT] Write Tag: address= 0x590c8c03d059 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffce150e3e0 <- tag= 1
[CODIFT] Read Tag: address= 0x7ffce150e3e0 -> tag= 1
[CODIFT] Read Tag: address= 0x7ffce150e404 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e3dc <- tag= 0
[CODIFT] Read Tag: address= 0x7ffce150e3dc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e408 <- tag= 0
  Tag: 1
[CODIFT] Read Tag: address= 0x7ffce150e3dc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffce150e40c <- tag= 0

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetecting_testProtected.ll
25
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -check-selective -inject-selective -enable-new-pm=0 llvmIR/automaticDetectingTest.ll -S -o llvmIR/automaticDetectingTestProtected.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/automaticDetectingTestProtected.ll -L. -lcodiftruntime -o automatic_test_protected
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./automatic_test_protected
[CODIFT] Read Tag: address= 0x7ffcf7563de8 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563de0 <- tag= 0
[CODIFT] Initialized
(tag memory size: 262144 bytes)
=== Direct Security Test ===

TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7ffcf7563dec -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563ddc <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563ddc -> tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563df0 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563dd8 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563dd8 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563df4 <- tag= 0
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffcf7563dd8 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563df8 <- tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
99999
[CODIFT] Write Tag: address= 0x56c92b732059 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffcf7563dd4 <- tag= 1
[CODIFT] Read Tag: address= 0x7ffcf7563dd4 -> tag= 1
[CODIFT] Write Tag: address= 0x7ffcf7563dfc <- tag= 1
[CODIFT] Read Tag: address= 0x7ffcf7563e00 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563de4 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563de4 -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Read Tag: address= 0x7ffcf7563d98 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563d94 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563d94 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563d9c <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563da0 -> tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563da4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563da8 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563dac -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563d94 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563dd4 -> tag= 1
[CODIFT] Read Tag: address= 0x7ffcf7563e04 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563dd0 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffcf7563dd0 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563e08 <- tag= 0
  Tag: 1
[CODIFT] Read Tag: address= 0x7ffcf7563dd0 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffcf7563e0c <- tag= 0

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetectingTestProtected.ll
40
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject -enable-new-pm=0 llvmIR/automaticDetectingTest.ll -S -o llvmIR/automaticDetectingTestNormalProtection.ll
[CODIFT] Found return instruction in function: access
[CO-DIFT INJECT] Processing: access
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Binary op: add
[INJECT] Store
[INJECT] Store
[CO-DIFT INJECT] Modified: access
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
[CO-DIFT INJECT] Processing: main
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetectingTestNormalprotection.ll
75
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check-selective -codift-inject-selective -enable-new-pm=0 llvmIR/automaticDetectingTest.ll -S -o llvmIR/automaticDetectingTest_protected.ll
[SELECTIVE CO-DIFT INJECT] Processing: access
[SELECTIVE CO-DIFT INJECT] Modified: access
[SELECTIVE CHECK] Processing security function: main
[SELECTIVE CHECK] Injected check for:   %14 = load i32, ptr %4, align 4
[SELECTIVE CHECK] Modified: main
[SELECTIVE CO-DIFT INJECT] Processing: main
  [SELECTIVE] Tainted memory argument: @.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
  [SELECTIVE] Tainted memory argument:   %4 = alloca i32, align 4
[SELECTIVE CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 llvmIR/automaticDetectingTest_protected.ll -L. -lcodiftruntime -o automatic_test_protected
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ LD_LIBRARY_PATH=. ./automatic_test_protected
[CODIFT] Write Tag: address= 0x7ffdb2d18610 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d1860c <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18608 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18604 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18600 <- tag= 0
[CODIFT] Initialized
(tag memory size: 262144 bytes)
=== Direct Security Test ===

TEST 1: Clean data
[CODIFT] Read Tag: address= 0x7ffdb2d1860c -> tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d18608 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18614 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d18618 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18614 <- tag= 0
  Tag: 0
[CODIFT] Read Tag: address= 0x7ffdb2d18608 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d1861c <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d18620 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d1861c <- tag= 0

[CODIFT SECURITY CHECK PASSED]

TEST 2: Tainted data
9999
[CODIFT] Write Tag: address= 0x6247d0ce2059 <- tag= 1
[CODIFT] Write Tag: address= 0x7ffdb2d18604 <- tag= 1
[CODIFT] Read Tag: address= 0x7ffdb2d18604 -> tag= 1
[CODIFT] Write Tag: address= 0x7ffdb2d18624 <- tag= 1
[CODIFT] Read Tag: address= 0x7ffdb2d18628 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18624 <- tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d185fc <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d1862c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d185fc <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d185fc -> tag= 0

[CODIFT SECURITY CHECK PASSED]
[CODIFT] Write Tag: address= 0x7ffdb2d185bc <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d185bc -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d185c0 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d185c4 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d185c0 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d185c8 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d185cc <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d18604 -> tag= 1
[CODIFT] Read Tag: address= 0x7ffdb2d18600 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18630 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d18634 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18630 <- tag= 0
  Tag: 1
[CODIFT] Read Tag: address= 0x7ffdb2d18600 -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18638 <- tag= 0
[CODIFT] Read Tag: address= 0x7ffdb2d1863c -> tag= 0
[CODIFT] Write Tag: address= 0x7ffdb2d18638 <- tag= 0

[CODIFT SECURITY EXCEPTION]
Tainted Data Detected
Potential control-flow attack prevented...
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/automaticDetectingTest_protected.ll
46
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ 

jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ mkdir build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ cd build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cmake -DLLVM_DIR=/urs/lib/llvm-15/cmake ../src
-- The C compiler identification is GNU 13.3.0
-- The CXX compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features       
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test HAVE_FFI_CALL
-- Performing Test HAVE_FFI_CALL - Success
-- Found FFI: /usr/lib/x86_64-linux-gnu/libffi.so  
-- Performing Test Terminfo_LINKABLE
-- Performing Test Terminfo_LINKABLE - Success
-- Found Terminfo: /usr/lib/x86_64-linux-gnu/libtinfo.so  
-- Found ZLIB: /usr/lib/x86_64-linux-gnu/libz.so (found version "1.3")  
-- Found zstd: /usr/lib/x86_64-linux-gnu/libzstd.so  
-- Found LibXml2: /usr/lib/x86_64-linux-gnu/libxml2.so (found version "2.9.14") 
-- Configuring done (21.4s)
-- Generating done (0.8s)
-- Build files have been written to: /mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ make
[  9%] Building CXX object CMakeFiles/CountFunc.dir/passes/countFunc.cpp.o
[ 18%] Linking CXX shared module libCountFunc.so
[ 18%] Built target CountFunc
[ 27%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftCheckPass.cpp.o
[ 36%] Building CXX object CMakeFiles/CodiftPass.dir/passes/codiftInjectPass.cpp.o
[ 45%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftCheckPass.cpp.o
[ 54%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selective_codiftInjectPass.cpp.o
[ 63%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selectiveCodift_checkPass.cpp.o
[ 72%] Building CXX object CMakeFiles/CodiftPass.dir/passes/selectiveCodift_injectPass.cpp.o
[ 81%] Building CXX object CMakeFiles/CodiftPass.dir/passes/checkPass.cpp.o
[ 90%] Building CXX object CMakeFiles/CodiftPass.dir/passes/injectPass.cpp.o
[100%] Linking CXX shared module CodiftPass.so
[100%] Built target CodiftPass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ opt-15 -load ./CodiftPass.so --help-list2>&1 | grep -i "CODIFT"
      --check                                                              - Selective CODIFT Check Pass
      --check-selective                                                    - Selective CODIFT Check Pass
      --codift-check                                                       - CODIFT check pass
      --codift-check-selective                                             - Selective CO-DIFT Check Pass
      --codift-inject                                                      - CODIFT inject pass
      --codift-inject-selective                                            - SELECTIVE CO-DIFT inject pass
      --inject                                                             - Selective CODIFT Inject Pass
      --inject-selective                                                   - Selective CODIFT Inject Pass
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT/build$ cd ..
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ mkdir llvmIR
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ clang-15 -S -emit-llvm src/code/automaticDetecting_test.c -o llvmIR/test.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -check -inject -enable-new-pm=0 llvmIR/test.ll -S -o llvmIR/testProtected.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/testProtected.ll
36
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check -codift-inject -enable-new-pm=0 llvmIR/test.ll -S -o llvmIR/testProtected.ll
[CODIFT] Found return instruction in function: access
[CO-DIFT INJECT] Processing: access
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Binary op: add
[INJECT] Store
[INJECT] Store
[CO-DIFT INJECT] Modified: access
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Injected security check before instruction in main
[CODIFT] Found return instruction in function: main
[CO-DIFT INJECT] Processing: main
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Alloca initialized
[INJECT] Store
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Store
[INJECT] Load
[INJECT] Store
[INJECT] Load
[INJECT] Store
[CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/testProtected.ll
75
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -check-selective -inject-selective -enable-new-pm=0 llvmIR/test.ll -S -o llvmIR/testProtected.ll
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/testProtected.ll
40
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ opt-15 -load ./build/CodiftPass.so -codift-check-selective -codift-inject-selective -enable-new-pm=0 llvmIR/test.ll -S -o llvmIR/testProtected.ll
[SELECTIVE CO-DIFT INJECT] Processing: access
[SELECTIVE CO-DIFT INJECT] Modified: access
[SELECTIVE CHECK] Processing security function: main
[SELECTIVE CHECK] Injected check for:   %14 = load i32, ptr %4, align 4
[SELECTIVE CHECK] Modified: main
[SELECTIVE CO-DIFT INJECT] Processing: main
  [SELECTIVE] Tainted memory argument: @.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
  [SELECTIVE] Tainted memory argument:   %4 = alloca i32, align 4
[SELECTIVE CO-DIFT INJECT] Modified: main
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$ grep -c "ramReadFunc\|ramWriteFunc\|secExcFunc" llvmIR/testProtected.ll
46
jeya1811@DESKTOP-C11QTJA:/mnt/c/Users/Lenovo/Desktop/github/project/CODIFT$

```