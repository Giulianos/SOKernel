#include <stdint.h>
#include <string.h>
#include <lib.h>
#include "interrupts/interrupts.h"
#include "PagingManager/paging.h"
#include "PageAllocator/pageAllocator.h"
#include "Scheduler/process.h"
#include "ModulesManager/modules.h"
#include "tty/tty.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	k_memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void *)((char *)allocatePage() + pageSize() - sizeof(uint64_t));
}

void * initializeKernelBinary()
{
	loadModulesToKernel();
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main()
{
	int i=0;
	pcb_t aux_proc;
	initializeScheduler();
	init_tty();
	k_log("Creating idle process...\n");
	scheduleProcess(createProcess(0, 0, 0));
	k_log("Creating shells...\n");
	for(;i<7; i++) {
		aux_proc = createProcess(2, 0, i);
		k_log(" sh for tty%d has pid %d\n", i+1, aux_proc.pid);
		scheduleProcess(aux_proc);
	}
	schedule();
	configureInterrupts();
	switchToProcess();

	while(1){
	}

	return 0;
}
