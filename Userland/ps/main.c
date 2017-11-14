/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);
extern char * getModuleName(int i);

typedef struct
{
	int pid;
	int ppid;
	int vt_id;
	int allocated_memory;
} process_t;

int main()
{
	process_t procs[100];
	int i = 0, quantity;
	init_printf(0, putc);

	//Obtenemos los procesos con la syscall ps (0x49)
	quantity = systemCall(0x49, (uint64_t)procs, 0, 0, 0, 0);

	printf("PID PPID TTY  MEM\n");
	//Itermamos los procesos
	for(; i<quantity; i++) {
		printf("%3d  %3d tty%d %3d\n", procs[i].pid, procs[i].ppid, procs[i].vt_id, procs[i].allocated_memory);
	}

	return 0;
}
