#include <stdlib.h>
#include "../../ipc/mutex/mutex.h"

int syscall_mx_unlock(char * id_str)
{
  mutex_t aux;

  aux = get_mutex(id_str);
  if(aux == NULL) {
    return -1;
  }
  unlock_mutex(aux);
  return 1;
}
