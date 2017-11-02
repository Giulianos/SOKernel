#ifndef PROCESS_H
#define PROCESS_H

	#include <stdint.h>

	#define MAX_PROCESS 100
	#define MAX_ALLOCATED_PAGES 20

	#define PROCESS_HEAP_BASE 0x20000000

	#define CTX_KERNEL_MODE 1
	#define CTX_USER_MODE 2

	#define PROC_STATE_READY 0
	#define PROC_STATE_LOCKED 1
	#define PROC_STATE_UNASSIGNED 2


	typedef struct {
		uint64_t pid;
		uint64_t ppid;
		void * code_page;
		void * stack;
		void * kstack;
		void * allocated_pages[MAX_ALLOCATED_PAGES];
		int allocated_pages_quantity;
		char module_number;
		int vt_id;
	} pcb_t;

	typedef struct {
		uint64_t pid;
		int tty;
		char module_number;
	} process_info_t;

	void * restoreProcessStack(void * kstack);
	void * restoreKernelStack(void * stack);
	void schedule();
	void initializeScheduler();
	pcb_t createProcess(uint8_t moduleid, uint64_t ppid, int vt_id);
	void switchToProcess();
	void scheduleProcess(pcb_t process);
	void killProc(uint64_t pid);
	uint64_t currentProc();
	void lockProcess(uint64_t pid);
	void unlockProcess(uint64_t pid);
	int getProcessVT(uint64_t pid);
	void waitProcess(uint64_t waiter, uint64_t pid);
	int listProcs(process_info_t * procs);
	void * assignAllocatedPage(uint64_t pid, void * page);


#endif
