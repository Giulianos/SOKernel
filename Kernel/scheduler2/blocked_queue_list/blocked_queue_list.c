#include "blocked_queue_list.h"

typedef struct blocked_queue_list_node * blocked_queue_list_node_t;

struct blocked_queue_list_node
{
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
  new_node = (blocked_queue_list_node_t)malloc(sizeof(struct blocked_queue_list_node));

  if(new_node == NULL || new_queue == NULL) {
    k_log("Error creating blocked queue\n");
    return NULL;
  }

  

}

thread_queue_t get_blocked_queue_list(int queue_id)
{

}

int remove_blocked_queue_list(int queue_id)
{

}
