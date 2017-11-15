#include <stdint.h>
#include "mxlib.h"

extern uint64_t systemCall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);

int mx_create(char * id_str)
{
  return (int)systemCall(0x120, (uint64_t)id_str, 0, 0, 0, 0);
}
int mx_lock(char * id_str)
{
  return (int)systemCall(0x121, (uint64_t)id_str, 0, 0, 0, 0);
}
int mx_unlock(char * id_str)
{
  return (int)systemCall(0x122, (uint64_t)id_str, 0, 0, 0, 0);
}
int mx_close(char * id_str)
{
  return (int)systemCall(0x123, (uint64_t)id_str, 0, 0, 0, 0);
}
