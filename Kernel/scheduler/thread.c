#include <stdlib.h>
#include <lib.h>
#include "thread.h"
#include "process.h"
#include "thread_stack_frame.h"
#include "thread_queue/thread_queue.h"
#include <paging.h>
#include <page_allocator.h>

static void * setup_stack_frame_thread(thread_t thread);
static void clone_thread_stack(thread_t src_thread, thread_t dest_src);
static void * to_stack_start(void * stack);

thread_t create_thread(void * code, process_t process)
{
  thread_t ret = (thread_t)k_malloc(sizeof(struct thread));

  if(ret == NULL) {
    #ifdef THREAD_DEBUG_MSG
    k_log("Couldn't allocate space for thread!\n");
    #endif
    return NULL;
  }
  if(process == NULL) {
    #ifdef THREAD_DEBUG_MSG
    k_log("Can't create thread without a process!\n");
    #endif
    return NULL;
  }

  ret->tid = -1; /* For the scheduler it means a new thread */
  ret->process = process;
  offer_thread_queue(process->threads, ret, NULL);
  ret->state = THREAD_READY;
  ret->blocked_queue = -1;
  ret->code = code;
  ret->stack_page = allocatePage();
  ret->stack = to_stack_start(ret->stack_page);
  ret->stack = setup_stack_frame_thread(ret);

  #ifdef THREAD_DEBUG_MSG
  k_log("Thread with tid:%d has been created!\n", ret->tid);
  k_log("Stack is at %x\n", ret->stack);
  #endif

	return ret;
}

thread_t clone_thread(thread_t thread, process_t owner_process)
{
  thread_t ret = (thread_t)k_malloc(sizeof(struct thread));

  if(ret == NULL) {
    #ifdef THREAD_DEBUG_MSG
    k_log("Couldn't allocate space for thread!\n");
    #endif
    return NULL;
  }
  if(owner_process == NULL) {
    #ifdef THREAD_DEBUG_MSG
    k_log("Can't clone thread without an owner process!\n");
    #endif
    return NULL;
  }

  ret->tid = -1; /* For the scheduler it means a new thread */
  ret->process = owner_process;
  offer_thread_queue(owner_process->threads, ret, NULL);
  ret->state = THREAD_READY;
  ret->blocked_queue = -1;
  ret->code = thread->code;
  clone_thread_stack(thread, ret);

  #ifdef THREAD_DEBUG_MSG
  k_log("Thread with tid:%d has been created!\n", ret->tid);
  k_log("Stack is at %x\n", ret->stack);
  #endif

  return ret;
}

void clone_thread_stack(thread_t src_thread, thread_t dest_src)
{
  dest_src->stack_page = allocatePage();

  k_memcpy(dest_src->stack_page, src_thread->stack_page, pageSize());
  dest_src->stack = (void *)( (char *)dest_src->stack_page +
                              (size_t)((char *)src_thread->stack -
                              (char *)src_thread->stack_page) );
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
