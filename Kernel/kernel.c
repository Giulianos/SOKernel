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
	memset(bssAddress, 0, bssSize);
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

	initializeScheduler();
	init_tty();
	//idle
	scheduleProcess(createProcess(0, 0, 0));
	//shells
	scheduleProcess(createProcess(2, 0, 0));
	scheduleProcess(createProcess(2, 0, 1));
	scheduleProcess(createProcess(2, 0, 2));
	scheduleProcess(createProcess(2, 0, 3));
	scheduleProcess(createProcess(2, 0, 4));
	scheduleProcess(createProcess(2, 0, 5));
	scheduleProcess(createProcess(2, 0, 6));
	schedule();
	configureInterrupts();
	switchToProcess();

	while(1){
	}

	return 0;
}
