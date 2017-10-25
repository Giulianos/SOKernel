#ifndef STDIO_H
#define STDIO_H
#include <stdint.h>
#include "../types.h"
int sscanf(char format[90],char str[90], ...);
int scanf(const char* format,...);
int abs(int a);
void intostr(int num,char*a);
void printf(const char* str,...);
void clearScreen();
char getchar();
void backspace() ;
void newLine();
void putchar(const char a);
char* readLine();
char* readInt(char* string, int* num);
int isNum(char c);
extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
#define NALLOC 1024  // Number of block sizes to allocate on call to sbrk
#define NULL 0
void *malloc(unsigned nbytes);
uint64_t getSO(int a);
typedef long Align;
union header {
  struct {
    union header *next;
    unsigned size;
  } s;

  Align x;
};
typedef union header Header;
static Header *morecore(unsigned nblocks);
void free(void *ptr);

#endif
