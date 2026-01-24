#include <stdio.h>
#include <stdlib.h>
#include "../src/runtime/codift_runtime.h"

void process_input(int* input){
    if(*input> 100){
        printf("Processing large input: %d\n", *input);
    }
    else{
        printf("Processing normal input: %d\n", *input);
    }
}

int main(){
    printf("===CO-DIFT Runtime Test===\n\n");

    codift_init();

    int clean_data= 42;
    int tainted_data= 200;

    printf("1.Testing with clean data:\n"
            "Value: %d\n", clean_data);
    codift_clean_region(&clean_data, sizeof(int));
    uint32_t tag= ramReadFunc(&clean_data);
    printf("    Tag read back: %u\n", tag);
    secExcFunc(TAG_CLEAN);
    process_input(&clean_data);

    printf("\n2.Testing with Tainted data:\n"
            "Value: %d (simulating untrusted input)\n", tainted_data);
    codift_taint_region(&tainted_data, sizeof(int));
    tag= ramReadFunc(&tainted_data);
    printf("    Tag read back: %u\n", tag);
    printf("    Attempting to process tainted data...\n");
    secExcFunc(tag);
    process_input(&tainted_data);

    printf("\n3.Cleaning the tainted data:\n");
    codift_clean_region(&tainted_data, sizeof(int));
    tag= ramReadFunc(&tainted_data);
    printf("    Tag after cleaning: %u\n", tag);
    process_input(&tainted_data);
    printf("\n===Test Complete===\n");
    return 0;
}