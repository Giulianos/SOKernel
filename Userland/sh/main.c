/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);

char parse_command(char * input);
uint64_t execve(int id);
void wait(uint64_t pid);

int main()
{
	char input[80];
	init_printf(0, putc);
	while(1) {
		printf(">");
		gets(input);
		if(!parse_command(input))
			printf("Comando no encontrado: %s\n", input);
	}
}

char parse_command(char * input)
{
	uint64_t child_pid;

	if(strcmp(input, "help") == 0) {
		child_pid = execve(3);
		wait(child_pid);
		return 1;
	}
	if(strcmp(input, "ps") == 0) {
		child_pid = execve(4);
		wait(child_pid);
		return 1;
	}
	if(strcmp(input, "help&") == 0) {
		child_pid = execve(3);
		return 1;
	}
	if(strcmp(input, "ps&") == 0) {
		child_pid = execve(4);
		return 1;
	}

	return 0;

}

void wait(uint64_t pid)
{
	systemCall(0x07, pid, 0, 0, 0, 0);
}

uint64_t execve(int id)
{
	return systemCall(0x0B, id, 0, 0, 0, 0);
}
