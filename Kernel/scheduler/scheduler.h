#ifndef SCHEDULER_H
#define SCHEDULER_H

  #include "thread.h"
  #include "process.h"

  int init_scheduler();
  void add_scheduler(thread_t thread);
  void schedule_scheduler();
  thread_t current_thread();
  int create_blocked_queue();
  int free_blocked_queue();
  int block_thread(thread_t thread, int queue, void * extra_info);
  int unblock_from_queue_thread(int queue, void(*callback)(void *));
  int terminate_thread(thread_t thread);

#endif
