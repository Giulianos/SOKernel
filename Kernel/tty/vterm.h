#ifndef TTY_H_
#define TTY_H_

  #include <stdlib.h>
  #include <stdint.h>

  #define TTY_WIDTH 80
  #define TTY_HEIGHT 25
  #define TTY_TEXTSIZE TTY_WIDTH*TTY_HEIGHT

  #define TTY_DFLT_TXT_ATR 0x0F

  typedef struct vterm_concrete * vterm_t;

  vterm_t new_vterm();
  void free_vterm(vterm_t vt);
  void format_vterm(vterm_t vt, unsigned char format);
  void write_vterm(vterm_t vt, const char * buff, size_t count);
  void cpytext_vterm(vterm_t vt, void(*putchar)(char, uint8_t, uint8_t, uint8_t));

#endif
