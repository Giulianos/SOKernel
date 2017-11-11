#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stdlib.h>

void * k_memset(void * destination, int32_t character, uint64_t length);
void * k_memcpy(void * destination, const void * source, uint64_t length);
void * k_malloc(size_t n);
void k_free(void * ptr);
void k_log(char *fmt, ...);
int k_strcmp(const char * s1, const char * s2);
int k_strncpy(char * destination, const char * source, size_t n);

char * cpuVendor(char *result);

#endif
