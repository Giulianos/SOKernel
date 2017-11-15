#include <stdlib.h>
#include <lib.h>
#include "vterm.h"
#include "lockedQueue.h"
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
  tid_t fg_tid;
  int blocked_queue_id;
  unsigned char kbState;
  kbBuffer_t kbBuffer;
} vterm_concrete_t;

struct read_block_details
{
  void * buffer;
  size_t count;
  vterm_t vt;
};

typedef vterm_concrete_t * vterm_t;
typedef struct read_block_details * read_block_details_t;

static void scroll_vterm(vterm_t vt);
static void putchar_vterm(vterm_t vt, char c);
static void read_unblock_callback(void * info);
static void dump_buffer(vterm_t vt, char * dest, size_t count);
static void eraseChar_vterm(vterm_t vt);

static int tty_last_id = 0;

vterm_t new_vterm()
{
  int i=0;
  vterm_t vterm = k_malloc(sizeof(vterm_concrete_t));

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

  /* We ask the scheduler for a blocked queue */
  vterm->blocked_queue_id = create_blocked_queue();

  return vterm;
}

void free_vterm(vterm_t vt)
{
  k_free(vt);
}

void set_fg_vterm(vterm_t vt, tid_t tid)
{
  vt->fg_tid = tid;
}

void scroll_vterm(vterm_t vt)
{
  int i=0;
  //Move everything one line up
  for(; i<TTY_HEIGHT-1; i++) {
    k_memcpy((void*)&(vt->text[i*TTY_WIDTH]), (void*)&(vt->text[(i+1)*TTY_WIDTH]), sizeof(charattr_t)*TTY_WIDTH);
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
      unblock_from_queue_thread(vt->blocked_queue_id, read_unblock_callback);
    }
    else {
      putchar_vterm(vt, ascii);
      putCharBuffer(&(vt->kbBuffer), ascii);
    }
  }
}

void read_unblock_callback(void * info)
{
  read_block_details_t det = (read_block_details_t)info;
  dump_buffer(det->vt, det->buffer, det->count);
  k_free(info);
}

void dump_buffer(vterm_t vt, char * dest, size_t count)
{
  if(!count)
  {
    while(!bufferIsEmpty(&(vt->kbBuffer))) {
      *(dest++) = getCharBuffer(&(vt->kbBuffer));
    }
  } else {
    while(!bufferIsEmpty(&(vt->kbBuffer)) && count--) {
      *(dest++) = getCharBuffer(&(vt->kbBuffer));
    }
    while(!bufferIsEmpty(&(vt->kbBuffer))) {
      getCharBuffer(&(vt->kbBuffer));
    }
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

int read_vterm(vterm_t vt, char * buff, size_t count)
{
  read_block_details_t details;

  if(current_thread()->tid != vt->fg_tid) {
    suspend_thread(current_thread());
    return -1;
  }

  details = (read_block_details_t)k_malloc(sizeof(struct read_block_details));

  if(details == NULL) {
    #ifdef VTERM_DEBUG_MSG
    k_log("Error while reading from terminal\n");
    #endif
    return -1;
  }
  details->buffer = buff;
  details->count = count;
  details->vt = vt;
  block_thread(current_thread(), vt->blocked_queue_id, (void *)details);
  return 1;
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
