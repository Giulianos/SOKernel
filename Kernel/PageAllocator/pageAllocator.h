#ifndef PAGEALLOCATOR_H
#define PAGEALLOCATOR_H

#include <stdint.h>

#define CONCURRENT_ALLOCATED_PAGES 20
#define MAX_PHYSICAL_PAGES 0x4000 //asumimos como máximo 32 GB (2^14) de memoria RAM
#define PAGE_SIZE 2 //2 MB
#define USED_PAGE 1
#define LAST_KERNEL_PAGE 9


void * allocatePage();
void * allocateSinglePage();
unsigned long long pageSize();
void freePage(void * pageAddress);

#endif
