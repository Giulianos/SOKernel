#include <stdint.h>
#include "../scheduler/scheduler.h"

int syscall_execve(int module_number)
{
  process_t current_process = current_thread()->process;

  replace_process_image(current_process, module_number);
  add_scheduler(get_main_thread_process(current_process));
  schedule_scheduler();
  return 1;
}
