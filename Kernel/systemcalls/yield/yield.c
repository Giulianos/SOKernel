#include "../../Scheduler/process.h"

int syscall_yield()
{
  schedule();
  return 1;
}
