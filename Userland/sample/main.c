/* sampleCodeModule.c */
#include <stdint.h>
#include <stdlib.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"
#include "threadlib/threadlib.h"

extern void putc ( void* p, char c);

static thread_num = 0;

void impresor(int i)
{
	printf("Soy el thread %d\n", i);
}

int main()
{
	int i = 0;
	init_printf(0, putc);

	printf("Soy sample, voy a crear 4 threads\n");

	for(; i < 4; i++)
		new_thread(impresor, (void *)i);

	while(1){}

}
