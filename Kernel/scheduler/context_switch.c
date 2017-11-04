#include "thread.h"
#include "scheduler.h"

#define CONTEXT_PROCESS 1
#define CONTEXT_KERNEL 2

static void * kernel_stack;
static unsigned char context_state = CONTEXT_KERNEL;

void * process_context_switch(void * kstack)
{
	kernel_stack = kstack;
	context_state = CONTEXT_PROCESS;
	return current_thread()->stack;
}

int currently_in_kernel_context()
{
		return context_state == CONTEXT_KERNEL;
}

void * kernel_context_switch(void * stack)
{
	if(currently_in_kernel_context())
		return 0;
	current_thread()->stack = stack;
	context_state = CONTEXT_KERNEL;
	return kernel_stack;
}
