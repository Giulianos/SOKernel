#include <handlers.h>
#include "KeyboardDriver/driver.h"
#include "MouseDriver/driver.h"
#include "Syscalls/syscalls.h"

void keyboardHandlerC()
{
  keyboardDriver();
}

void mouseHandlerC()
{
  mouseDriver();
}

uint64_t terminalSysCallHandler(uint64_t rax,uint64_t rbx,uint64_t rcx,uint64_t rdx,uint64_t rsi,uint64_t rdi)
{
  switch(rax)
  {
    case SYSCALL_READ:
      return (uint64_t)syscall_read((int)rbx, (char *)rcx, (size_t)rdx);
    case SYSCALL_WRITE:
      return (uint64_t)syscall_write((int)rbx, (char *)rcx, (size_t)rdx);
    case SYSCALL_EXECVE:
      return (uint64_t)syscall_execve((int)rbx);
    case SYSCALL_EXIT:
      return (uint64_t)syscall_exit();
    case SYSCALL_WAIT:
      return (uint64_t)syscall_wait(rbx);
    case SYSCALL_YIELD:
      return (uint64_t)syscall_yield();
    case SYSCALL_PS:
      return (uint64_t)syscall_ps((process_info_t *)rbx);
    case SYSCALL_TOGGLEVIDEO:
      //Sin implementar
      break;
    case SYSCALL_CLEAR:
      //Sin implementar
      break;
    case SYSCALL_VIDEODRAW:
      //Sin implementar
      break;
    case SYSCALL_GETKEYSTATE:
      //Sin implementar
      break;
  }
  return 0;
}
