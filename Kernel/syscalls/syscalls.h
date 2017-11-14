#ifndef SYSCALLS_H
#define SYSCALLS_H

  #include <stdlib.h>
  #include <stdint.h>

  #define SYSCALL_EXIT 0x01
  #define SYSCALL_FORK 0x02
  #define SYSCALL_READ 0x03
  #define SYSCALL_WRITE 0x04
  #define SYSCALL_WAIT 0x07
  #define SYSCALL_EXECVE 0x0B
  #define SYSCALL_CLEAR 0x44
  #define SYSCALL_SBRK 0x2D
  #define SYSCALL_TOGGLEVIDEO 0x45
  #define SYSCALL_VIDEODRAW 0x046
  #define SYSCALL_GETKEYSTATE 0x47
  #define SYSCALL_YIELD 0x48
  #define SYSCALL_LS_PROCS 0x49
  #define SYSCALL_MQ_OPEN 0x115
  #define SYSCALL_MQ_UNLINK 0x116
  #define SYSCALL_MQ_SEND 0x117
  #define SYSCALL_MQ_RECEIVE 0x118
  #define SYSCALL_MX_CREATE 0x120
  #define SYSCALL_MX_LOCK 0x121
  #define SYSCALL_MX_UNLOCK 0x122
  #define SYSCALL_SEM_CREATE 0x130
  #define SYSCALL_SEM_WAIT 0x131
  #define SYSCALL_SEM_SIGNAL 0x132
  #define SYSCALL_FG 0x150
  #define SYSCALL_NEW_THREAD 0x160
  #define SYSCALL_EXIT_THREAD 0x161

  uint64_t syscall_execve(int module_number);
  void * syscall_sbrk();
  int syscall_exit();
  int syscall_read(int fd, char * buff, size_t count);
  int syscall_write(int fd, char * buff, size_t count);
  int syscall_wait(int pid);
  int syscall_yield();
  int syscall_fork();
  int syscall_mq_unlink(char * mq_name);
  int syscall_mq_send(char * mq_name, char * msg, size_t len);
  int syscall_mq_receive(char * mq_name, char * buf, size_t len);
  int syscall_mq_open(char * mq_name);
  int syscall_mx_create(char * id_str);
  int syscall_mx_lock(char * id_str);
  int syscall_mx_unlock(char * id_str);
  int syscall_sem_create(int initial_value, char * id_str);
  int syscall_sem_wait(char * id_str);
  int syscall_sem_signal(char * id_str);
  int syscall_fg(int pid);
  int syscall_ls_procs(void * ret_buffer);
  int syscall_new_thread(void * start, void * arg0, void * arg1);
  int syscall_exit_thread();


#endif
