#ifndef PHILOGUI_H
#define PHILOGUI_H

#include "philosophers.h"

void render();
void setPhiloState(int philo, State state);
void setForkState(int fork, int owner);

#endif