```bash
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

jeya1811@ubuntu:~/Github/project/ActualCODIFT$ make clean
[CLEAN] Removing build/ and firmware/...
rm -rf build firmware
[CLEAN] ✓ Done
jeya1811@ubuntu:~/Github/project/ActualCODIFT$ make passes
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
-- Configuring done (1.5s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jeya1811/Github/project/ActualCODIFT/build/passes_build
[PASSES] Compiling...
cmake --build build/passes_build --parallel
gmake[1]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[2]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[3]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[3]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[3]: Entering directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/injectPass.cpp.o
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/checkPass.cpp.o
[100%] Linking CXX shared module libCodiftPass.so
gmake[3]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
[100%] Built target CodiftPass
gmake[2]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
gmake[1]: Leaving directory '/home/jeya1811/Github/project/ActualCODIFT/build/passes_build'
cp build/passes_build/libCodiftPass.so build/CodiftPass.so
[PASSES] ✓ Built: build/CodiftPass.so
jeya1811@ubuntu:~/Github/project/ActualCODIFT$ make vulnerable
mkdir -p build/vuln
[VULN 1/3] Compiling door_lock.c...
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -I. -ffreestanding -fno-exceptions -c src/door_lock.c -o build/vuln/app.o
[VULN 2/3] Linking...
[VULN 3/3] ELF → HEX...
mkdir -p firmware
avr-gcc -mmcu=atmega328p build/vuln/app.o -o build/vuln/app.elf
avr-objcopy -O ihex -R .eeprom build/vuln/app.elf firmware/door_lock_vulnerable.hex
[VULN]   ✓ firmware/door_lock_vulnerable.hex
jeya1811@ubuntu:~/Github/project/ActualCODIFT$ make protected
mkdir -p build/prot
[PROT 1/4] door_lock.c → LLVM IR...
clang-15 --target=avr-unknown-unknown -mmcu=atmega328p -DF_CPU=16000000UL -Os -I. -ffreestanding -fno-exceptions -emit-llvm -S src/door_lock.c -o build/prot/app.ll

[PROT 1/4] ✓ build/prot/app.ll
[PROT 2/4] Running CO-DIFT passes...
opt-15 -load build/CodiftPass.so -inject -check -enable-new-pm=0 \
    -S -o build/prot/app_dift.ll build/prot/app.ll
[Inject] read_input
[Inject]  auto-injected codift_init() at entry
[Inject]  1 store(s) tagged TAINTED
[Check]  read_input
[Check]  found: s
[Inject] verify
[Check]  verify
[Check]  found: s
[Inject] main
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
jeya1811@ubuntu:~/Github/project/ActualCODIFT$ make stats

══════════════════════════════════════════════════════════
  CO-DIFT Selective Instrumentation Overhead Report
══════════════════════════════════════════════════════════

  [1] LLVM IR Instruction Counts
      Instruction  Before  After Overhead
      -----------  ------  ----- --------
      call              0      3      new
      load              0      0      n/a
      store            56     56      +0%
      xor               0      0      n/a
      or                0      0      n/a
      and               0      0      n/a
      add               0      0      n/a
      br               75     75      +0%
      ret              17     17      +0%

      Total lines     563    571    ~1.4% (IR overhead)

  [2] Binary Flash Usage
      Vulnerable (no DIFT)  :    1266 bytes
      Protected  (with DIFT):    2046 bytes  (+61.6% flash overhead)
      ATmega328P flash total:    32768 bytes

  [3] Comparison with CO-DIFT paper (IEEE CCNC 2025)
      Full CO-DIFT        : ~118% IR overhead
      Selective CO-DIFT   : see above IR overheaad
      source code change  : null

══════════════════════════════════════════════════════════

jeya1811@ubuntu:~/Github/project/ActualCODIFT$

jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make install-tools

============================================================
  CO-DIFT  –  Installing Dependencies
============================================================

[1/4] Updating apt package list...
sudo apt-get update -qq

[2/4] Installing LLVM / Clang 15 toolchain...
sudo apt-get install -y \
    clang-15 \
    llvm-15 \
    llvm-15-dev \
    llvm-15-tools \
    libclang-15-dev
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
clang-15 is already the newest version (1:15.0.7-14build3).
llvm-15 is already the newest version (1:15.0.7-14build3).
llvm-15-dev is already the newest version (1:15.0.7-14build3).
llvm-15-tools is already the newest version (1:15.0.7-14build3).
libclang-15-dev is already the newest version (1:15.0.7-14build3).
0 upgraded, 0 newly installed, 0 to remove and 21 not upgraded.
      ✓ LLVM / Clang 15 installed

[3/4] Installing AVR cross-compilation toolchain...
sudo apt-get install -y \
    gcc-avr \
    binutils-avr \
    avr-libc \
    avrdude
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
gcc-avr is already the newest version (1:7.3.0+Atmel3.7.0-1).
binutils-avr is already the newest version (2.26.20160125+Atmel3.7.0-2).
avr-libc is already the newest version (1:2.0.0+Atmel3.7.0-1).
avrdude is already the newest version (7.1+dfsg-3build2).
0 upgraded, 0 newly installed, 0 to remove and 21 not upgraded.
      ✓ AVR toolchain installed

[4/4] Installing build system tools...
sudo apt-get install -y \
    cmake \
    build-essential \
    wget
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
cmake is already the newest version (3.28.3-1build7).
build-essential is already the newest version (12.10ubuntu1).
wget is already the newest version (1.21.4-1ubuntu4.1).
0 upgraded, 0 newly installed, 0 to remove and 21 not upgraded.
      ✓ Build tools installed

============================================================
  All dependencies installed successfully!
============================================================

jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make verify-tools

  Verifying required tools:
      clang-15             ✓  Ubuntu clang version 15.0.7
      opt-15               ✓  Ubuntu LLVM version 15.0.7
      llc-15               ✓  Ubuntu LLVM version 15.0.7
      llvm-config-15       ✓  15.0.7
      avr-gcc              ✓  avr-gcc (GCC) 7.3.0
      avr-objcopy          ✓  GNU objcopy (GNU Binutils) 2.26.20160125
      avr-size             ✓  GNU size (GNU Binutils) 2.26.20160125
      avrdude              ✓  avrdude version 7.1, URL: <https://github.com/avrdudes/avrdude>
      cmake                ✓  cmake version 3.28.3

  ✓  All tools found.

jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make all
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
-- Configuring done (1.3s)
-- Generating done (0.0s)
-- Build files have been written to: /home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build
[PASSES] Compiling...
cmake --build build/passes_build --parallel
gmake[1]: Entering directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
gmake[2]: Entering directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
gmake[3]: Entering directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
gmake[3]: Leaving directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
gmake[3]: Entering directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/injectPass.cpp.o
[ 66%] Building CXX object CMakeFiles/CodiftPass.dir/checkPass.cpp.o
[100%] Linking CXX shared module libCodiftPass.so
gmake[3]: Leaving directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
[100%] Built target CodiftPass
gmake[2]: Leaving directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
gmake[1]: Leaving directory '/home/jeya1811/Github/codift-fyp/CODIFT/build/passes_build'
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
mkdir -p build/prot
[PROT 1/4] door_lock.c → LLVM IR...
clang-15 --target=avr-unknown-unknown -mmcu=atmega328p -DF_CPU=16000000UL -Os -I. -ffreestanding -fno-exceptions -emit-llvm -S src/door_lock.c -o build/prot/app.ll
[PROT 1/4] ✓ build/prot/app.ll
[PROT 2/4] Running CO-DIFT passes...
opt-15 -load build/CodiftPass.so -inject -check -enable-new-pm=0 \
    -S -o build/prot/app_dift.ll build/prot/app.ll
[Inject] read_input
[Inject]  auto-injected codift_init() at entry
[Inject]  1 store(s) tagged TAINTED
[Check]  read_input
[Check]  found: s
[Inject] verify
[Check]  verify
[Check]  found: s
[Inject] main
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

============================================================
  BUILD COMPLETE
  Vulnerable : firmware/door_lock_vulnerable.hex
  Protected  : firmware/door_lock_protected.hex
============================================================
jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make stats

══════════════════════════════════════════════════════════
  CO-DIFT Selective Instrumentation Overhead Report
══════════════════════════════════════════════════════════

  [1] LLVM IR Instruction Counts
      Instruction  Before  After Overhead
      -----------  ------  ----- --------
      call              0      3      new
      load              0      0      n/a
      store            56     56      +0%
      xor               0      0      n/a
      or                0      0      n/a
      and               0      0      n/a
      add               0      0      n/a
      br               75     75      +0%
      ret              17     17      +0%

      Total lines     563    571    ~1.4% (IR overhead)

  [2] Binary Flash Usage
      Vulnerable (no DIFT)  :    1266 bytes
      Protected  (with DIFT):    2046 bytes  (+61.6% flash overhead)
      ATmega328P flash total:    32768 bytes

  [3] Comparison with CO-DIFT paper (IEEE CCNC 2025)
      Full CO-DIFT        : ~118% IR overhead
      Selective CO-DIFT   : ~1.4% IR overhead
      source code change  : null

══════════════════════════════════════════════════════════

jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make flash-vulnerable PORT=/dev/ttyUSB0
avrdude -c arduino -p atmega328p -P /dev/ttyUSB0 -b 115200 -U flash:w:firmware/door_lock_vulnerable.hex:i

avrdude: AVR device initialized and ready to accept instructions
avrdude: device signature = 0x1e950f (probably m328p)
avrdude: Note: flash memory has been specified, an erase cycle will be performed.
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: reading input file firmware/door_lock_vulnerable.hex for flash
         with 1266 bytes in 1 section within [0, 0x4f1]
         using 10 pages and 14 pad bytes
avrdude: writing 1266 bytes flash ...

Writing | ################################################## | 100% 0.17 s

avrdude: 1266 bytes of flash written
avrdude: verifying flash memory against firmware/door_lock_vulnerable.hex

Reading | ################################################## | 100% 0.12 s

avrdude: 1266 bytes of flash verified

avrdude done.  Thank you.

jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make flash-protected PORT=/dev/ttyUSB0
avrdude -c arduino -p atmega328p -P /dev/ttyUSB0 -b 115200 -U flash:w:firmware/door_lock_protected.hex:i

avrdude: AVR device initialized and ready to accept instructions
avrdude: device signature = 0x1e950f (probably m328p)
avrdude: Note: flash memory has been specified, an erase cycle will be performed.
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: reading input file firmware/door_lock_protected.hex for flash
         with 2046 bytes in 1 section within [0, 0x7fd]
         using 16 pages and 2 pad bytes
avrdude: writing 2046 bytes flash ...

Writing | ################################################## | 100% 0.27 s

avrdude: 2046 bytes of flash written
avrdude: verifying flash memory against firmware/door_lock_protected.hex

Reading | ################################################## | 100% 0.20 s

avrdude: 2046 bytes of flash verified

avrdude done.  Thank you.

jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make clean
[CLEAN] Removing build/ and firmware/...
rm -rf build firmware
[CLEAN] ✓ Done
jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$ make remove-tools

============================================================
  CO-DIFT  –  Removing dependencies
============================================================

[1/4] Removing LLVM / Clang 15 toolchain...
sudo apt-get remove --purge -y \
    clang-15 \
    llvm-15 \
    llvm-15-dev \
    llvm-15-tools \
    libclang-15-dev
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following packages were automatically installed and are no longer required:
  lib32gcc-s1 lib32stdc++6 libc6-i386 libclang-common-15-dev libclang-cpp15t64 libclang-rt-15-dev libclang1-15t64 libgc1 libllvm15t64 libncurses-dev
  libobjc-13-dev libobjc4 libpfm4 libxml2-dev libz3-4 libz3-dev llvm-15-linker-tools llvm-15-runtime
Use 'sudo apt autoremove' to remove them.
The following packages will be REMOVED:
  clang-15* libclang-15-dev* llvm-15* llvm-15-dev* llvm-15-tools*
0 upgraded, 0 newly installed, 5 to remove and 21 not upgraded.
After this operation, 617 MB disk space will be freed.
(Reading database ... 212342 files and directories currently installed.)
Removing clang-15 (1:15.0.7-14build3) ...
Removing libclang-15-dev (1:15.0.7-14build3) ...
Removing llvm-15-dev (1:15.0.7-14build3) ...
Removing llvm-15 (1:15.0.7-14build3) ...
Removing llvm-15-tools (1:15.0.7-14build3) ...
Processing triggers for man-db (2.12.0-4build2) ...
Processing triggers for libc-bin (2.39-0ubuntu8.7) ...
      ✓ LLVM / Clang 15 removed

[2/4] Removing AVR cross-compilation toolchain...
sudo apt-get remove --purge -y \
    gcc-avr \
    binutils-avr \
    avr-libc \
    avrdude
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following packages were automatically installed and are no longer required:
  lib32gcc-s1 lib32stdc++6 libc6-i386 libclang-common-15-dev libclang-cpp15t64 libclang-rt-15-dev libclang1-15t64 libftdi1 libgc1 libhidapi-libusb0
  libllvm15t64 libncurses-dev libobjc-13-dev libobjc4 libpfm4 libusb-0.1-4 libxml2-dev libz3-4 libz3-dev llvm-15-linker-tools llvm-15-runtime
Use 'sudo apt autoremove' to remove them.
The following packages will be REMOVED:
  avr-libc* avrdude* binutils-avr* gcc-avr*
0 upgraded, 0 newly installed, 4 to remove and 21 not upgraded.
After this operation, 138 MB disk space will be freed.
(Reading database ... 208173 files and directories currently installed.)
Removing avr-libc (1:2.0.0+Atmel3.7.0-1) ...
Removing avrdude (7.1+dfsg-3build2) ...
Removing gcc-avr (1:7.3.0+Atmel3.7.0-1) ...
Removing binutils-avr (2.26.20160125+Atmel3.7.0-2) ...
Processing triggers for man-db (2.12.0-4build2) ...
(Reading database ... 204661 files and directories currently installed.)
Purging configuration files for avrdude (7.1+dfsg-3build2) ...
      ✓ AVR toolchain removed

[3/4] Removing build system tools...
sudo apt-get remove --purge -y \
    cmake \
    build-essential
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following packages were automatically installed and are no longer required:
  cmake-data g++ g++-13 g++-13-x86-64-linux-gnu g++-x86-64-linux-gnu lib32gcc-s1 lib32stdc++6 libc6-i386 libclang-common-15-dev libclang-cpp15t64
  libclang-rt-15-dev libclang1-15t64 libftdi1 libgc1 libhidapi-libusb0 libjsoncpp25 libllvm15t64 libncurses-dev libobjc-13-dev libobjc4 libpfm4
  librhash0 libstdc++-13-dev libusb-0.1-4 libxml2-dev libz3-4 libz3-dev llvm-15-linker-tools llvm-15-runtime
Use 'sudo apt autoremove' to remove them.
The following packages will be REMOVED:
  build-essential* cmake*
0 upgraded, 0 newly installed, 2 to remove and 21 not upgraded.
After this operation, 37.4 MB disk space will be freed.
(Reading database ... 204660 files and directories currently installed.)
Removing build-essential (12.10ubuntu1) ...
Removing cmake (3.28.3-1build7) ...
Processing triggers for man-db (2.12.0-4build2) ...
      ✓ Build tools removed

[4/4] Autoremove && Autoclean...
sudo apt-get autoremove --purge -y
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
The following packages will be REMOVED:
  cmake-data* g++* g++-13* g++-13-x86-64-linux-gnu* g++-x86-64-linux-gnu* lib32gcc-s1* lib32stdc++6* libc6-i386* libclang-common-15-dev*
  libclang-cpp15t64* libclang-rt-15-dev* libclang1-15t64* libftdi1* libgc1* libhidapi-libusb0* libjsoncpp25* libllvm15t64* libncurses-dev*
  libobjc-13-dev* libobjc4* libpfm4* librhash0* libstdc++-13-dev* libusb-0.1-4* libxml2-dev* libz3-4* libz3-dev* llvm-15-linker-tools*
  llvm-15-runtime*
0 upgraded, 0 newly installed, 29 to remove and 21 not upgraded.
After this operation, 385 MB disk space will be freed.
(Reading database ... 204638 files and directories currently installed.)
Removing cmake-data (3.28.3-1build7) ...
Removing g++ (4:13.2.0-7ubuntu1) ...
Removing g++-13 (13.3.0-6ubuntu2~24.04.1) ...
Removing g++-x86-64-linux-gnu (4:13.2.0-7ubuntu1) ...
Removing g++-13-x86-64-linux-gnu (13.3.0-6ubuntu2~24.04.1) ...
Removing libclang-rt-15-dev:amd64 (1:15.0.7-14build3) ...
Removing lib32stdc++6 (14.2.0-4ubuntu2~24.04.1) ...
Removing lib32gcc-s1 (14.2.0-4ubuntu2~24.04.1) ...
Removing libc6-i386 (2.39-0ubuntu8.7) ...
Removing libclang-common-15-dev (1:15.0.7-14build3) ...
Removing libclang-cpp15t64 (1:15.0.7-14build3) ...
Removing libclang1-15t64 (1:15.0.7-14build3) ...
Removing libftdi1:amd64 (0.20-4ubuntu3) ...
Removing libobjc-13-dev:amd64 (13.3.0-6ubuntu2~24.04.1) ...
Removing libobjc4:amd64 (14.2.0-4ubuntu2~24.04.1) ...
Removing libgc1:amd64 (1:8.2.6-1build1) ...
Removing libhidapi-libusb0:amd64 (0.14.0-1build1) ...
Removing libjsoncpp25:amd64 (1.9.5-6build1) ...
Removing llvm-15-runtime (1:15.0.7-14build3) ...
Removing llvm-15-linker-tools (1:15.0.7-14build3) ...
Removing libllvm15t64:amd64 (1:15.0.7-14build3) ...
Removing libncurses-dev:amd64 (6.4+20240113-1ubuntu2) ...
Removing libpfm4:amd64 (4.13.0+git32-g0d4ed0e-1) ...
Removing librhash0:amd64 (1.4.3-3build1) ...
Removing libstdc++-13-dev:amd64 (13.3.0-6ubuntu2~24.04.1) ...
Removing libusb-0.1-4:amd64 (2:0.1.12-35build1) ...
Removing libxml2-dev:amd64 (2.9.14+dfsg-1.3ubuntu3.7) ...
Removing libz3-dev:amd64 (4.8.12-3.1build1) ...
Removing libz3-4:amd64 (4.8.12-3.1build1) ...
Processing triggers for systemd (255.4-1ubuntu8.14) ...
Processing triggers for man-db (2.12.0-4build2) ...
Processing triggers for libc-bin (2.39-0ubuntu8.7) ...
(Reading database ... 199452 files and directories currently installed.)
Purging configuration files for libc6-i386 (2.39-0ubuntu8.7) ...
Purging configuration files for cmake-data (3.28.3-1build7) ...
sudo apt-get autoclean && sudo apt-get clean
Reading package lists... Done
Building dependency tree... Done
Reading state information... Done

============================================================
  All dependencies removed successfully!
============================================================

jeya1811@ubuntu:~/Github/codift-fyp/CODIFT$
```
