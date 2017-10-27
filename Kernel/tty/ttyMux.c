#include "../VideoDriver/driver.h"
#include "vterm.h"
#include "tty.h"

static vterm_t vt1;

static void flip_tty();

void init_tty()
{
  vt1 = new_vterm();
}

void write_tty(const char * buff, size_t count)
{
  write_vterm(vt1, buff, count);
  flip_tty();
}

void read_tty(const char * buff, size_t count)
{
  read_vterm(vt1, buff, count);
}

void flip_tty()
{
  cpytext_vterm(vt1, videoPutChar);
}

void keyPressed_tty(keycode_t key)
{
  keyPressed_vterm(vt1, key);
  flip_tty();
}
