#ifndef MUTEX_H
#define MUTEX_H

  typedef struct mutex * mutex_t;

  mutex_t create_mutex(char * id_str);
  int lock_mutex(mutex_t mutex);
  int unlock_mutex(mutex_t mutex);
  int free_mutex(mutex_t mutex);
  int add_mutex(mutex_t mx);
  const char * get_id_str_mutex(mutex_t mutex);
  int get_id_mutex(mutex_t mx);
  mutex_t get_mutex(const char * id_str);
  void remove_mutex(const char * id_str);



#endif
