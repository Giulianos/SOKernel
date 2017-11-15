/* consumer */
#include "semlib/semlib.h"
#include "mxlib/mxlib.h"
#include "stdlib/printf.h"
#include "mqlib/mqlib.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

#define BUF_SIZE 5
#define MAX_PRINT_BUF 50

extern void putc ( void* p, char c);

int main()
{
	char print_buf[MAX_PRINT_BUF];
	int salir = 0;

	printf(":::CONSUMER:::\n");
	printf("Presione enter para recibir mensajes del producer o\nsalga con \".salir\"\n");

	mq_open("prodcons_buf");
	mx_create("crit_zone");
	sem_create("buf_size", BUF_SIZE);
	sem_create("prod_count", 0);

	while(!salir) {
		gets(print_buf);
		if(strcmp(print_buf, ".salir") != 0) {
			sem_wait("prod_count");
			mx_lock("crit_zone");
			mq_receive("prodcons_buf", print_buf, MAX_PRINT_BUF);
			mx_unlock("crit_zone");
			sem_signal("buf_size");
			printf("mensaje: %s\n", print_buf);
		} else {
			salir = 1;
		}
	}

	return 0;
}
