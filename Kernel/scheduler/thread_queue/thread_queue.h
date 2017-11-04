#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

  #include "../thread.h"

  typedef struct thread_queue * thread_queue_t;

  thread_queue_t new_thread_queue();
  int offer_thread_queue(thread_queue_t tq, thread_t thread, void * extra_info);
  thread_t poll_thread_queue(thread_queue_t tq);
  thread_t peek_thread_queue(thread_queue_t tq);
  int free_thread_queue(thread_queue_t tq);
  void * peek_extra_info_thread_queue(thread_queue_t tq);
  int remove_thread_queue(thread_queue_t tq, thread_t thread);
  int is_empty_thread_queue(thread_queue_t tq);

#endif
