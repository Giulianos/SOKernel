#ifndef _PAGING_H
#define _PAGING_H

  #include <datastructures/pagemap_list.h>

  void map_process(void * pageAddr);
  void map_physical(void * logic, void * physical);
  void map_pagemap_list(pagemap_list_t pm_list);
  void * get_logical_userland_page();

#endif
