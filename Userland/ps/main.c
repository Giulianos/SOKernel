/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);
extern char * getModuleName(int i);

typedef struct {
	uint64_t pid;
	int tty;
	char module_number;
} process_info_t;

int main()
{
	process_info_t procs[10];
	int i = 0, quantity;
	init_printf(0, putc);

	//Obtenemos los procesos con la syscall ps (0x49)
	quantity = 0;//systemCall(0x49, (uint64_t)procs, 0, 0, 0, 0);

	printf("PID TTY  CMD\n");
	//Itermamos los procesos
	for(; i<quantity; i++) {
		printf("%3d tty%d %s\n", procs[i].pid, procs[i].tty, getModuleName(procs[i].module_number));
	}

	return 0;
}
