#include "../../scheduler2/scheduler.h"

int syscall_yield()
{
  schedule_scheduler();
  return 1;
}
