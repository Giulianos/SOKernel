#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

typedef enum {
	Hungry = 0,
	Thinking = 1,
	Eating = 2
} State;

void decrementPhilosophers();
void incrementPhilosophers();

#endif
