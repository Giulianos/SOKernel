#include <stdlib.h>
#include <lib.h>
#include "thread.h"
#include "process.h"
#include "thread_stack_frame.h"
#include "thread_queue/thread_queue.h"
#include "../PagingManager/paging.h"
#include "../PageAllocator/pageAllocator.h"

static void * setup_stack_frame_thread(thread_t thread);
void * to_stack_start(void * stack);

thread_t create_thread(void * code, process_t process)
{
  thread_t ret = (thread_t)k_malloc(sizeof(struct thread));

  if(ret == NULL) {
    k_log("Couldn't allocate space for thread!\n");
    return NULL;
  }
  if(process == NULL) {
    k_log("Couldn't create thread without a process!\n");
    return NULL;
  }

  ret->tid = 0;
  ret->process = process;
  offer_thread_queue(process->threads, ret, NULL);
  ret->state = THREAD_READY;
  ret->code = code;
  ret->stack = to_stack_start(allocatePage());
  ret->stack = setup_stack_frame_thread(ret);

  k_log("Thread with tid:%d has been created!\n", ret->tid);
  k_log("Stack is at %x\n", ret->stack);

	return ret;
}

int get_vt_thread(thread_t thread)
{
  if(thread == NULL)
    return -1;
  return thread->process->vt_id;
}

void * setup_stack_frame_thread(thread_t thread)
{
	thread_stack_frame_t * stack = (thread_stack_frame_t *)thread->stack - 1;

	stack->rip = (uint64_t)thread->code;
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

	return (void *)stack;
}

void * to_stack_start(void * stack)
{
	return stack + pageSize() - 0x1;
}
