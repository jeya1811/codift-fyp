#include "runtime.h"

/* Arduino environment */
#include <Arduino.h>

/*
 * Arduino Uno constraints:
 *  - 2 KB SRAM
 *  - No MMU
 *  - No malloc / printf / exit
 */

#define TAG_MEMORY_SIZE 256

static uint8_t tag_memory[TAG_MEMORY_SIZE];

/* ================= Address → Tag Mapping ================= */

static inline uint8_t addr_to_index(uint8_t* addr) {
  uintptr_t a = (uintptr_t)addr;
  return (uint8_t)((a >> 2) & (TAG_MEMORY_SIZE - 1));
}

/* ================= CODIFT Runtime ================= */

uint8_t ramReadFunc(uint8_t* addr) {
  return tag_memory[addr_to_index(addr)];
}

void ramWriteFunc(uint8_t* addr, uint8_t tag) {
  tag_memory[addr_to_index(addr)] = tag;
}

void secExcFunc(uint8_t tag) {
  if (tag != TAG_CLEAN) {
    Serial.println("CODIFT SECURITY VIOLATION");
    digitalWrite(LED_BUILTIN, HIGH);

    /* Fail-secure: halt system */
    while (1) {
      /* infinite loop */
    }
  }
}

void codift_init(void) {
  /* Initialize tag memory */
  for (uint16_t i = 0; i < TAG_MEMORY_SIZE; i++) {
    tag_memory[i] = TAG_CLEAN;
  }

  /* Prepare hardware indicators */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

/* ================= Automatic Initialization =================
 * This runs BEFORE Arduino setup()
 * No application changes required
 */

__attribute__((constructor))
void codift_auto_init(void) {
  codift_init();
}
