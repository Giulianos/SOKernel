#include <stdlib.h>
#include "../../tty/tty.h"

int syscall_write(int fd, char * buff, size_t count)
{
  write_tty(buff, count);
  return 0;
}
