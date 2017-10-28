#ifndef PROCESS_H
#define PROCESS_H

	#include <stdint.h>
	#define MAX_PROCESS 100
	#define CTX_KERNEL_MODE 1
	#define CTX_USER_MODE 2
	#define PROC_STATE_READY 0
	#define PROC_STATE_LOCKED 1
	#define PROC_STATE_UNASSIGNED 2


	typedef struct {
		uint64_t pid;
		uint64_t ppid;
		uint64_t code_page;
		uint64_t stack;
		uint64_t kstack;
		char module_number;
		int vt_id;
	} pcb_t;

	typedef struct {
		uint64_t pid;
		int tty;
		char module_number;
	} process_info_t;

	uint64_t restoreProcessStack(uint64_t kstack);
	uint64_t restoreKernelStack(uint64_t stack);
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


#endif
