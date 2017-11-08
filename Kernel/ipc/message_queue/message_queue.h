#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

  #include <stdlib.h>

  typedef struct message_queue * message_queue_t;

  message_queue_t new_message_queue(char * id_str);
  int write_message_queue(message_queue_t mq, const char * msg, size_t count);
  int read_message_queue(message_queue_t mq, char * buf, size_t count);
  int free_message_queue(message_queue_t mq);
  int add_message_queue(message_queue_t mq);
  const char * get_id_str_message_queue(message_queue_t mq);
  int get_id_message_queue(message_queue_t mq);
  message_queue_t get_message_queue(const char * id_str);
  void remove_message_queue(const char * id_str);

#endif
