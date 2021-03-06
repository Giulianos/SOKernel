#include "pagemap_list.h"
#include <stdlib.h>
#include <lib.h>

pagemap_list_t add_pagemap(pagemap_list_t pm_list, void * physical, void * logical)
{
  pagemap_list_t curr = pm_list;
  pagemap_list_t new_node = (pagemap_list_t)k_malloc(sizeof(struct pagemap_list_node));

  if(new_node == NULL)
  {
    #ifdef PAGEMAP_LIST_DEBUG_MSG
    k_log("Error, couldn't add pagemap to list!\n");
    #endif
    return NULL;
  }
  new_node->logical = logical;
  new_node->physical = physical;
  new_node->next = NULL;
  if(pm_list == NULL) {
    return new_node;
  }
  while(curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = new_node;
  return pm_list;
}

pagemap_list_t remove_physical_pagemap(pagemap_list_t pm_list, void * physical)
{
  if(pm_list == NULL)
    return NULL;
  if(pm_list->physical == physical)
    return pm_list->next;
  pm_list->next = remove_physical_pagemap(pm_list->next, physical);
  return pm_list;
}

pagemap_list_t remove_logical_pagemap(pagemap_list_t pm_list, void * logical)
{
  if(pm_list == NULL)
    return NULL;
  if(pm_list->logical == logical)
    return pm_list->next;
  pm_list->next = remove_logical_pagemap(pm_list->next, logical);
  return pm_list;
}

int size_pagemap(pagemap_list_t pm_list)
{
  if(pm_list == NULL)
    return 0;
  return 1 + size_pagemap(pm_list->next);
}

void * get_last_logical(pagemap_list_t pm_list)
{
  pagemap_list_t curr = pm_list;

  if(curr == NULL)
    return NULL;

  while(curr->next != NULL) {
    curr = curr->next;
  }

  return curr->logical;

}

void each_pagemap(pagemap_list_t pm_list, void(*callback)(void * logical, void * phyisical))
{
  pagemap_list_t current = pm_list;

  while(current != NULL) {
    callback(current->logical, current->physical);
    current = current->next;
  }
}
