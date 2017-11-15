/* chat */
#include <stdint.h>
#include <stdlib.h>
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdio.h"
#include "stdlib/stdio.h"
#include "mqlib/mqlib.h"

int main()
{
	char buf[255];

	mq_open("chat");

	printf("::::: Interprocess chat! :::::\nRead(1) or Write(2)?: ");
	gets(buf);
	if(strcmp("2", buf)==0)
	{
		while(1) {
			printf("escriba su mensaje\n");
			gets(buf);
			mq_send("chat", buf, strlen(buf)+1);
			printf("sent %s\n", buf);
		}
	} else {
		while(1) {
			mq_receive("chat", buf, 255);
			printf("received: %s\n", buf);
		}
	}

	return 0;
}
