#include <stdlib.h>
#include <lib.h>
#include "vterm.h"
#include "lockedQueue.h"
#include "../Scheduler/process.h"
#include "keyMapping.h"
#include "buffer.h"

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
  lockedQueue_t lockedQ;
  unsigned char kbState;
  kbBuffer_t kbBuffer;
} vterm_concrete_t;

typedef vterm_concrete_t * vterm_t;

static void scroll_vterm(vterm_t vt);
static void putchar_vterm(vterm_t vt, char c);
static void dumpBuffer(vterm_t vt, char * dest);
static void eraseChar_vterm(vterm_t vt);

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
  vterm->kbState=0;

  //Buffer init
  vterm->kbBuffer.f = 0;
  vterm->kbBuffer.l = 0;

  //lockedQueue init
  vterm->lockedQ.f = 0;
  vterm->lockedQ.l = 0;

  return vterm;
}

void free_vterm(vterm_t vt)
{
  free(vt);
}

void scroll_vterm(vterm_t vt)
{
  int i=0;
  //Move everything one line up
  for(; i<TTY_HEIGHT-1; i++) {
    memcpy((void*)&(vt->text[i*TTY_WIDTH]), (void*)&(vt->text[(i+1)*TTY_WIDTH]), sizeof(charattr_t)*TTY_WIDTH);
  }
  //Clean the last line
  for(i=TTY_WIDTH*(TTY_HEIGHT-1); i<TTY_TEXTSIZE; i++) {
    vt->text[i].c = ' ';
  }
  vt->cursor-=TTY_WIDTH;
}

void keyPressed_vterm(vterm_t vt, keycode_t key)
{
  char ascii;
  lockedProcess_t p;
  if(!updateState(key, &(vt->kbState)) && key.action == KBD_ACTION_PRESSED) {
    ascii = getAscii(key, vt->kbState);
    if(ascii == 0x8) {
      if(bufferIsEmpty(&(vt->kbBuffer)))
        return;
      eraseChar_vterm(vt);
      eraseFromBuffer(&(vt->kbBuffer));
    }
    else if(ascii == '\n') {
      putchar_vterm(vt, ascii);
      putCharBuffer(&(vt->kbBuffer), ascii);
      p = pollLockedProcess(&(vt->lockedQ));
      dumpBuffer(vt, p.buffer);
      unlockProcess(p.pid);
    }
    else {
      putchar_vterm(vt, ascii);
      putCharBuffer(&(vt->kbBuffer), ascii);
    }
  }
}

void dumpBuffer(vterm_t vt, char * dest)
{
  while(!bufferIsEmpty(&(vt->kbBuffer))) {
    *(dest++) = getCharBuffer(&(vt->kbBuffer));
  }
}

void eraseChar_vterm(vterm_t vt)
{
  vt->text[vt->cursor-1].c = ' ';
  vt->cursor--;
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

void read_vterm(vterm_t vt, char * buff, size_t count)
{
  lockedProcess_t aux;

  aux.pid = currentProc();
  aux.count = count;
  aux.buffer = buff;
  offerLockedProcess(&(vt->lockedQ), aux);
  lockProcess(aux.pid);
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
