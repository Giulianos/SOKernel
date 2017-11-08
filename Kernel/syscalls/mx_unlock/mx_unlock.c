#include "../../ipc/mutex/mutex.h"
#include <stdlib.h>

int syscall_mx_unlock(int id)
{
  mutex_t aux;

  aux = get_mutex(id);
  if(aux == NULL) {
    return -1;
  }
  unlock_mutex(aux);
}
