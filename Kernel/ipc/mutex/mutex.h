#ifndef MUTEX_H
#define MUTEX_H

  typedef struct mutex * mutex_t;

  mutex_t create_mutex();
  int lock_mutex(mutex_t mutex);
  int unlock_mutex(mutex_t mutex);
  int free_mutex(mutex_t mutex);
  int add_mutex(mutex_t mx);
  int get_id_mutex(mutex_t mx);
  mutex_t get_mutex(int id);
  void remove_mutex(int id);

#endif
