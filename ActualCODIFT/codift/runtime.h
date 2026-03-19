#ifndef CODIFT_RUNTIME_H
#define CODIFT_RUNTIME_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define TAG_CLEAN 0
#define TAG_TAINTED 1

extern volatile uint8_t codift_violation;

void codift_init(void);
uint8_t ramReadFunc(uint8_t* addr);
void ramWriteFunc(uint8_t* addr, uint8_t tag);

void secExcFunc(uint8_t tag, uint8_t* addr);

#ifdef __cplusplus
}
#endif
#endif
