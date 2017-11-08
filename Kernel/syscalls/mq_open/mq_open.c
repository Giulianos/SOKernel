#include <lib.h>
#include "../../ipc/message_queue/message_queue.h"

int syscall_mq_open(char * mq_name)
{
  message_queue_t aux;

  aux = get_message_queue(mq_name);
  if(aux == NULL) {
    add_message_queue(aux=new_message_queue(mq_name));
  }
  return get_id_message_queue(aux);
}
