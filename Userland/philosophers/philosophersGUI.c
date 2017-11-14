#include "semlib/semlib.h"
#include "mxlib/mxlib.h"
#include "mqlib/mqlib.h"
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdlib.h"
#include "philosophersGUI.h"

#define philosopherCount 2
#define philosopherMax 40

extern int philosophersQuantity;

char * stateStrings[3] = { "Hungry", "Thinking", "Eating" };
State philoState[philosopherMax];
int forkState[philosopherMax];

void render() {
	for(int i = 0; i < philosophersQuantity; i++) {
		printf("Philosopher %d: %s\n", i, stateStrings[philoState[i]]);
		printf("Fork - ");

		if (forkState[i] == -1)
			printf("Free\n");
		else
			printf("Owner %d\n", forkState[forkState[i]]);
	}

	printf("\n\n");
}

void setPhiloState(int philo, State state) {
	philoState[philo] = state;
}

void setForkState(int fork, int owner) {
	forkState[fork] = owner;
}
