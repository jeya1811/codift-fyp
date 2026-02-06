// direct_test.c
#include "../runtime/codift_runtime.h"
#include <stdio.h>

int main() {
  codift_init();

  printf("=== Direct Security Test ===\n\n");

  // Test 1: Clean data
  printf("TEST 1: Clean data\n");
  int clean = 42;
  uint32_t clean_tag = ramReadFunc(&clean);
  printf("  Tag: %u\n", clean_tag);
  secExcFunc(clean_tag); // Should pass

  printf("\nTEST 2: Tainted data\n");
  int tainted = 999;
  codift_taint_region(&tainted, sizeof(int));
  uint32_t tainted_tag = ramReadFunc(&tainted);
  printf("  Tag: %u\n", tainted_tag);
  secExcFunc(tainted_tag); // Should FAIL with security exception!

  return 0;
}