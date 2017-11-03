#include "thread_cqueue/thread_cqueue.h"
#include "process.h"
#include "thread.h"
#include "../PagingManager/paging.h"
#include <stdlib.h>
#include <lib.h>

static thread_cqueue_t ready_queue_scheduler;
static thread_t current_thread_scheduler;
static int current_tid_scheduler;
static int current_pid_scheduler;

int get_new_pid_scheduler();
int get_new_tid_scheduler();

int init_scheduler()
{
  ready_queue_scheduler = new_thread_cqueue();
  if(ready_queue_scheduler == NULL) {
    k_log("Couldn't allocate space for ready_queue\n");
    return -1;
  }
  current_tid_scheduler = 0;
  current_pid_scheduler = 0;
  return 1;
}

int get_new_tid_scheduler()
{
  return current_tid_scheduler++;
}

int get_new_pid_scheduler()
{
  return current_pid_scheduler++;
}

void add_scheduler(thread_t thread)
{
  thread->tid = get_new_tid_scheduler();
  k_log("Added thread with tid:%d to the scheduler\n", thread->tid);
  offer_thread_cqueue(ready_queue_scheduler, thread);
}

void schedule_scheduler()
{
  rotate_thread_cqueue(ready_queue_scheduler);
  current_thread_scheduler = peek_thread_cqueue(ready_queue_scheduler);
  mapProcess(current_thread_scheduler->process->code);
  k_log("Scheduled! Next thread has tid:%d\n", current_thread_scheduler->tid);
}



thread_t current_thread()
{
  return current_thread_scheduler;
}
