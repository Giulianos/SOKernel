#ifndef MXLIB_H
#define MXLIB_H

  int mx_create(char * id_str);
  int mx_lock(char * id_str);
  int mx_unlock(char * id_str);
  int mx_close(char * id_str);

#endif
