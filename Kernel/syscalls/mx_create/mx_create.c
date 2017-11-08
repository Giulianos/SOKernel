#include "../../ipc/mutex/mutex.h"

int syscall_mx_create()
{
  mutex_t aux;

  aux = create_mutex();
  return get_id_mutex(aux);
}
