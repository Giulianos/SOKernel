#include <stdlib.h>
#include <stdint.h>
#include "../../scheduler/scheduler.h"

int syscall_new_thread(void * start, void * arg0, void * arg1)
{
  thread_t new;
  new = create_thread(start, current_thread()->process);
  if(new != NULL) {
    map_thread(new);
    get_stack_frame_thread(new)->rdi = (uint64_t)arg0;
    get_stack_frame_thread(new)->rsi = (uint64_t)arg1;
    map_thread(current_thread());
    add_scheduler(new);
    return (int)(new->tid);
  }

  return -1;
}
