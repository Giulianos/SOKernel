#include <stdint.h>
#include "../../Scheduler/process.h"
#include "../../PageAllocator/pageAllocator.h"

uint64_t syscall_sbrk()
{
  return (void *)assignAllocatedPage(currentProc(), allocatePage());
}
