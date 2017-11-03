#include <lib.h>
#include <stdlib.h>
#include "thread_cqueue.h"

typedef struct thread_cqueue_node * thread_cqueue_node_t;
typedef struct thread_cqueue * thread_cqueue_t;

struct thread_cqueue_node
{
  thread_t thread;
  thread_cqueue_node_t next;
  thread_cqueue_node_t prev;
};

struct thread_cqueue
{
  thread_cqueue_node_t first;
  thread_cqueue_node_t last;
  int size;
};

thread_cqueue_t new_thread_cqueue()
{
  thread_cqueue_t ret = (thread_cqueue_t)k_malloc(sizeof(struct thread_cqueue));

  if(ret == NULL) {
    k_log("Couldn't allocate space for thread_cqueue!\n");
    return NULL;
  }

  ret->first = ret->last = NULL;
  ret->size = 0;

  return ret;
}

int offer_thread_cqueue(thread_cqueue_t tq, thread_t thread)
{
  thread_cqueue_node_t new_node = (thread_cqueue_node_t)k_malloc(sizeof(struct thread_cqueue_node));

  if(new_node == NULL) {
    k_log("Couldn't allocate space for thread_cqueue_node!\n");
    return -1;
  }

  new_node->thread = thread;

  if(tq->first == NULL) {
    tq->first = new_node;
    tq->last = new_node;
    new_node->next = new_node;
    new_node->prev = new_node;
  } else {
    tq->last->next = new_node;
    tq->first->prev = new_node;
    new_node->prev = tq->last;
    new_node->next = tq->first;
    tq->last = new_node;
  }

  tq->size++;

  return 1;
}

thread_t poll_thread_cqueue(thread_cqueue_t tq)
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
    tq->first->prev = tq->last;
    tq->last->next = tq->first;
  }

  tq->size--;

  return ret;
}

thread_t peek_thread_cqueue(thread_cqueue_t tq)
{
  if(tq->first == NULL)
    return NULL;
  return tq->first->thread;
}

int rotate_thread_cqueue(thread_cqueue_t tq)
{
  if(tq->first == NULL)
    return -1;

  tq->first = tq->first->next;
  tq->last = tq->first->prev;

  return 1;
}
