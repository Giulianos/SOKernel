#include <stdint.h>
#include <stdlib.h>
#include "mqlib.h"

extern uint64_t systemCall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);

int mq_unlink(char * mq_name)
{
	return systemCall(0x116 ,(uint64_t)mq_name, 0, 0, 0, 0);
}
int mq_send(char * mq_name, char * msg, size_t len)
{
	return systemCall(0x117 ,(uint64_t)mq_name, (uint64_t)msg, (uint64_t)len, 0, 0);
}
int mq_receive(char * mq_name, char * msg, size_t len)
{
	return systemCall(0x118 ,(uint64_t)mq_name, (uint64_t)msg, (uint64_t)len, 0, 0);
}
int mq_open(char * mq_name)
{
	return systemCall(0x115 ,(uint64_t)mq_name, 0, 0, 0, 0);
}
