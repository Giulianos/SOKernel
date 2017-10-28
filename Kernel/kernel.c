#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include "PagingManager/paging.h"
#include "Scheduler/process.h"
#include "ModulesManager/modules.h"
#include "tty/tty.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

extern uint64_t systemCall(uint64_t rax,uint64_t rbx,uint64_t rcx,uint64_t rdx,uint64_t rsi,uint64_t rdi);

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
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
