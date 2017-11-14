#include <stdint.h>
#include <stdlib.h>
#include "../../scheduler/scheduler.h"

int syscall_ls_procs(void * ret_buffer)
{
  int proc_quantity = size_process_list(get_pl());

  if(ret_buffer == NULL)
    return proc_quantity;

  list_process((userland_process_struct *)ret_buffer);
  return proc_quantity;
}
