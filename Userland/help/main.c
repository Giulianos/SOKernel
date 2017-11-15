/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);

int main()
{
	printf("====================================================================\n");
	printf("::::::::::::::::::::::: Comandos disponibles :::::::::::::::::::::::\n");
	printf("====================================================================\n");
	printf("help             Muestra los comandos disponibles\n");
	printf("ps               Muestra la lista de procesos\n");
	printf("producer         Abre un productor interactivo\n");
	printf("consumer         Abre un consumidor interactivo\n");
	printf("philosophers     Abre el programa 'philosophers'\n");
	printf("chat             Permite chatear entre procesos (demo message queue)\n");
	printf("sample           Demo de sincronizacion y threads\n");
	printf("====================================================================\n");

	return 0;
}
