#ifndef PAGEMAP_LIST_H
#define PAGEMAP_LIST_H

  typedef struct pagemap_list_node * pagemap_list_t;

  struct pagemap_list_node
  {
    void * physical;
    void * logical;
    pagemap_list_t next;
  };

#endif
