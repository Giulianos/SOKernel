#include "../../ipc/mutex/mutex.h"
#include <stdlib.h>

int syscall_mx_lock(int id)
{
  mutex_t aux;

  aux = get_mutex(id);
  if(aux == NULL) {
    return -1;
  }
  lock_mutex(aux);
}
