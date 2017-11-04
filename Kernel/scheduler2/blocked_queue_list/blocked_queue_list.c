#include "blocked_queue_list.h"
#include <stdlib.h>
#include <lib.h>

typedef struct blocked_queue_list_node * blocked_queue_list_node_t;

struct blocked_queue_list_node
{
  int id;
  thread_queue_t queue;
  blocked_queue_list_node_t next;
};

static blocked_queue_list_node_t first = NULL;
static blocked_queue_list_node_t last = NULL;
static int last_id = 0;

int new_blocked_queue()
{
  blocked_queue_list_node_t new_node;
  thread_queue_t new_queue;

  new_queue = new_thread_queue();
  new_node = (blocked_queue_list_node_t)k_malloc(sizeof(struct blocked_queue_list_node));

  if(new_node == NULL || new_queue == NULL) {
    k_log("Error creating blocked queue\n");
    return -1;
  }

  new_node->queue = new_queue;
  new_node->id = last_id++;
  new_node->next = NULL;
  if(first == NULL) {
    first = new_node;
    last = new_node;
  } else {
    last->next = new_node;
    last = new_node;
  }

  return new_node->id;
}

thread_queue_t get_blocked_queue(int queue_id)
{
  blocked_queue_list_node_t curr = first;

  while(curr != NULL) {
    if(curr->id == queue_id)
      return curr->queue;
    curr = curr->next;
  }
  return NULL;
}

int remove_blocked_queue(int queue_id)
{
  blocked_queue_list_node_t curr = first;
  blocked_queue_list_node_t prev = NULL;

  while(curr != NULL) {
    if(curr->id == queue_id) {
      if(prev == NULL) {
        first = curr->next;
      } else {
        prev->next = curr->next;
      }
      free_thread_queue(curr->queue);
      k_free(curr);
      return 1;
    }
  }

  return -1;
}
