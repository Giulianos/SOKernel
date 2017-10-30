#ifndef TTY_LOCKED_QUEUE_H
#define TTY_LOCKED_QUEUE_H

#include <stdlib.h>

#define MAX_LOCKED_PROC 100

typedef struct {
  uint64_t pid;
  size_t count;
  char * buffer;
} lockedProcess_t;

typedef struct {
  unsigned f;
  unsigned l;
  lockedProcess_t queue[MAX_LOCKED_PROC + 1];
} lockedQueue_t;

void offerLockedProcess(lockedQueue_t * q, lockedProcess_t p);
lockedProcess_t pollLockedProcess(lockedQueue_t * q);
char noLockedProcesses(lockedQueue_t * q);

#endif
