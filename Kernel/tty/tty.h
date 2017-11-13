#ifndef TTY_H_
#define TTY_H_

#include <stdlib.h>
#include "../KeyboardDriver/driver.h"
#include "../scheduler/scheduler.h"

void init_tty();
void read_tty(char * buff, size_t count);
void write_tty(const char * buff, size_t count);
void keyPressed_tty(keycode_t key);
void set_fg_tty(int vterm_id, tid_t tid);

#endif
