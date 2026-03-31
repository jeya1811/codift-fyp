CLANG   := clang-15
OPT     := opt-15
LLC     := llc-15
GCC     := avr-gcc
OBJCOPY := avr-objcopy
CMAKE   := cmake

PORT    ?= /dev/ttyACM0
BAUD    := 115200
PROG    := arduino

MCU     := atmega328p
F_CPU   := 16000000UL
SRC     := src/door_lock.c

BD      := build
FW      := firmware
BP      := $(BD)/passes_build
PASS_SO := $(BD)/CodiftPass.so

GCC_FLAGS   := -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -I. \
               -ffreestanding -fno-exceptions
CLANG_FLAGS := --target=avr-unknown-unknown \
               -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -I. \
               -ffreestanding -fno-exceptions \
               -emit-llvm -S

.PHONY: all passes vulnerable protected stats flash-vulnerable flash-protected clean

all: passes vulnerable protected
	@echo ""
	@echo "============================================================"
	@echo "  BUILD COMPLETE"
	@echo "  Vulnerable : $(FW)/door_lock_vulnerable.hex"
	@echo "  Protected  : $(FW)/door_lock_protected.hex"
	@echo "============================================================"

passes: $(PASS_SO)
$(PASS_SO): codift/passes/injectPass.cpp codift/passes/checkPass.cpp \
            codift/passes/CMakeLists.txt
	@echo "[PASSES] Configuring cmake..."
	mkdir -p $(BP)
	$(CMAKE) -B $(BP) -S codift/passes \
	    -DLLVM_DIR=$$(llvm-config-15 --cmakedir) \
	    -DCMAKE_BUILD_TYPE=Release
	@echo "[PASSES] Compiling..."
	$(CMAKE) --build $(BP) --parallel
	cp $(BP)/libCodiftPass.so $(PASS_SO)
	@echo "[PASSES] ✓ Built: $(PASS_SO)"

$(BD)/runtime.o: codift/runtime.c | $(BD)
	@echo "[RUNTIME] Compiling runtime.c..."
	$(GCC) $(GCC_FLAGS) -c $< -o $@
	@echo "[RUNTIME] ✓ $@"

vulnerable: $(FW)/door_lock_vulnerable.hex
	@echo "[VULN]   ✓ $(FW)/door_lock_vulnerable.hex"

$(FW)/door_lock_vulnerable.hex: $(BD)/vuln/app.o | $(FW)
	$(GCC) -mmcu=$(MCU) $< -o $(BD)/vuln/app.elf
	$(OBJCOPY) -O ihex -R .eeprom $(BD)/vuln/app.elf $@

$(BD)/vuln/app.o: $(SRC) | $(BD)/vuln
	@echo "[VULN 1/3] Compiling door_lock.c..."
	$(GCC) $(GCC_FLAGS) -c $< -o $@
	@echo "[VULN 2/3] Linking..."
	@echo "[VULN 3/3] ELF → HEX..."

$(BD)/vuln:
	mkdir -p $@

protected: $(FW)/door_lock_protected.hex
	@echo "[PROT]   ✓ $(FW)/door_lock_protected.hex"

$(FW)/door_lock_protected.hex: $(BD)/prot/app_dift.o $(BD)/runtime.o | $(FW)
	$(GCC) -mmcu=$(MCU) $^ -o $(BD)/prot/app.elf
	$(OBJCOPY) -O ihex -R .eeprom $(BD)/prot/app.elf $@

$(BD)/prot/app_dift.o: $(BD)/prot/app_dift.ll | $(BD)/prot
	@echo "[PROT 3/4] Instrumented IR → AVR object..."
	$(LLC) -march=avr -mcpu=$(MCU) -filetype=obj -o $@ $<
	@echo "[PROT 3/4] ✓ $@"

$(BD)/prot/app_dift.ll: $(BD)/prot/app.ll $(PASS_SO)
	@echo "[PROT 2/4] Running CO-DIFT passes..."
	$(OPT) -load $(PASS_SO) -inject -check -enable-new-pm=0 \
	    -S -o $@ $<
	@echo "[PROT 2/4] ✓ Instrumented IR: $@"

$(BD)/prot/app.ll: $(SRC) | $(BD)/prot
	@echo "[PROT 1/4] door_lock.c → LLVM IR..."
	$(CLANG) $(CLANG_FLAGS) $< -o $@
	@echo "[PROT 1/4] ✓ $@"

$(BD)/prot:
	mkdir -p $@

$(BD):
	mkdir -p $@

$(FW):
	mkdir -p $@

stats: $(BD)/prot/app.ll $(BD)/prot/app_dift.ll \
       $(FW)/door_lock_vulnerable.hex $(FW)/door_lock_protected.hex
	@echo ""
	@echo "══════════════════════════════════════════════════════════"
	@echo "  CO-DIFT Selective Instrumentation Overhead Report"
	@echo "══════════════════════════════════════════════════════════"
	@echo ""
	@echo "  [1] LLVM IR Instruction Counts"
	@printf "      %-12s %6s %6s %8s\n" "Instruction" "Before" "After" "Overhead"
	@printf "      %-12s %6s %6s %8s\n" "-----------" "------" "-----" "--------"
	@for op in call load store xor or and add br ret; do \
	    before=$$(grep -c "  $$op " $(BD)/prot/app.ll 2>/dev/null || true); \
	    after=$$(grep -c "  $$op " $(BD)/prot/app_dift.ll 2>/dev/null || true); \
	    before=$${before:-0}; after=$${after:-0}; \
	    if [ "$$before" -eq 0 ] && [ "$$after" -gt 0 ]; then \
	        overhead="new"; \
	    elif [ "$$before" -eq 0 ]; then \
	        overhead="n/a"; \
	    else \
	        overhead=$$(awk "BEGIN{printf \"+%.0f%%\", (($$after-$$before)/$$before)*100}"); \
	    fi; \
	    printf "      %-12s %6d %6d %8s\n" "$$op" "$$before" "$$after" "$$overhead"; \
	done
	@before_total=$$(grep -c "." $(BD)/prot/app.ll 2>/dev/null || echo 0); \
	 after_total=$$(grep -c "." $(BD)/prot/app_dift.ll 2>/dev/null || echo 0); \
	 overhead=$$(awk "BEGIN{printf \"~%.1f%%\", (($$after_total-$$before_total)/$$before_total)*100}"); \
	 printf "\n      %-12s %6d %6d %8s (IR overhead)\n" "Total lines" "$$before_total" "$$after_total" "$$overhead"
	@echo ""
	@echo "  [2] Binary Flash Usage"
	@vuln=$$(avr-size $(BD)/vuln/app.elf 2>/dev/null | awk 'NR==2{print $$1+$$2}'); \
	 prot=$$(avr-size $(BD)/prot/app.elf 2>/dev/null | awk 'NR==2{print $$1+$$2}'); \
	 overhead=$$(awk "BEGIN{printf \"+%.1f%%\", (($$prot-$$vuln)/$$vuln)*100}"); \
	 printf "      Vulnerable (no DIFT)  :%8d bytes\n" "$$vuln"; \
	 printf "      Protected  (with DIFT):%8d bytes  (%s flash overhead)\n" "$$prot" "$$overhead"; \
	 echo "      ATmega328P flash total:    32768 bytes"
	@echo ""
	@echo "  [3] Comparison with CO-DIFT paper (IEEE CCNC 2025)"
	@echo "      Full CO-DIFT        : ~118% IR overhead"
	@before=$$(grep -c "." $(BD)/prot/app.ll 2>/dev/null || echo 0); \
	 after=$$(grep -c "." $(BD)/prot/app_dift.ll 2>/dev/null || echo 0); \
	 overhead=$$(awk "BEGIN{printf \"~%.1f%%\", (($$after-$$before)/$$before)*100}"); \
	 printf "      Selective CO-DIFT   : %s IR overhead\n" "$$overhead"
	@echo "      source code change  : null"
	@echo ""
	@echo "══════════════════════════════════════════════════════════"
	@echo ""

flash-vulnerable: $(FW)/door_lock_vulnerable.hex
	avrdude -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<:i

flash-protected: $(FW)/door_lock_protected.hex
	avrdude -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<:i

clean:
	@echo "[CLEAN] Removing build/ and firmware/..."
	rm -rf $(BD) $(FW)
	@echo "[CLEAN] ✓ Done"
