global systemCall
global rand

; uint64_t systemCall(rax, rbx, rcx, rdx, rsi, rdi)
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

rand:
  mov rax, qword [rand_num]
  shl rax, 0x31
  add rax, 0x12
  mov qword [rand_num], rax
  ret

rand_num:
dq 0xC0FFEE
