#ifndef SYSCALLS_H
#define SYSCALLS_H

  #include <stdlib.h>
  #include <stdint.h>
  #include "../Scheduler/process.h"

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

  uint64_t syscall_execve(int module_number);
  int syscall_ps(process_info_t * procs);
  int syscall_exit();
  int syscall_read(int fd, char * buff, size_t count);
  int syscall_write(int fd, char * buff, size_t count);
  int syscall_wait(uint64_t pid);
  int syscall_yield();

#endif
