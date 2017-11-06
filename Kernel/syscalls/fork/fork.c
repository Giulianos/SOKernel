#include <stdint.h>
#include <lib.h>
#include <scheduler.h>

int syscall_fork()
{
  process_t aux_process;

  aux_process =  clone_process(current_thread()->process, current_thread());
  add_scheduler(get_main_thread_process(aux_process));
  return aux_process->pid;
}
