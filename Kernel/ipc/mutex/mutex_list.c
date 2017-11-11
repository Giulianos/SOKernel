#include "mutex.h"
#include <lib.h>
#include <stdlib.h>

typedef struct mutex_node * mutex_node_t;

struct mutex_node
{
 mutex_t mx;
 mutex_node_t next;
 mutex_node_t prev;
};

static mutex_node_t first = NULL;
static mutex_node_t last = NULL;

mutex_t get_recursive_mutex(const char * id_str, mutex_node_t node);
void remove_recursive_mutex(const char * id_str, mutex_node_t node);

int add_mutex(mutex_t mx)
{
  mutex_node_t new_node = (mutex_node_t)k_malloc(sizeof(struct mutex_node));

  if(new_node == NULL) {
    k_log("Couldn't allocate space for mutex!\n");
    return -1;
  }

  new_node->mx = mx;
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

mutex_t get_mutex(const char * id_str)
{
  return get_recursive_mutex(id_str, first);
}

mutex_t get_recursive_mutex(const char * id_str, mutex_node_t node)
{
  if(node == NULL)
    return NULL;

  if(k_strcmp(id_str, get_id_str_mutex(node->mx)) == 0)
    return node->mx;

  return get_recursive_mutex(id_str, node->next);
}

void remove_mutex(const char * id_str)
{
  remove_recursive_mutex(id_str, first);
}

void remove_recursive_mutex(const char * id_str, mutex_node_t node)
{
  mutex_node_t aux;

  if(node == NULL)
    return;

  if(k_strcmp(id_str, get_id_str_mutex(node->mx)) == 0) {
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
    free_mutex(aux->mx);
    k_free(aux);
    return;
  }

  remove_recursive_mutex(id_str, node->next);
}
