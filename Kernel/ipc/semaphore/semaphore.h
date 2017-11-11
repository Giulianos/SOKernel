#ifndef SEMAPHORE_H
#define SEMAPHORE_H

  typedef struct semaphore * semaphore_t;

  semaphore_t create_semaphore(int initial_value, char * id_str);
  int wait_semaphore(semaphore_t semaphore);
  int signal_semaphore(semaphore_t semaphore);
  int free_semaphore(semaphore_t semaphore);
  int add_semaphore(semaphore_t sem);
  int get_id_semaphore(semaphore_t sem);
  const char * get_id_str_semaphore(semaphore_t semaphore);
  semaphore_t get_semaphore(const char * id_str);
  void remove_semaphore(const char * id_str);


#endif
