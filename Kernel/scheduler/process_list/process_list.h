#ifndef PROCESS_LIST_H
#define PROCESS_LIST_H

  #include "../process.h"

  typedef struct process_list * process_list_t;

  process_list_t new_process_list();
  int add_process_list(process_list_t pl, process_t process);
  int free_process_list(process_list_t pl);
  int remove_process_list(process_list_t pl, process_t process);
  process_t get_process_list(process_list_t pl, pid_t pid);
  int is_empty_process_list(process_list_t pl);

#endif
