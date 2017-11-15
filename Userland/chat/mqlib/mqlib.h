#ifndef MQLIB_H
#define MQLIB_H

  #include <stdlib.h>

  int mq_unlink(char * mq_name);
  int mq_send(char * mq_name, char * msg, size_t len);
  int mq_receive(char * mq_name, char * msg, size_t len);
  int mq_open(char * mq_name);

#endif
