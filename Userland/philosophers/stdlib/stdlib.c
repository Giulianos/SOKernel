#include <stdint.h>

extern uint64_t systemCall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t esi, uint64_t rdi);

void _exit()
{
  systemCall(0x01, 0, 0, 0, 0, 0);
}

int fork()
{
	return (int)systemCall(0x02, 0, 0, 0, 0, 0);
}
