#include <stdint.h>
#include <scheduler.h>

int syscall_exit()
{
  kill_process(current_thread()->process);
  schedule_scheduler();
  return 1;
}
