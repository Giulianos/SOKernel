/* producer */
#include "semlib/semlib.h"
#include "mxlib/mxlib.h"
#include "mqlib/mqlib.h"
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"

#define BUF_SIZE 5
#define MAX_IN_BUF 50

extern void putc ( void* p, char c);

int main()
{
	char in_buf[MAX_IN_BUF];
	int salir = 0;

	printf(":::PRODUCER:::\n");
	printf("Genere mensajes para enviar al consumer o\nsalga con \".salir\"\n");

	mq_open("prodcons_buf");
	mx_create("crit_zone");
	sem_create("buf_size", BUF_SIZE);
	sem_create("prod_count", 0);

	while(!salir) {
		gets(in_buf);
		if(strcmp(in_buf, ".salir") != 0) {
			sem_wait("buf_size");
			mx_lock("crit_zone");
			mq_send("prodcons_buf", in_buf, strlen(in_buf)+1);
			mx_unlock("crit_zone");
			sem_signal("prod_count");
		} else {
			salir = 1;
		}
	}

	return 0;
}
