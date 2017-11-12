/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);

char parse_command(char * input);
int execve(int id);
int fork();
void wait(int pid);
void exit();

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
	int child_pid;

	if(strcmp(input, "help") == 0) {
		child_pid = fork();
		if(child_pid == 0)
		{
			printf("Aca va la informacion de ayuda\n");
			exit();
		} else {
			wait(child_pid);
		}
		return 1;
	}

	return 0;
}

void wait(int pid)
{
	systemCall(0x07, (uint64_t)pid, 0, 0, 0, 0);
}

void exit()
{
	systemCall(0x01, 0, 0, 0, 0, 0);
}

int fork()
{
	return (int)systemCall(0x02, 0, 0, 0, 0, 0);
}

int execve(int id)
{
	return (int)systemCall(0x0B, id, 0, 0, 0, 0);
}
