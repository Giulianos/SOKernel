#include <stdlib.h>
#include <stdint.h>
#include "../../scheduler/scheduler.h"

int syscall_exit_thread()
{
  return terminate_thread(current_thread());
}
