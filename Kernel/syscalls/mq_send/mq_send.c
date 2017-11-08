#include <lib.h>
#include "../../ipc/message_queue/message_queue.h"

int syscall_mq_send(char * mq_name, char * msg, size_t len)
{
  message_queue_t aux;

  aux = get_message_queue(mq_name);
  if(aux == NULL) {
    return -1; /*mq doesn't exist*/
  }
  return write_message_queue(aux, msg, len);
}
