#include "message_queue.h"
#include <lib.h>
#include <stdlib.h>

typedef struct message_queue_node * message_queue_node_t;

struct message_queue_node
{
  message_queue_t mq;
  message_queue_node_t next;
  message_queue_node_t prev;
};

static message_queue_node_t first = NULL;
static message_queue_node_t last = NULL;

message_queue_t get_recursive_message_queue(const char * id_str, message_queue_node_t node);
void remove_recursive_message_queue(const char * id_str, message_queue_node_t node);

int add_message_queue(message_queue_t mq)
{
  message_queue_node_t new_node = (message_queue_node_t)k_malloc(sizeof(struct message_queue_node));

  if(new_node == NULL) {
    k_log("Couldn't allocate space for message queue!\n");
    return -1;
  }

  new_node->mq = mq;
  new_node->next = NULL;
  new_node->prev = NULL;
  if(first==NULL) {
    first = new_node;
    last = new_node;
  } else {
    new_node->prev = last;
    last->next = new_node;
    last = new_node;
  }

  return 1;
}

message_queue_t get_message_queue(const char * id_str)
{
  return get_recursive_message_queue(id_str, first);
}

message_queue_t get_recursive_message_queue(const char * id_str, message_queue_node_t node)
{
  if(node == NULL)
    return NULL;

  if(k_strcmp(id_str, get_id_str_message_queue(node->mq)) == 0)
    return node->mq;

  return get_recursive_message_queue(id_str, node->next);
}

void remove_message_queue(const char * id_str)
{
  remove_recursive_message_queue(id_str, first);
}

void remove_recursive_message_queue(const char * id_str, message_queue_node_t node)
{
  message_queue_node_t aux;

  if(node == NULL)
    return;

  if(k_strcmp(id_str, get_id_str_message_queue(node->mq)) == 0) {
    aux = node;
    if(node == first && node == last) {
      first = NULL;
      last = NULL;
    } else if(node == first) {
      first = node->next;
      node->next->prev = NULL;
    } else if(node == last) {
      last = node->prev;
      node->prev->next = NULL;
    } else {
      node->prev->next = node->next;
      node->next->prev = node->prev;
    }
    free_message_queue(aux->mq);
    k_free(aux);
    return;
  }

  remove_recursive_message_queue(id_str, node->next);
}
