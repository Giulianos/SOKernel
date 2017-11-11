#include <stdlib.h>
#include "../../ipc/semaphore/semaphore.h"

int syscall_sem_create(int initial_value, char * id_str)
{
  semaphore_t aux;

  aux = get_semaphore(id_str);
  if(aux == NULL) {
    aux = create_semaphore(initial_value, id_str);
    add_semaphore(aux);
  }
  return get_id_semaphore(aux);
}
