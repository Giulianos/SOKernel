#include <handlers.h>
#include <hardwareCom.h>
#include "Scheduler/process.h"
#include "KeyboardDriver/driver.h"
#include "MouseDriver/driver.h"
#include "tty/tty.h"

#define SYSCALL_EXIT 0x01
#define SYSCALL_READ 0x03
#define SYSCALL_WRITE 0x04
#define SYSCALL_WAIT 0x07
#define SYSCALL_EXECVE 0x0B
#define SYSCALL_CLEAR 0x44
#define SYSCALL_TOGGLEVIDEO 0x45
#define SYSCALL_VIDEODRAW 0x046
#define SYSCALL_GETKEYSTATE 0x47
#define SYSCALL_YIELD 0x48
#define SYSCALL_PS 0x49

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
  pcb_t aux_proc;

  switch(rax)
  {
    case SYSCALL_READ:
      read_tty((uint8_t*)rcx, rdx);
      break;
    case SYSCALL_WRITE:
      write_tty((uint8_t*)rcx, rdx);
      break;
    case SYSCALL_EXECVE:
      aux_proc = createProcess(rbx, currentProc(), getProcessVT(currentProc()));
      scheduleProcess(aux_proc);
      return aux_proc.pid;
    case SYSCALL_EXIT:
      killProc(currentProc());
      break;
    case SYSCALL_WAIT:
      waitProcess(currentProc(), rbx);
      break;
    case SYSCALL_YIELD:
      schedule();
      break;
    case SYSCALL_PS:
      return listProcs((process_info_t *)rbx);
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
