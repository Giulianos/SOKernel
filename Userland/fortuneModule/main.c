/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/stdio.h"
#include "stdlib/stdlib.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);

int main()
{
	char buffer[LINE_LEN];
	uint64_t len;
	char * str[] = {"Break into jail and claim police brutality.\n","Never be led astray onto the path of virtue\n", "You will forget that you ever knew me.\n", "Your society will be sought by people of taste and refinement.\n", "You will be honored for contributing your time and skill to a worthy cause.\n", "Expect the worst, it's the least you can do.\n"};

	init_printf(0, putc);
	printf("Vienvenido a al adibinador de la fortuna! Este mensaje es muy largo, y puede ser muy molesto al usuario. Tal vez deberiamos acortarlo?\n");
	printf("Cual es tu nonbre?: ");
	getline(buffer);
	len = strlen(buffer);
	printf("\nTu fortuna es: %s\n", str[len%6]);
	return 0xDEADBEEF;
}
