#ifndef PROCESS_H
#define PROCESS_H

  #include <datastructures/pagemap_list.h>

  typedef int pid_t;
  typedef struct process * process_t;
  typedef struct thread * thread_t;
  typedef struct thread_queue * thread_queue_t;
  typedef struct userland_process_struct userland_process_struct;

  struct process
  {
    pid_t pid;
    pid_t ppid;
    int vt_id;
    void * code;
    pagemap_list_t heap;
    thread_queue_t threads;
    int waiting_queue_id; /*threads waiting for this process*/
  };

  struct userland_process_struct
  {
    pid_t pid;
    pid_t ppid;
    int vt_id;
    int allocated_memory;
  };

  process_t create_process(int module, int ppid, int vt_id, int flags);
  process_t clone_process(process_t process, thread_t calling_thread);
  void kill_process(process_t process);
  thread_t get_main_thread_process(process_t process);
  void replace_process_image(process_t process, int module);
  void * allocate_page_heap_process(process_t process);

#endif
