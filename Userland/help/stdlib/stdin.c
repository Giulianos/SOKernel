#include "stdio.h"
#include <stdint.h>

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);

char * gets(char * s)
{
  systemCall(0x03, 0x01, s, 0, 0, 0);
  while(*s!='\n') { s++; }
  *s = '\0';
  return s;
}
