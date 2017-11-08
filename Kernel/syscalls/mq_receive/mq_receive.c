#include <lib.h>
#include "../../ipc/message_queue.h"

int syscall_mq_receive(char * mq_name, char * buf, size_t len)
{
  message_queue_t aux;

  aux = get_message_queue(mq_name);
  if(aux == NULL) {
    return -1; /*mq doesn't exist*/
  }
  return read_message_queue(aux, buf, len);
}
