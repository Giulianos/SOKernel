#include <stdint.h>
#include "lockedQueue.h"

static lockedProcess_t dummy = {0,0,0};

void offerLockedProcess(lockedQueue_t * q, lockedProcess_t p)
{
  if(q->l == MAX_LOCKED_PROC) {
    if(q->f == 0)
      return;
    q->queue[0] = p;
    q->l = 1;
  }
  else
  {
    q->queue[q->l] = p;
    q->l++;
  }
}

lockedProcess_t pollLockedProcess(lockedQueue_t * q)
{
  if(q->f == q->l)
    return dummy;
  if(q->f == MAX_LOCKED_PROC) {
    if(q->l == 0) {
      q->f = 0;
      return dummy;
    }
    q->f = 1;
  }
  else
    q->f++;
  return q->queue[q->f-1];
}

char noLockedProcesses(lockedQueue_t * q)
{
  return ((q->f == q->l)||(q->f == MAX_LOCKED_PROC && q->l == 0))?1:0;
}
