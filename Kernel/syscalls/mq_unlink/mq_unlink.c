#include <lib.h>
#include "../../ipc/message_queue/message_queue.h"

int syscall_mq_unlink(char * mq_name)
{
  message_queue_t aux;

  aux = get_message_queue(mq_name);
  if(aux == NULL) {
    return -1; /*mq doesn't exist*/
  }
  return free_message_queue(aux);
}
