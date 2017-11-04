#ifndef THREAD_H
#define THREAD_H

  #define THREAD_READY 1
  #define THREAD_BLOCKED 2

  typedef int tid_t;
  typedef struct thread * thread_t;
  typedef struct process * process_t;

  struct thread
  {
    tid_t tid;
    process_t process;
    int state;
    void * code;
    void * stack;
  };

  thread_t create_thread(void * code, process_t process);
  int get_vt_thread(thread_t thread);

#endif
