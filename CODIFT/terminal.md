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

```