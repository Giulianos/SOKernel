#ifndef PAGEMAP_LIST_H
#define PAGEMAP_LIST_H

  typedef struct pagemap_list_node * pagemap_list_t;

  struct pagemap_list_node
  {
    void * physical;
    void * logical;
    pagemap_list_t next;
  };

  pagemap_list_t add_pagemap(pagemap_list_t pm_list, void * physical, void * logical);
  pagemap_list_t remove_physical_pagemap(pagemap_list_t pm_list, void * physical);
  pagemap_list_t remove_logical_pagemap(pagemap_list_t pm_list, void * logical);
  void each_pagemap(pagemap_list_t pm_list, void(*callback)(void * logical, void * phyisical));

#endif
