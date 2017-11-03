#ifndef THREAD_CQUEUE_H
#define THREAD_CQUEUE_H

  #include "../thread.h"

  typedef struct thread_cqueue * thread_cqueue_t;

  thread_cqueue_t new_thread_cqueue();
  int offer_thread_cqueue(thread_cqueue_t tq, thread_t thread);
  thread_t poll_thread_cqueue(thread_cqueue_t tq);
  thread_t peek_thread_cqueue(thread_cqueue_t tq);
  int rotate_thread_cqueue(thread_cqueue_t tq);

#endif
