#include <stdint.h>
#include "../../Scheduler/process.h"
#include "../../PageAllocator/pageAllocator.h"

void * syscall_sbrk()
{
  return (void *)assignAllocatedPage(currentProc(), allocatePage());
}
