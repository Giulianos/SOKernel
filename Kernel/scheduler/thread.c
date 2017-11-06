#include <stdlib.h>
#include <lib.h>
#include "thread.h"
#include "process.h"
#include "thread_stack_frame.h"
#include "thread_queue/thread_queue.h"
#include <paging.h>
#include <page_allocator.h>
#include <stddef.h>

#define THREAD_DEBUG_MSG

static void * setup_stack_frame_thread(thread_t thread);
static void clone_thread_stack(thread_t src_thread, thread_t dst_src);
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
  k_log("Stack was at %x\n", thread->stack);
  k_log("Stack is at %x\n", ret->stack);
  k_log("Code was at %x\n", thread->code);
  k_log("Code is at %x\n", ret->code);
  #endif

  return ret;
}

void clone_thread_stack(thread_t src_thread, thread_t dst_thread)
{
  thread_stack_frame_t * dst_stack_frame;
  thread_stack_frame_t * src_stack_frame;

  dst_thread->stack_page = allocatePage();
  k_memcpy(dst_thread->stack_page, src_thread->stack_page, pageSize());

  dst_stack_frame = get_stack_frame_thread(dst_thread);
  src_stack_frame = get_stack_frame_thread(src_thread);

  dst_thread->stack = translate_addr_page(src_thread->stack, dst_thread->stack_page);
  dst_stack_frame->rip = (uint64_t)src_stack_frame->rip;
  dst_stack_frame->rsp = (uint64_t)translate_addr_page((void *)src_stack_frame->rsp, dst_thread->stack_page);
  dst_stack_frame->rax = 0x0;
}

int get_vt_thread(thread_t thread)
{
  if(thread == NULL)
    return -1;
  return thread->process->vt_id;
}

thread_stack_frame_t * get_stack_frame_thread(thread_t thread)
{
  return (thread_stack_frame_t *)((char *)thread->stack);
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
