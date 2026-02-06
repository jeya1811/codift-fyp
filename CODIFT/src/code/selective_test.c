#include <stdio.h>

// Function to simulate untrusted input
int get_untrusted_input() {
  return 200; // Simulate network input
}

// Function with trusted data
int get_trusted_data() {
  return 42; // Internal calculation
}

int main() {
  // Case 1: Clean data (should NOT be tracked heavily)
  int clean1 = 10;                 // Constant - skip
  int clean2 = 20;                 // Constant - skip
  int clean_sum = clean1 + clean2; // Might skip

  // Case 2: Tainted data (SHOULD be tracked)
  int tainted = get_untrusted_input(); // Tainted!
  int mixed = clean_sum + tainted;     // Tainted + Clean = Tainted

  // Case 3: Store/Load with tainted data
  int array[5];
  array[0] = mixed;      // Store tainted
  int loaded = array[0]; // Load tainted

  // Security check point
  if (loaded > 100) { // Check should happen
    printf("Access with tainted data!\n");
  }

  // Case 4: Only clean data
  int trusted = get_trusted_data();
  if (trusted > 0) { // Check might be optimized
    printf("Clean access\n");
  }

  return 0;
}