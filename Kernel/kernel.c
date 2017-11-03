#include <stdint.h>
#include <string.h>
#include <lib.h>
#include "interrupts/interrupts.h"
#include "PagingManager/paging.h"
#include "PageAllocator/pageAllocator.h"
#include "scheduler2/scheduler.h"
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
	process_t aux_process;
	init_scheduler();
	init_tty();
	k_log("Creating idle process...\n");
	aux_process = create_process(0, 0, 0, 0);
	add_scheduler(get_main_thread_process(aux_process));
	k_log("Creating init process...\n");
	aux_process = create_process(1, 0, 0, 0);
	add_scheduler(get_main_thread_process(aux_process));
	/*k_log("Creating shells...\n");
	for(;i<7; i++) {
		k_log("Creating sh for tty%d\n", i+1);
		aux_process = create_process(2, 0, i, 0);
		add_scheduler(get_main_thread_process(aux_process));
	}*/
	schedule_scheduler();
	configureInterrupts();
	switchToProcess();

	while(1){
	}

	return 0;
}
