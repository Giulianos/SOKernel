#include "syscalls.h"

uint64_t syscall_handler(uint64_t rax,uint64_t rbx,uint64_t rcx,uint64_t rdx,uint64_t rsi,uint64_t rdi)
{
  switch(rax)
  {
    case SYSCALL_EXIT:
      return (uint64_t)syscall_exit();
    case SYSCALL_FORK:
      return (uint64_t)syscall_fork();
    case SYSCALL_READ:
      return (uint64_t)syscall_read((int)rbx, (char *)rcx, (size_t)rdx);
    case SYSCALL_WRITE:
      return (uint64_t)syscall_write((int)rbx, (char *)rcx, (size_t)rdx);
    case SYSCALL_EXECVE:
      return (uint64_t)syscall_execve((int)rbx);
    case SYSCALL_WAIT:
      return (uint64_t)syscall_wait((int)rbx);
    case SYSCALL_YIELD:
      return (uint64_t)syscall_yield();
    case SYSCALL_PS:
      return (uint64_t)syscall_ps();
    case SYSCALL_SBRK:
      return (uint64_t)syscall_sbrk();
    case SYSCALL_MQ_OPEN:
      return (uint64_t)syscall_mq_open((char *)rbx);
    case SYSCALL_MQ_UNLINK:
      return (uint64_t)syscall_mq_unlink((char *)rbx);
    case SYSCALL_MQ_SEND:
      return (uint64_t)syscall_mq_send((char *)rbx, (char *)rcx, (size_t)rdx);
    case SYSCALL_MQ_RECEIVE:
      return (uint64_t)syscall_mq_receive((char *)rbx, (char *)rcx, (size_t)rdx);
    case SYSCALL_MX_CREATE:
      return (uint64_t)syscall_mx_create((char *)rbx);
    case SYSCALL_MX_LOCK:
      return (uint64_t)syscall_mx_lock((char *)rbx);
    case SYSCALL_MX_UNLOCK:
      return (uint64_t)syscall_mx_unlock((char *)rbx);
    case SYSCALL_SEM_CREATE:
      return (uint64_t)syscall_sem_create((int)rcx, (char *)rbx);
    case SYSCALL_SEM_WAIT:
      return (uint64_t)syscall_sem_wait((char *)rbx);
    case SYSCALL_SEM_SIGNAL:
      return (uint64_t)syscall_sem_signal((char *)rbx);
    case SYSCALL_FG:
      return (uint64_t)syscall_fg((int)rbx);
  }
  return 0;
}
