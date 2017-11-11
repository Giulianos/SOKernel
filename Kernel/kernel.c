#include <stdint.h>
#include <string.h>
#include <lib.h>
#include "interrupts/interrupts.h"
#include <page_allocator.h>
#include "scheduler/scheduler.h"
#include "scheduler/context_switch.h"
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
	int i=2;
	process_t aux_process;
	init_scheduler();
	init_tty();
	#ifdef KERNEL_INIT_DEBUG_MSG
	k_log("Creating idle process...\n");
	#endif
	aux_process = create_process(0, 0, 0, 0);
	add_scheduler(get_main_thread_process(aux_process));
	#ifdef KERNEL_INIT_DEBUG_MSG
	k_log("Creating init process...\n");
	#endif
	aux_process = create_process(1, 0, 0, 0);
	add_scheduler(get_main_thread_process(aux_process));
	#ifdef KERNEL_INIT_DEBUG_MSG
	k_log("Creating shells...\n");
	#endif
	aux_process = create_process(5, 0, 0, 0);
	add_scheduler(get_main_thread_process(aux_process));
	aux_process = create_process(6, 0, 1, 0);
	add_scheduler(get_main_thread_process(aux_process));
	for(;i<7; i++) {
		#ifdef KERNEL_INIT_DEBUG_MSG
		k_log("Creating sh for tty%d\n", i+1);
		#endif
		aux_process = create_process(2, 0, i, 0);
		add_scheduler(get_main_thread_process(aux_process));
	}
	schedule_scheduler();
	configureInterrupts();
	switch_to_process();

	while(1){
	}

	return 0;
}
