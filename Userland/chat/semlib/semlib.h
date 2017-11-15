#ifndef SEMLIB_H
#define SEMLIB_H

  int sem_create(char * id_str, int initial_value);
  int sem_wait(char * id_str);
  int sem_signal(char * id_str);

#endif
