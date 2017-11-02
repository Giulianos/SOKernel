#include <stdint.h>
#include "../../Scheduler/process.h"

uint64_t syscall_execve(int module_number)
{
  pcb_t aux_proc;

  aux_proc = createProcess(module_number, currentProc(), getProcessVT(currentProc()));
  scheduleProcess(aux_proc);

  return aux_proc.pid;
}
