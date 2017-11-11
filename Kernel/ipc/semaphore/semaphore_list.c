#include "semaphore.h"
#include <lib.h>
#include <stdlib.h>

typedef struct semaphore_node * semaphore_node_t;

struct semaphore_node
{
 semaphore_t sem;
 semaphore_node_t next;
 semaphore_node_t prev;
};

static semaphore_node_t first = NULL;
static semaphore_node_t last = NULL;

semaphore_t get_recursive_semaphore(const char * id_str, semaphore_node_t node);
void remove_recursive_semaphore(const char * id_str, semaphore_node_t node);

int add_semaphore(semaphore_t sem)
{
  semaphore_node_t new_node = (semaphore_node_t)k_malloc(sizeof(struct semaphore_node));

  if(new_node == NULL) {
    k_log("Couldn't allocate space for semaphore!\n");
    return -1;
  }

  new_node->sem = sem;
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


semaphore_t get_semaphore(const char * id_str)
{
  return get_recursive_semaphore(id_str, first);
}

semaphore_t get_recursive_semaphore(const char * id_str, semaphore_node_t node)
{
  if(node == NULL)
    return NULL;

  if(k_strcmp(id_str, get_id_str_semaphore(node->sem)) == 0)
    return node->sem;

  return get_recursive_semaphore(id_str, node->next);
}

void remove_semaphore(const char * id_str)
{
  remove_recursive_semaphore(id_str, first);
}

void remove_recursive_semaphore(const char * id_str, semaphore_node_t node)
{
  semaphore_node_t aux;

  if(node == NULL)
    return;

  if(k_strcmp(id_str, get_id_str_semaphore(node->sem)) == 0) {
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
    free_semaphore(aux->sem);
    k_free(aux);
    return;
  }

  remove_recursive_semaphore(id_str, node->next);
}
