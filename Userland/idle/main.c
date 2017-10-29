/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void halt();
extern void putc ( void* p, char c);

int main() {
	init_printf(0, putc);
	while(1) {
		halt();
	}
}
