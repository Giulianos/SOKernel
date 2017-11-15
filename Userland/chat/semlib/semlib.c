#include <stdint.h>
#include "semlib.h"

extern uint64_t systemCall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);

int sem_create(char * id_str, int initial_value)
{
  return (int)systemCall(0x130, (uint64_t)id_str, (uint64_t)initial_value, 0, 0, 0);
}
int sem_wait(char * id_str)
{
  return (int)systemCall(0x131, (uint64_t)id_str, 0, 0, 0, 0);
}
int sem_signal(char * id_str)
{
  return (int)systemCall(0x132, (uint64_t)id_str, 0, 0, 0, 0);
}
