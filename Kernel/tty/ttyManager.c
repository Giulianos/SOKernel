#include "../VideoDriver/driver.h"
#include "vterm.h"

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

void flip_tty()
{
  cpytext_vterm(vt1, videoPutChar);
}
