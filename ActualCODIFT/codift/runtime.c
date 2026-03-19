#define F_CPU 16000000UL
#include "runtime.h"
#include <avr/io.h>
#include <util/delay.h>

#define TAG_SIZE 256
#define SRAM_BASE 0x0100

static uint8_t tag_mem[TAG_SIZE];
volatile uint8_t codift_violation = 0;

static uint8_t addr_idx(uint8_t* a) {
  uintptr_t offset = (uintptr_t)a - SRAM_BASE;
  return (uint8_t)(offset & (TAG_SIZE - 1));
}

uint8_t ramReadFunc(uint8_t* a) { return tag_mem[addr_idx(a)]; }
void ramWriteFunc(uint8_t* a, uint8_t t) { tag_mem[addr_idx(a)] = t; }

void secExcFunc(uint8_t tag, uint8_t* addr) {
  if (tag != TAG_CLEAN) {
    codift_violation = 1;

    DDRB |= (1 << PB5);
    for (uint8_t i = 0; i < 5; i++) {
      PORTB |= (1 << PB5);
      _delay_ms(500);
      PORTB &= ~(1 << PB5);
      _delay_ms(500);
    }

    PORTB &= ~(1 << PB5);
    *addr = 0;
    codift_init();
  }
}

void codift_init(void) {
  for (uint16_t i = 0; i < TAG_SIZE; i++)
    tag_mem[i] = TAG_CLEAN;
  codift_violation = 0;
  DDRB |= (1 << PB5);
  PORTB &= ~(1 << PB5);
}

__attribute__((constructor)) static void _auto_init(void) { codift_init(); }
