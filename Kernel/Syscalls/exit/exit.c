#include <stdint.h>
#include "../../Scheduler/process.h"

int syscall_exit()
{
  killProc(currentProc());
  return 1;
}
