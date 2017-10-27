#include "../VideoDriver/driver.h"
#include "vterm.h"
#include "tty.h"

static vterm_t vt[7];
static int active_vt_id = 0;

static void flip_tty();

void init_tty()
{
  int i = 0;

  //Quantity of ttys = 7
  for(; i<7; i++) {
      vt[i] = new_vterm();
  }
  active_vt_id = 0;
}

void write_tty(const char * buff, size_t count)
{
  write_vterm(vt[getProcessVT(currentProc())], buff, count);
  if(getProcessVT(currentProc()) == active_vt_id)
    flip_tty(); //solo se hace si la terminal activa es la del proceso que hizo el write
}

void read_tty(const char * buff, size_t count)
{
  read_vterm(vt[getProcessVT(currentProc())], buff, count);
}

void flip_tty()
{
  cpytext_vterm(vt[active_vt_id], videoPutChar);
}

void keyPressed_tty(keycode_t key) //esto se hace en la terminal activa
{
  if(key.action == KBD_ACTION_PRESSED) {
    switch (key.code) {
      case 0x01:  active_vt_id = 0; flip_tty(); return;
      case 0x02:  active_vt_id = 1; flip_tty(); return;
      case 0x03:  active_vt_id = 2; flip_tty(); return;
      case 0x04:  active_vt_id = 3; flip_tty(); return;
      case 0x05:  active_vt_id = 4; flip_tty(); return;
      case 0x06:  active_vt_id = 5; flip_tty(); return;
      case 0x07:  active_vt_id = 6; flip_tty(); return;
    }
  }
  keyPressed_vterm(vt[active_vt_id], key);
  flip_tty();
}
