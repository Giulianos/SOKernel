#ifndef PROCESS_H
#define PROCESS_H

  #include "pagemap_list/pagemap_list.h"

  typedef int pid_t;
  typedef struct process * process_t;
  typedef struct thread * thread_t;
  typedef struct thread_queue * thread_queue_t;

  struct process
  {
    pid_t pid;
    pid_t ppid;
    int vt_id;
    void * code;
    pagemap_list_t heap;
    thread_queue_t threads;
  };

  process_t create_process(int module, int ppid, int vt_id, int flags);
  thread_t get_main_thread_process(process_t process);

#endif
