#include "codift_runtime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAG_MEMORY_SIZE 65536

static uint32_t tag_memory[TAG_MEMORY_SIZE] = {0};

static inline uint32_t addr_to_index(void *addr) {
  uintptr_t ptr = (uintptr_t)addr;
  return (ptr >> 2) % TAG_MEMORY_SIZE;
}

uint32_t ramReadFunc(void *addr) {
  uint32_t idx = addr_to_index(addr);
  uint32_t tag = tag_memory[idx];

  printf("[CODIFT] Read Tag: address= %p -> tag= %u\n", addr, tag);
  return tag;
}

void ramWriteFunc(void *addr, uint32_t tag) {
  uint32_t idx = addr_to_index(addr);
  tag_memory[idx] = tag;

  printf("[CODIFT] Write Tag: address= %p <- tag= %u\n", addr, tag);
}

void secExcFunc(uint32_t tag) {
  if (tag != TAG_CLEAN) {
    printf("\n[CODIFT SECURITY EXCEPTION]\n"
           "Tainted Data Detected\n"
           "Potential control-flow attack prevented...\n");
  } else {
    printf("\n[CODIFT SECURITY CHECK PASSED]\n");
  }
}

void codift_init(void) {
  memset(tag_memory, 0, sizeof(tag_memory));
  printf("[CODIFT] Initialized \n(tag memory size: %zu bytes)\n",
         sizeof(tag_memory));
}

void codift_taint_region(void *start, size_t size) {
  printf("[CODIFT] Tainting region: %p (size: %zu)\n", start, size);

  for (size_t i = 0; i < size; i++) {
    void *addr = (void *)((uintptr_t)start + i);
    ramWriteFunc(addr, TAG_TAINTED);
  }
}

void codift_clean_region(void *start, size_t size) {
  printf("[CODIFT] Cleaning region: %p (size: %zu)\n", start, size);

  for (size_t i = 0; i < size; i++) {
    void *addr = (void *)((uintptr_t)start + i);
    ramWriteFunc(addr, TAG_CLEAN);
  }
}