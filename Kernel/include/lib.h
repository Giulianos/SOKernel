#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdlib.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
void sti();
void cli();
void reloadCR3();
void * kalloc(size_t n);
void free(void * ptr);

#endif
