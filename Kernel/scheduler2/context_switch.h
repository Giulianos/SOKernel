#ifndef CONTEXT_SWITCH_H
#define CONTEXT_SWITCH_H

	void * process_context_switch(void * kstack);
	int currently_in_kernel_context();
	void * kernel_context_switch(void * stack);

#endif
