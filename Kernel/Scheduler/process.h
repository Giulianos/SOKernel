#ifndef PROCESS_H
#define PROCESS_H

	#include <stdint.h>
	#define MAX_PROCESS 100
	#define CTX_KERNEL_MODE 1
	#define CTX_USER_MODE 2

	typedef struct
	{
		uint64_t pid;
		uint64_t ppid;
		uint64_t code_page;
		uint64_t stack;
		uint64_t kstack;
	} pcb_t;

	uint64_t restoreProcessStack(uint64_t kstack);
	uint64_t restoreKernelStack(uint64_t stack);
	void schedule();
	void initializeScheduler();
	pcb_t createProcess(uint8_t moduleid, uint64_t ppid);
	void switchToProcess();
	void scheduleProcess(pcb_t process);
	void killProc(uint64_t pid);
	uint64_t currentProc();

#endif