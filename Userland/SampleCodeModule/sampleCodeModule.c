/* sampleCodeModule.c */
#include <stdint.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"

extern uint64_t systemCall(uint64_t eax, uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi);
extern void putc ( void* p, char c);

static int my_var = 20;

uint64_t * pd = (uint64_t *)0x10000;

int main() {
	int var1;
	int var2;
	//Lets print something through syscalls
	char buffer[40];
	uint64_t * pml4 = (char *)0x2000;
	uint64_t * pdp = (char *)0x3000;
	init_printf(0, putc);
	printf(":::SampleCodeModule:::\n", pml4);
	printf("First PML4 Entry dump(@0x%x): \n", pml4);
	for(int i=0; i<64; i++) {
		printf("%c", (*pml4>>(63-i))&0x1?'1':'0');
	}
	printf("\n");
	printf("First PDP Entry dump(@0x%x): \n", pdp);
	for(int i=0; i<64; i++) {
		printf("%c", (*pdp>>(63-i))&0x1?'1':'0');
	}
	printf("\n");
	printf("PD Entry 10 dump(@0x%x): \n", &(pd[10]));
	for(int i=0; i<64; i++) {
		printf("%c", (pd[10]>>(63-i))&0x1?'1':'0');
	}
	printf("\n");
	printf("PD Entry 255 dump(@0x%x): \n", &(pd[255]));
	for(int i=0; i<64; i++) {
		printf("%c", (pd[255]>>(63-i))&0x1?'1':'0');
	}
	printf("\n");
	printf("my_var=%d is located at %x\n", my_var, &my_var);
	//printf("PDP Base Address[0:16]: %x\n", (*pml4>>12)&0xFFFF);
	//printf("PDP Base Address[0:16]: %x\n", (*pml4>>12)&0xFFFF);

	printf("El stack crece hacia %s\n", (&var1<&var2)?"abajo":"arriba");

	return 0xDEADBEEF;
}
