#include "process.h"
#include "../ModulesManager/modules.h"
#include "../PagingManager/paging.h"
#include "../PageAllocator/pageAllocator.h"
#include "trapStackFrame.h"
#include <naiveConsole.h>
#include <lib.h>

typedef struct {
	pcb_t pcb;
	uint8_t state;
} process_t;

static uint64_t current_pid=0;
static process_t processList[MAX_PROCESS];
static uint64_t kernelStack;
static uint8_t contextState = CTX_KERNEL_MODE;

static uint64_t stackPageToAddr(uint64_t stackPage);
static uint64_t setupProcessStack(pcb_t * process);

void lockProcess(uint64_t pid)
{
	processList[pid].state = PROC_STATE_LOCKED;
	schedule();
}

void unlockProcess(uint64_t pid)
{
	processList[pid].state = PROC_STATE_READY;
}

uint64_t getNewPID()
{
	uint64_t i;
	for(i=0; i<MAX_PROCESS; i++) {
		if(processList[i].state == PROC_STATE_UNASSIGNED)
			return i;
	}
}

pcb_t createProcess(uint8_t moduleid, uint64_t ppid)
{
	pcb_t newProc;

	newProc.pid = getNewPID();

	newProc.ppid = ppid;
	newProc.code_page = allocatePage();

	newProc.stack = stackPageToAddr((uint64_t)allocatePage());
	newProc.stack = setupProcessStack(&newProc);
	loadModule(moduleid, newProc.code_page);
	return newProc;
}

void scheduleProcess(pcb_t process) {
	processList[process.pid].state = PROC_STATE_READY;
	processList[process.pid].pcb = process;
}

void initializeScheduler() {
	int i;

	for(i=0; i<MAX_PROCESS; i++) {
		processList[i].state = PROC_STATE_UNASSIGNED;
	}
	current_pid=0;

}

void killProc(uint64_t pid)
{
	freePage(processList[pid].pcb.code_page);
	freePage(processList[pid].pcb.stack);
	processList[pid].state = PROC_STATE_UNASSIGNED;
	schedule();
	switchToProcess();
}

uint64_t currentProc()
{
	return current_pid;
}

//va al final de la pagina, el stack crece para arriba
uint64_t stackPageToAddr(uint64_t stackPage)
{
	return stackPage + pageSize() - 0x1;
}

uint64_t setupProcessStack(pcb_t * process)
{
	trapStackFrame_t * stack = (trapStackFrame_t *)process->stack - 1;

	stack->rip = getLogicalUserlandPage();
	stack->cs = 0x08;
	stack->rflags = 0x202;
	stack->rsp = (uint64_t)&(stack->base);
	stack->ss = 0x00;
	stack->base = 0x00;
	stack->r15 = 0x00;
	stack->r14 = 0x00;
	stack->r13 = 0x00;
	stack->r12 = 0x00;
	stack->r11 = 0x00;
	stack->r10 = 0x00;
	stack->r9 = 0x00;
	stack->r8 = 0x00;
	stack->rsi = 0x00;
	stack->rdi = 0x00;
	stack->rbp = 0x00;
	stack->rdx = 0x00;
	stack->rcx = 0x00;
	stack->rbx = 0x00;
	stack->rax = 0x00;

	return (uint64_t)stack;
}

void schedule()
{
	uint64_t i=current_pid;
	uint8_t found=0;

	while(!found) {
		i++;
		if(i==MAX_PROCESS)
			i=0;
		if(processList[i].state == PROC_STATE_READY)
			found=1;
	}
	current_pid=i;
	mapProcess(processList[current_pid].pcb.code_page);
}

uint64_t restoreProcessStack(uint64_t kstack)
{
	kernelStack = kstack;
	contextState=CTX_USER_MODE;
	return processList[current_pid].pcb.stack;
}

uint64_t currentlyInKernelMode()
{
		return contextState==CTX_KERNEL_MODE;
}

uint64_t restoreKernelStack(uint64_t stack)
{
	if(currentlyInKernelMode())
		return 0;
	processList[current_pid].pcb.stack = stack;
	contextState = CTX_KERNEL_MODE;
	return kernelStack;
}
