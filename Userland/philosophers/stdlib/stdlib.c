#include <stdint.h>

extern uint64_t systemCall(rax, rbx, rcx, rdx, esi, rdi);

void _exit()
{
  systemCall(0x01, 0, 0, 0, 0, 0);
}

int fork()
{
	return (int)systemCall(0x02, 0, 0, 0, 0, 0);
}
