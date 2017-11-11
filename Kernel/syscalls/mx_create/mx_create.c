#include <stdlib.h>
#include "../../ipc/mutex/mutex.h"

int syscall_mx_create(char * id_str)
{
  mutex_t aux;

  aux = get_mutex(id_str);
  if(aux == NULL) {
    aux = create_mutex(id_str);
    add_mutex(aux);
  }
  return get_id_mutex(aux);
}
