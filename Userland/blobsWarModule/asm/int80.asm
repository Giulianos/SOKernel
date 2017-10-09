GLOBAL int80
global systemCall
%include "./asm/macros.m"

int80:

    pushRegisters
    INT 80h
    popRegisters
    ret

systemCall:
  push rbp
  mov rbp, rsp

  mov rax, rdi
  mov rbx, rsi
  mov r10, rcx
  mov rcx, rdx
  mov rdx, r10
  mov rsi, r8
  mov rdi, r9
  int 0x80

  mov rsp, rbp
  pop rbp
  ret
