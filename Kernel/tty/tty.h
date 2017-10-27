#ifndef TTY_H_
#define TTY_H_

#include <stdlib.h>
#include "../KeyboardDriver/driver.h"

void init_tty();
void read_tty(const char * buff, size_t count);
void write_tty(const char * buff, size_t count);
void keyPressed_tty(keycode_t key);

#endif
