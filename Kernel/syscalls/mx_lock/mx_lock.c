#include <stdlib.h>
#include "../../ipc/mutex/mutex.h"

int syscall_mx_lock(char * id_str)
{
  mutex_t aux;

  aux = get_mutex(id_str);
  if(aux == NULL) {
    return -1;
  }
  lock_mutex(aux);
  return 1;
}
