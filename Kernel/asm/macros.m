
%macro pushaq 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popaq 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro ctxswitchkernel 0
	pushaq
	mov rdi, rsp ;Guardo el stack pointer del proceso y recupero el del kernel
	call restoreKernelStack ;Esta funcion guarda el stack de proceso y recupera el del kernel
	mov rsp, rax
%endmacro

%macro ctxswitchuser 0
	mov rdi, rsp
	call restoreProcessStack
	mov rsp, rax
	popaq
%endmacro
