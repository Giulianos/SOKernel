#include <stdint.h>
#include <page_allocator.h>

void * syscall_sbrk()
{
  return 0;//(void *)assignAllocatedPage(currentProc(), allocatePage());
}
