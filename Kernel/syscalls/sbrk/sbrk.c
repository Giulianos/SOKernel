#include <stdint.h>
#include "../../scheduler/scheduler.h"

void * syscall_sbrk()
{
  return allocate_page_heap_process(current_thread()->process);
}
