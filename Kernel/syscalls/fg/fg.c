#include <stdint.h>
#include "../../tty/tty.h"
#include "../../scheduler/scheduler.h"

int syscall_fg(int pid)
{
  process_t process;

  if(pid < 0) {
    set_fg_tty(get_vt_thread(current_thread()), current_thread()->tid);
    return 1;
  }

  process = get_process((pid_t)pid);
  if(process != NULL) {
    set_fg_tty(get_vt_thread(current_thread()), get_main_thread_process(process)->tid);
    return 1;
  }
  return -1;
}
