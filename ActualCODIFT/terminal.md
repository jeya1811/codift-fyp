jeya1811@ubuntu:~/Github/project/ActualCODIFT$ tree
.
├── codift
│   ├── passes
│   │   ├── checkPass.cpp
│   │   ├── CMakeLists.txt
│   │   └── injectPass.cpp
│   ├── runtime.c
│   └── runtime.h
├── diagram.json
├── Makefile
├── src
│   └── door_lock.c
└── terminal.md

4 directories, 9 files
jeya1811@ubuntu:~/Github/project/ActualCODIFT$ make all && make stats
[PASSES] Configuring cmake...
mkdir -p build/passes_build
cmake -B build/passes_build -S codift/passes \
    -DLLVM_DIR=$(llvm-config-15 --cmakedir) \
    -DCMAKE_BUILD_TYPE=Release
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
-- Found LLVM 15.0.7
-- Configuring done (1.1s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jeya1811/Github/project/ActualCODIFT/build/passes_build
[PASSES] Compiling...
cmake --build build/passes_build --parallel
gmake[1]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[2]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[3]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[3]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[3]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/checkPass.cpp.o
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/injectPass.cpp.o
[100%] Linking CXX shared module libCodiftPass.so
gmake[3]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
[100%] Built target CodiftPass
gmake[2]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[1]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
cp build/passes_build/libCodiftPass.so build/CodiftPass.so
[PASSES] ✓ Built: build/CodiftPass.so
mkdir -p build/vuln
[VULN 1/3] Compiling door_lock.c...
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -I. -ffreestanding -fno-exceptions -c src/door_lock.c -o build/vuln/app.o
[VULN 2/3] Linking...
[VULN 3/3] ELF → HEX...
mkdir -p firmware
avr-gcc -mmcu=atmega328p build/vuln/app.o -o build/vuln/app.elf
avr-objcopy -O ihex -R .eeprom build/vuln/app.elf firmware/door_lock_vulnerable.hex
[VULN]   ✓ firmware/door_lock_vulnerable.hex
[VULN]     → Attack with 123456789# → door OPENS
mkdir -p build/prot
[PROT 1/4] door_lock.c → LLVM IR...
clang-15 --target=avr-unknown-unknown -mmcu=atmega328p -DF_CPU=16000000UL -Os -I. -ffreestanding -fno-exceptions -emit-llvm -S src/door_lock.c -o build/prot/app.ll
[PROT 1/4] ✓ build/prot/app.ll
[PROT 2/4] Running CO-DIFT passes...
opt-15 -load build/CodiftPass.so -inject -check -enable-new-pm=0 \
    -S -o build/prot/app_dift.ll build/prot/app.ll
[Inject] main
[Inject]  1 store(s) tagged TAINTED
[Check]  main
[Check]  found: s
[Check]  1 s.verified load(s) in main — injecting
[Check]  -> injected secExcFunc before s.verified load in main
[PROT 2/4] ✓ Instrumented IR: build/prot/app_dift.ll
[PROT 3/4] Instrumented IR → AVR object...
llc-15 -march=avr -mcpu=atmega328p -filetype=obj -o build/prot/app_dift.o build/prot/app_dift.ll
[PROT 3/4] ✓ build/prot/app_dift.o
[RUNTIME] Compiling runtime.c...
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -I. -ffreestanding -fno-exceptions -c codift/runtime.c -o build/runtime.o
[RUNTIME] ✓ build/runtime.o
avr-gcc -mmcu=atmega328p build/prot/app_dift.o build/runtime.o -o build/prot/app.elf
avr-objcopy -O ihex -R .eeprom build/prot/app.elf firmware/door_lock_protected.hex
[PROT]   ✓ firmware/door_lock_protected.hex
[PROT]     → Attack with 123456789# → BLOCKED

============================================================
  BUILD COMPLETE
  Vulnerable : firmware/door_lock_vulnerable.hex
  Protected  : firmware/door_lock_protected.hex
------------------------------------------------------------
  Wokwi: upload diagram.json then either HEX
  Attack input: 123456789#
  Vulnerable → door OPENS   Protected → BLOCKED
============================================================

══════════════════════════════════════════════════════════
  CO-DIFT Selective Instrumentation Overhead Report
══════════════════════════════════════════════════════════

  [1] LLVM IR Instruction Counts
      Instruction  Before  After Overhead
      -----------  ------  ----- --------
      call              0      2      new
      load              0      0      n/a
      store            41     41      +0%
      xor               0      0      n/a
      or                0      0      n/a
      and               0      0      n/a
      add               0      0      n/a
      br               44     44      +0%
      ret               3      3      +0%

      Total lines     405    411    ~1.5%

  [2] Binary Flash Usage
      Vulnerable (no DIFT):      1186 bytes
      Protected  (with DIFT):    1684 bytes  (+42.0% overhead)
      ATmega328P flash total:    32768 bytes

  [3] Comparison with CO-DIFT paper (IEEE CCNC 2025)
      Full CO-DIFT (instrument all)    : ~118% IR overhead
      Selective CO-DIFT (this project) : see Total lines above
      Zero application code changes    : programmer writes nothing
══════════════════════════════════════════════════════════

jeya1811@ubuntu:~/Github/project/ActualCODIFT$ 
