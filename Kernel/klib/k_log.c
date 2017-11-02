#include "k_printf.h"

#define NULL 0

extern void k_log_putchar(void * p, char c);

void k_log(char *fmt, ...)
{
  va_list va;
  va_start(va,fmt);
  tfp_format(NULL,k_log_putchar,fmt,va);
  va_end(va);
}
