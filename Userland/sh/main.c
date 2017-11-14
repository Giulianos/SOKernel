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
void set_fg(int pid);
int run_app(int number, int fg);


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

	if(strcmp(input, "help") == 0) {
		return run_app(3, 1);
	}
	if(strcmp(input, "help&") == 0) {
		return run_app(3, 0);
	}
	if(strcmp(input, "producer") == 0) {
		return run_app(5, 1);
	}
	if(strcmp(input, "producer&") == 0) {
		return run_app(5, 0);
	}
	if(strcmp(input, "consumer") == 0) {
		return run_app(6, 1);
	}
	if(strcmp(input, "consumer&") == 0) {
		return run_app(6, 0);
	}
	if(strcmp(input, "ps") == 0) {
		return run_app(4, 1);
	}
	if(strcmp(input, "ps&") == 0) {
		return run_app(4, 0);
	}
	if(strcmp(input, "sample") == 0) {
		return run_app(7, 1);
	}
	if(strcmp(input, "sample&") == 0) {
		return run_app(7, 0);
	}

	return 0;
}

int run_app(int number, int fg)
{
	int child_pid = fork();
	if(child_pid == 0)
	{
		execve(number);
		exit();
	} else {
		if(fg) {
			set_fg(child_pid);
			wait(child_pid);
			set_fg(-1);
		}
	}
	return 1;
}

void set_fg(int pid)
{
	systemCall(0x150, (uint64_t)pid, 0, 0, 0, 0);
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
