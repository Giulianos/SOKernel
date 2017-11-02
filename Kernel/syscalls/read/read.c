#include <stdlib.h>
#include "../../tty/tty.h"

int syscall_read(int fd, char * buff, size_t count)
{
  read_tty(buff, count);
  return 0;
}
