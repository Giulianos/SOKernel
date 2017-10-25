global cpuVendor
global sti
global cli
global reloadCR3
global switchToProcess
global getSampleRFLAGS

extern restoreKernelStack
extern restoreProcessStack
extern schedule
extern switchContextState

%include "./asm/macros.m"

section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

sti:
	sti
	ret

cli:
	cli
	ret

reloadCR3:
	mov rax, cr3
	mov cr3, rax
	ret

getSampleRFLAGS:
	pushfq
  pop rax
	ret

switchToProcess:
	mov rdi, rsp
	call restoreProcessStack
	mov rsp, rax
	popaq
	iretq
