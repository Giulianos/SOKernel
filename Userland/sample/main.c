/* sampleCodeModule.c */
#include <stdint.h>
#include <stdlib.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

extern uint64_t systemCall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);

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

int main()
{
	char buf[255];
	int aux = 0;

	init_printf(0, putc);

	mq_open("chat");

	printf("Interprocess chat!\nRead(1) or Write(2)?: ");
	gets(buf);
	if(strcmp("2", buf)==0)
	{
		while(1) {
			printf("escriba su mensaje\n");
			gets(buf);
			mq_send("chat", buf, strlen(buf)+1);
			printf("sent %s\n", buf);
		}
	} else {
		while(1) {
			aux = mq_receive("chat", buf, 255);
			printf("received: %s\n", buf);
		}
	}

	return 0;
}
