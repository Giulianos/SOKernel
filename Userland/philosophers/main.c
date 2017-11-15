/* philosophers */
#include "semlib/semlib.h"
#include "mxlib/mxlib.h"
#include "mqlib/mqlib.h"
#include "stdlib/printf.h"
#include "stdlib/string.h"
#include "stdlib/stdlib.h"
#include "philosophers.h"
#include "philosophersGUI.h"

#define philosopherCount 2
#define philosopherMax 40

int left(int i);
int right(int i);
void * philosopher(void * id);
void takeForks(int id);
void putForks(int id);
void test(int i);
int randRange(int min, int max);

State state[philosopherMax];
char philoExists[philosopherMax];

char * mutex = "philo_crit_zone";
char semaphores[10][philosopherMax];
int philosopherId[philosopherMax];
int	philosophersQuantity = philosopherCount;

void initSemNames()
{
	char * auxStr;

	for(int i = 0; i < philosopherMax; i++) {
		philoExists[i] = 1;
		strncpy(semaphores[i], "philo_000", 10);
		semaphores[i][8] = i%10 + '0';
		semaphores[i][7] = (i%100)/10 + '0';
		semaphores[i][6] = (i%1000)/100 + '0';
	}
}

static void sleep(int time)
{
	unsigned long i = time * 2000;

	while(i--){}
}

void * philosopher(void * id) {
	while(philoExists[*(int*)id]) {
		//Think
		//sleep(10);
		sleep(randRange(5, 10));

		takeForks(*(int*)id);

		//Eat
		//sleep(10);
		sleep(randRange(5, 10));

		putForks(*(int*)id);
	}
	mx_close(semaphores[*(int*)id]);
}

void takeForks(int id) {
	mx_lock(mutex);				//Crit zone

	//Set state
	state[id] = Hungry;
	setPhiloState(id, Hungry);
	render();

	test(id);								//Try to acquire forks
	mx_unlock(mutex);			//Crit zone exit
	mx_lock(semaphores[id]);	//Locks if forks not acquired
}

void putForks(int id) {
	mx_lock(mutex);				//Crit zone

	//Set state
	state[id] = Thinking;
	//Think and release forks
	setPhiloState(id, Thinking);
	setForkState(left(id), -1);
	setForkState(id, -1);
	render();

	test(left(id));							//Try to acquire forks for left
	test(right(id));						//Try to acquire forks for right
	mx_unlock(mutex);			//Crit zone exit
}

void test(int id) {
	if (state[id] == Hungry &&				//Philosopher is hungry
		state[left(id)] != Eating &&		//Both philosophers at
		state[right(id)] != Eating) {		//left and right are not eating

		//Set state
		state[id] = Eating;					//Philosopher can eat!
		//Take forks and eat
		setPhiloState(id, Eating);
		setForkState(left(id), id);
		setForkState(id, id);
		render();

		mx_unlock(semaphores[id]);	//Forks acquired, unlock
	}
}

int main(int argc, char ** argv) {
	//Setup
	mx_create(mutex);
	initSemNames();

	for (int i = 0; i < philosopherCount; i++) {
		mx_create(semaphores[i]);
		mx_lock(semaphores[i]);		//Philosophers start not having
	}											//ownership of the forks

	for (int i = 0; i < philosopherCount; i++) {
		philosopherId[i] = i;
		state[i] = Thinking;
		new_thread(philosopher, &philosopherId[i]);
	}

	printf("running\n");

	while(1) {
		switch(getchar()) {
			case 'd':
			case 'D': decrementPhilosophers(); break;
			case 'i':
			case 'I': incrementPhilosophers(); break;
			case 'q':
			case 'Q': _exit(0);
		}
	}

	return 0;
}

void decrementPhilosophers()
{
	if (philosophersQuantity <= 1) {
		printf("No puede haber menos filosofos!\n");
		return;
	}
	philoExists[philosophersQuantity-1] = 0;
	philosophersQuantity--;
}

void incrementPhilosophers()
{

	if(philosophersQuantity < philosopherMax) {

		mx_create(semaphores[philosophersQuantity]);
		mx_unlock(semaphores[philosophersQuantity]);
		mx_lock(semaphores[philosophersQuantity]);

		philosopherId[philosophersQuantity] = philosophersQuantity;
		state[philosophersQuantity] = Thinking;
		philoExists[philosophersQuantity] = 1;

		new_thread(philosopher, &philosopherId[philosophersQuantity]);

		philosophersQuantity++;

	} else {
		printf("No puede haber mas filosofos!\n");
	}
	return;
}

int left(int i) {
	return (i + philosophersQuantity - 1) % philosophersQuantity;
}

int right(int i) {
	return (i + 1) % philosophersQuantity;
}

int randRange(int min, int max) {
	return rand() % (max - min) + min;
}
