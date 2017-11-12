#include <stdint.h>
#include <stdlib.h>
#include "../../scheduler/scheduler.h"

int syscall_wait(int pid)
{
  process_t process = get_process((pid_t)pid);

  //k_log("wait(%d): Blocking thread in %d\n", pid, get_process((pid_t)pid)->pid);
  if(process != NULL) {
    block_thread(current_thread(), process->waiting_queue_id, NULL);
    return 1;
  }
  k_log("Can't wait to process %d, it doesn't exist!\n");
  return -1;
}
