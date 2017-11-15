#include <stdlib.h>
#include "../../ipc/mutex/mutex.h"

int syscall_mx_close(char * id_str)
{
  remove_mutex(id_str);
  return 1;
}
