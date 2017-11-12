#include <lib.h>
#include <stdlib.h>
#include "process_list.h"
#include "../scheduler.h"

typedef struct process_list_node * process_list_node_t;
typedef struct process_list * process_list_t;

struct process_list_node
{
  process_t process;
  process_list_node_t next;
  process_list_node_t prev;
};

struct process_list
{
  process_list_node_t first;
  process_list_node_t last;
  int size;
};

static process_t poll_process_list(process_list_t pl);

process_list_t new_process_list()
{
  process_list_t ret = (process_list_t)k_malloc(sizeof(struct process_list));

  if(ret == NULL) {
    #ifdef process_list_DEBUG_MSG
    k_log("Couldn't allocate space for process_list!\n");
    #endif
    return NULL;
  }

  ret->first = ret->last = NULL;
  ret->size = 0;

  return ret;
}

int free_process_list(process_list_t pl)
{
  process_t aux_process;

  while((aux_process=poll_process_list(pl)) != NULL)
  {
    kill_process(aux_process);
  }

  k_free(pl);

  return 1;
}

process_t poll_process_list(process_list_t pl)
{
  process_t ret;

  if(pl->first == NULL)
    return NULL;

  if(pl->first == pl->last) {
    ret = pl->first->process;
    k_free(pl->first);
    pl->first = NULL;
    pl->last = NULL;
  } else {
    ret = pl->first->process;
    pl->first=pl->first->next;
    k_free(pl->first->prev);
    pl->first->prev = NULL;
  }

  pl->size--;

  return ret;
}

int add_process_list(process_list_t pl, process_t process)
{
  process_list_node_t new_node = (process_list_node_t)k_malloc(sizeof(struct process_list_node));

  if(new_node == NULL) {
    #ifdef process_list_DEBUG_MSG
    k_log("Couldn't allocate space for process_list_node!\n");
    #endif
    return -1;
  }

  new_node->process = process;

  if(pl->first == NULL) {
    pl->first = new_node;
    pl->last = new_node;
    new_node->next = NULL;
    new_node->prev = NULL;
  } else {
    pl->last->next = new_node;
    new_node->prev = pl->last;
    new_node->next = NULL;
    pl->last = new_node;
  }

  pl->size++;

  return 1;
}

process_t get_process_list(process_list_t pl, pid_t pid)
{
  process_list_node_t curr;

  if(pl == NULL)
    return NULL;
  if(is_empty_process_list(pl))
    return NULL;

  curr = pl->first;
  do {
    if(curr->process->pid == pid) {
      return curr->process;
    } else {
      curr = curr->next;
    }
  } while(curr != NULL);
  return NULL;
}

int is_empty_process_list(process_list_t pl)
{
  return pl->first == NULL;
}

int remove_process_list(process_list_t pl, process_t process)
{
  process_list_node_t curr;

  if(pl == NULL)
    return -1;
  if(is_empty_process_list(pl))
    return -1;

  curr = pl->first;
  do {
    if(curr->process->pid == process->pid) {
      if(curr == pl->first && curr == pl->last) {
        pl->first = NULL;
        pl->last = NULL;
      } else if(curr == pl->first) {
        pl->first = pl->first->next;
        pl->first->prev = NULL;
      } else if(curr == pl->last) {
        pl->last = pl->last->prev;
        pl->last->next = NULL;
      } else {
        curr->next->prev = curr->prev;
        curr->prev->next = curr->next;
      }
      k_free(curr);
      return 1;
    } else {
      curr = curr->next;
    }
  } while(curr != NULL);
  return -1;
}
