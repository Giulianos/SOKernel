/* sampleCodeModule.c */
#include <stdint.h>
#include <stdlib.h>
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

int get_process_list(process_t * procs);

int main()
{
	process_t * procs;
	int i = 0, quantity;

	//Obtenemos la cantidad de procesos con la syscall ls_procs pasandole NULL como buffer
	quantity = get_process_list(NULL);
	procs = (process_t *)malloc(sizeof(process_t)*quantity);
	//Asumimos que la cantidad de procesos no cambio desde la llamada anterior
	get_process_list(procs);

	printf("PID PPID TTY   MEM\n");
	//Itermamos los procesos
	for(; i<quantity; i++) {
		printf("%3d  %3d tty%d %3dM\n", procs[i].pid, procs[i].ppid, procs[i].vt_id + 1, procs[i].allocated_memory);
	}

	return 0;
}

int get_process_list(process_t * procs)
{
	return (int)systemCall(0x49, (void *)procs, 0, 0, 0, 0);
}
