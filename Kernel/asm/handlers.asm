global timerTickHandler
global keyboardHandler
extern keyboardHandlerC
global mouseHandler
extern mouseHandlerC
global systemCallHandler
extern terminalSysCallHandler
global spuriousInt7Handler
global spuriousInt15Handler
extern inputB
extern outputB
extern ncPrintHex
extern restoreKernelStack
extern restoreProcessStack
extern schedule

%include "./asm/macros.m"

keyboardHandler:
  pushaq
  mov [0xB8000], byte 'K'
  mov [0xB8002], byte 'K'
  mov rdi, rsp ;Guardo el stack pointer del proceso y recupero el del kernel
  call restoreKernelStack ;Esta funcion guarda el stack de proceso y recupera el del kernel
  cmp rax, 0x0 ;(en caso de que ya este en el kernel no hace nada y deuvelve 0)
  je .keyboardHandler_inKernel
  mov rsp, rax

  call keyboardHandlerC
  mov al, 0x20
  out 0x20, al ;ACK al master pic

  mov rdi, rsp
	call restoreProcessStack
	mov rsp, rax
	popaq
  ;mov [0xB8000], byte 'U'
  iretq

.keyboardHandler_inKernel:

  call keyboardHandlerC
  mov al, 0x20
  out 0x20, al ;ACK al master pic

  popaq
  iretq
;--------------------------------------------------------------------------------------------------------


mouseHandler:
  pushaq
  mov [0xB8000], byte 'K'
  mov [0xB8002], byte 'M'
  mov rdi, rsp ;Guardo el stack pointer del proceso y recupero el del kernel
  call restoreKernelStack ;Esta funcion guarda el stack de proceso y recupera el del kernel
  cmp rax, 0x0 ;(en caso de que ya este en el kernel no hace nada y deuvelve 0)
  je .mouseHandler_inKernel
  mov rsp, rax

  call mouseHandlerC
  mov al, 0x20
  out 0x20, al ;ACK al master pic
  out 0xA0, al

  mov rdi, rsp
	call restoreProcessStack
	mov rsp, rax
	popaq
  mov [0xB8000], byte 'U'
  iretq

.mouseHandler_inKernel:

  call keyboardHandlerC
  mov al, 0x20
  out 0x20, al ;ACK al master pic

  popaq
  mov [0xB8000], byte 'K'
  iretq
;--------------------------------------------------------------------------------------------------------

systemCallHandler:
  pushaq
  mov [0xB8000], byte 'K'
  mov [0xB8002], byte 'S'

  mov qword [temp_rax], rax
  mov qword [temp_rbx], rbx
  mov qword [temp_rcx], rcx
  mov qword [temp_rdx], rdx
  mov qword [temp_rdi], rdi
  mov qword [temp_rsi], rsi
  mov rdi, rsp ;Guardo el stack pointer del proceso y recupero el del kernel
  call restoreKernelStack ;Esta funcion guarda el stack de proceso y recupera el del kernel
  cmp rax, 0x0 ;(en caso de que ya este en el kernel no hace nada y deuvelve 0)
  je .systemCallHandler_inKernel
  mov rsp, rax:

  mov r9, qword [temp_rdi]
  mov r8, qword [temp_rsi]
  mov rdx, qword [temp_rcx]
  mov rcx, qword [temp_rdx]
  mov rdi, qword [temp_rax]
  mov rsi, qword [temp_rbx]
  call terminalSysCallHandler ; terminalSysCallHandler(rax,rbx,rcx,rdx,rsi,rdi)

  mov rdi, rsp
	call restoreProcessStack
	mov rsp, rax
	popaq
  mov [0xB8000], byte 'U'
  iretq

.systemCallHandler_inKernel:

  mov r9, qword [temp_rdi]
  mov r8, qword [temp_rsi]
  mov rdx, qword [temp_rcx]
  mov rcx, qword [temp_rdx]
  mov rdi, qword [temp_rax]
  mov rsi, qword [temp_rbx]
  call terminalSysCallHandler ; terminalSysCallHandler(rax,rbx,rcx,rdx,rsi,rdi)

  popaq
  iretq
;--------------------------------------------------------------------------------------------------------


timerTickHandler:
  pushaq
  mov rdi, rsp ;Guardo el stack pointer del proceso y recupero el del kernel
  call restoreKernelStack ;Esta funcion guarda el stack de proceso y recupera el del kernel
  cmp rax, 0x0 ;(en caso de que ya este en el kernel no hace nada y deuvelve 0)
  je .timerTickHandler_inKernel
  mov rsp, rax

  call schedule ;Ejecuta el algoritmo de scheduling (por ahora round robin quantum:1)
  mov al, 0x20
  out 0x20, al ;ACK al master pic

  mov rdi, rsp
	call restoreProcessStack
	mov rsp, rax
	popaq
  iretq

.timerTickHandler_inKernel:
  mov al, 0x20
  out 0x20, al ;ACK al master pic

  popaq
  iretq
;--------------------------------------------------------------------------------------------------------

spuriousInt7Handler:
  pushaq
  mov edi, 0x20
  mov esi, 0x0B
  call outputB ;recibe el registro a escribir (en edi) y el valor a escribir(en esi)
  mov edi, 0x20
  call inputB ;recibe el registro a leer en el registro edi
  and al, 0x80
  cmp al, 0x80
  je .salir ;Si era una spurious interrupt, no hago nada
  mov al, 0x20 ;Si no era, como no me importa IRQ7 simplemente mando
  out 0x20, al ;ACK al master pic
.salir:
  popaq
  iretq

spuriousInt15Handler:
  pushaq
  mov edi, 0xA0
  mov esi, 0x0B
  call outputB ;recibe el registro a escribir (en edi) y el valor a escribir(en esi)
  mov edi, 0xA0
  call inputB ;recibe el registro a leer en el registro edi
  and al, 0x80
  cmp al, 0x80
  je .salir ;Si era una spurious interrupt, no hago nada
  mov al, 0x20 ;Si no era, como no me importa IRQ15 simplemente mando
  out 0xA0, al ;ACK al slave pic
.salir:
  mov al, 0x20 ;Siempre mando
  out 0x20, al ;ACK al master pic
  popaq
  iretq

section .data

temp_rax: dq 0x00
temp_rbx: dq 0x00
temp_rcx: dq 0x00
temp_rdx: dq 0x00
temp_rdi: dq 0x00
temp_rsi: dq 0x00
