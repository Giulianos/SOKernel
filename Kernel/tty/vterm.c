#include "vterm.h"
#include <stdlib.h>
#include <lib.h>

typedef struct charattr
{
  char c;
  unsigned char attr;
} charattr_t;

typedef struct vterm_concrete
{
  int id;
  charattr_t text[TTY_TEXTSIZE];
  int cursor;
  unsigned char format;
} vterm_concrete_t;

typedef vterm_concrete_t * vterm_t;

static void scroll_vterm(vterm_t vt);
static void putchar_vterm(vterm_t vt, char c);

static int tty_last_id = 0;

vterm_t new_vterm()
{
  int i=0;
  vterm_t vterm = kalloc(sizeof(vterm_concrete_t));

  vterm->id=tty_last_id++;
  vterm->format = TTY_DFLT_TXT_ATR;
  for(; i<TTY_TEXTSIZE; i++) {
    vterm->text[i].c = ' ';
    vterm->text[i].attr = TTY_DFLT_TXT_ATR;
  }
  vterm->cursor=0;

  return vterm;
}

void free_vterm(vterm_t vt)
{
  free(vt);
}

void scroll_vterm(vterm_t vt)
{
  int i=0;
  for(; i<TTY_HEIGHT-1; i++) {
    memcpy((void*)&(vt->text[i*TTY_WIDTH]), (void*)&(vt->text[(i+1)*TTY_WIDTH]), sizeof(charattr_t)*TTY_WIDTH);
  }
  vt->cursor-=TTY_WIDTH;
}

void putchar_vterm(vterm_t vt, char c)
{
  if(vt->cursor == TTY_TEXTSIZE)
    scroll_vterm(vt);
  if(c=='\n') {
    do {
      vt->text[vt->cursor].c = ' ';
      vt->text[vt->cursor].attr = vt->format;
      vt->cursor++;
    } while(vt->cursor%TTY_WIDTH!=0);
  } else {
    vt->text[vt->cursor].c = c;
    vt->text[vt->cursor].attr = vt->format;
    vt->cursor++;
  }
}

void write_vterm(vterm_t vt, const char * buff, size_t count)
{
  while(count--) {
    putchar_vterm(vt, *(buff++));
  }
}

void format_vterm(vterm_t vt, unsigned char format)
{
  vt->format = format;
}

void cpytext_vterm(vterm_t vt, void(*putchar)(char, uint8_t, uint8_t, uint8_t))
{
  int i=0;

  for(; i<TTY_TEXTSIZE; i++) {
    putchar(vt->text[i].c, i/TTY_WIDTH, i%TTY_WIDTH, vt->text[i].attr);
  }
}
