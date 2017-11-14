#include <stdint.h>
#include "threadlib.h"

extern uint64_t systemCall(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);

static void thread_wrapper_func(void(*thread)(void *), void * args);

int new_thread(void * start, void * args)
{
  return (int)systemCall(0x160, (uint64_t)thread_wrapper_func, (uint64_t)start, (uint64_t)args, 0, 0);
}

int end_thread()
{
  return (int)systemCall(0x161, 0, 0, 0, 0, 0);
}

void thread_wrapper_func(void(*thread)(void *), void * args)
{
  thread(args);
  end_thread();
}
