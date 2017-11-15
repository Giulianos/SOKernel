/* sampleCodeModule.c */
#include <stdint.h>
#include <stdlib.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"
#include "stdlib/stdlib.h"
#include "mxlib/mxlib.h"
#include "semlib/semlib.h"
#include "threadlib/threadlib.h"

extern void putc ( void* p, char c);

typedef struct args
{
	int thread_num;
	int * arg1;
} args_t;

void impresor(args_t * args)
{
	mx_lock("mutex_sample"); //Begin critic zone
	printf("Soy el thread %d, esto esta en el heap: %d\n", args->thread_num, args->arg1);
	sem_signal("sem_sample");
	mx_unlock("mutex_sample"); //End critic zone
}

int main()
{
	int i = 0;
	args_t * args;
	int * variable_heap = (int *)malloc(sizeof(int));

	mx_create("mutex_sample");
	sem_create("sem_sample", 0);

	*variable_heap = 10;

	printf("Soy sample, voy a crear 4 threads\n");

	for(; i < 4; i++) {
		args = (args_t *)malloc(sizeof(args_t));
		args->thread_num = i;
		args->arg1 = variable_heap;
		new_thread(impresor, (void *)args);
	}

	for(i = 0; i < 4; i++) {
		sem_wait("sem_sample");
	}
	return 0;
}
