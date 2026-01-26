#pragma once
#include <stddef.h>
#include <stdint.h>

#define TAG_CLEAN 0
#define TAG_TAINTED 1
#define TAG_SENSITIVE 2

uint32_t ramReadFunc(void* addr);

void ramWriteFunc(void* addr, uint32_t tag);

void secExcFunc(uint32_t tag);

void codift_init(void);

void codift_taint_region(void* start, size_t size);

void codift_clean_region(void* start, size_t size);