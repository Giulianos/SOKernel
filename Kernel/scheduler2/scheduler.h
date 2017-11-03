#ifndef SCHEDULER_H
#define SCHEDULER_H

  #include "thread.h"
  #include "process.h"

  int init_scheduler();
  void add_scheduler(thread_t thread);
  void schedule_scheduler();
  thread_t current_thread();

#endif
