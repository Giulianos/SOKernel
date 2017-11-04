#include <lib.h>
#include <stdlib.h>
#include "thread_queue.h"

typedef struct thread_queue_node * thread_queue_node_t;
typedef struct thread_queue * thread_queue_t;

struct thread_queue_node
{
  thread_t thread;
  void * extra_info;
  thread_queue_node_t next;
  thread_queue_node_t prev;
};

struct thread_queue
{
  thread_queue_node_t first;
  thread_queue_node_t last;
  int size;
};

thread_queue_t new_thread_queue()
{
  thread_queue_t ret = (thread_queue_t)k_malloc(sizeof(struct thread_queue));

  if(ret == NULL) {
    k_log("Couldn't allocate space for thread_queue!\n");
    return NULL;
  }

  ret->first = ret->last = NULL;
  ret->size = 0;

  return ret;
}

int free_thread_queue(thread_queue_t tq)
{
  return 1;
}

int offer_thread_queue(thread_queue_t tq, thread_t thread, void * extra_info)
{
  thread_queue_node_t new_node = (thread_queue_node_t)k_malloc(sizeof(struct thread_queue_node));

  if(new_node == NULL) {
    k_log("Couldn't allocate space for thread_queue_node!\n");
    return -1;
  }

  new_node->thread = thread;
  new_node->extra_info = extra_info;

  if(tq->first == NULL) {
    tq->first = new_node;
    tq->last = new_node;
    new_node->next = NULL;
    new_node->prev = NULL;
  } else {
    tq->last->next = new_node;
    new_node->prev = tq->last;
    new_node->next = NULL;
    tq->last = new_node;
  }

  tq->size++;

  return 1;
}

void * peek_extra_info_thread_queue(thread_queue_t tq)
{
  if(tq->first == NULL)
    return NULL;
  return tq->first->extra_info;
}

thread_t poll_thread_queue(thread_queue_t tq)
{
  thread_t ret;

  if(tq->first == NULL)
    return NULL;

  if(tq->first == tq->last) {
    ret = tq->first->thread;
    k_free(tq->first);
    tq->first = NULL;
    tq->last = NULL;
  } else {
    ret = tq->first->thread;
    tq->first=tq->first->next;
    k_free(tq->first->prev);
    tq->first->prev = NULL;
  }

  tq->size++;

  return ret;
}

thread_t peek_thread_queue(thread_queue_t tq)
{
  if(tq->first == NULL)
    return NULL;
  return tq->first->thread;
}
