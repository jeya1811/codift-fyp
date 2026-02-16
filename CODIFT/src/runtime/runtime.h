#ifndef CODIFT_RUNTIME_H
#define CODIFT_RUNTIME_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ================= Tag Definitions ================= */

#define TAG_CLEAN 0
#define TAG_TAINTED 1

/* ================= CODIFT Runtime API ================= */

/* Read tag associated with a memory address */
uint8_t ramReadFunc(uint8_t* addr);

/* Write tag to a memory address */
void ramWriteFunc(uint8_t* addr, uint8_t tag);

/* Security exception handler */
void secExcFunc(uint8_t tag);

/* Initialize CODIFT runtime */
void codift_init(void);

#ifdef __cplusplus
}
#endif

#endif /* CODIFT_RUNTIME_H */
