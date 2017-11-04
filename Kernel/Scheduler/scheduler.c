#include "thread_cqueue/thread_cqueue.h"
#include "blocked_queue_list/blocked_queue_list.h"
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
    #ifdef SCHEDULER_DEBUG_MSG
    k_log("Couldn't allocate space for ready_queue\n");
    #endif
    return -1;
  }
  current_tid_scheduler = 0;
  current_pid_scheduler = 0;
  return 1;
}

int add_scheduler(thread_t thread)
{
  /* If the thread has tid=-1 it means it was never scheduled */
  if(thread->tid < 0)
    thread->tid = get_new_tid_scheduler();
  if(offer_thread_cqueue(ready_queue_scheduler, thread)>0) {
    #ifdef SCHEDULER_DEBUG_MSG
    k_log("Added thread with tid:%d to the scheduler\n", thread->tid);
    #endif
    return 1;
  }
  return -1;
}

void schedule_scheduler()
{
  rotate_thread_cqueue(ready_queue_scheduler);
  current_thread_scheduler = peek_thread_cqueue(ready_queue_scheduler);
  mapProcess(current_thread_scheduler->process->code);
  #ifdef SCHEDULER_DEBUG_MSG
  k_log("Scheduled! Next thread has tid:%d\n", current_thread_scheduler->tid);
  #endif
}

int get_new_tid_scheduler()
{
  return current_tid_scheduler++;
}

int get_new_pid_scheduler()
{
  return current_pid_scheduler++;
}

thread_t current_thread()
{
  return current_thread_scheduler;
}


/* ---------------- Thread-blocking mechanism implementation ---------------- */

int create_blocked_queue()
{
  return new_blocked_queue();
}

int free_blocked_queue()
{
  return free_blocked_queue();
}

int block_thread(thread_t thread, int queue, void * extra_info)
{
  thread_queue_t blocked_queue = get_blocked_queue(queue);

  /* First we check if the blocked queue exists */
  if(blocked_queue == NULL)
  {
    #ifdef SCHEDULER_DEBUG_MSG
    k_log("%d couldn't be blocked because the queue doesn't exist!\n", thread->tid);
    #endif
    return -1;
  }

  /* We add the thread to the blocked queue and remove it from the
     ready queue if it could be added */
  if(offer_thread_queue(blocked_queue, thread, extra_info)>0)
  {
    remove_thread_cqueue(ready_queue_scheduler, thread);
    schedule_scheduler();
    return 1;
  }
  #ifdef SCHEDULER_DEBUG_MSG
  k_log("%d couldn't be blocked!\n", thread->tid);
  #endif
  return -1;
}

/* This function polls a thread from the queue passed, unblocks it and
   executes the callback function with the extra_info stored in the queue. */
int unblock_from_queue_thread(int queue, void(*callback)(void *))
{
  thread_queue_t blocked_queue = get_blocked_queue(queue);
  thread_t unblocked_thread;
  void * extra_info;

  /* First we check if the blocked queue exists */
  if(blocked_queue == NULL)
  {
    #ifdef SCHEDULER_DEBUG_MSG
    k_log("Couldn't unblock thread because the queue doesn't exist!\n");
    #endif
    return -1;
  }

  extra_info = peek_extra_info_thread_queue(blocked_queue);
  callback(extra_info);
  unblocked_thread = poll_thread_queue(blocked_queue);

  if(add_scheduler(unblocked_thread)>0) {
    return 1;
  }

  #ifdef SCHEDULER_DEBUG_MSG
  k_log("%d couldn't be unblocked!\n", unblocked_thread->tid);
  #endif
  return -1;
}
