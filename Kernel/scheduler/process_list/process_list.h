#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

  #include "../process.h"

  typedef struct process_list * process_list_t;
  typedef void(* callback_t)(process_t, int, void *);

  process_list_t new_process_list();
  int add_process_list(process_list_t pl, process_t process);
  int free_process_list(process_list_t pl);
  int remove_process_list(process_list_t pl, process_t process);
  process_t get_process_list(process_list_t pl, pid_t pid);
  int is_empty_process_list(process_list_t pl);
  void each_process_list(process_list_t pl, callback_t callback, void * optional_arg);
  int size_process_list(process_list_t pl);

#endif
