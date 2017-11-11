#include <stdlib.h>
#include "../../ipc/semaphore/semaphore.h"

int syscall_sem_wait(char * id_str)
{
  semaphore_t aux;

  aux = get_semaphore(id_str);
  if(aux == NULL) {
    return -1;
  }
  wait_semaphore(aux);
}
