#include <stdint.h>
#include <lib.h>
#include "paging.h"

#define USERLAND_LOGIC_PAGE 0x1FE00000
#define USERLAND_STACK_LOGIC_PAGE 0x20000000
#define USERLAND_HEAP_BASE_PAGE 0x20200000

extern void reloadCR3();

typedef uint64_t PDEntry_t;
typedef uint64_t PDPEntry_t;

PDPEntry_t * PDPAddr = (PDPEntry_t *)0x3000;
uint64_t userlandPhysicalPage = 10;

void map_process(void * pageAddr)
{
  map_physical((void *)USERLAND_LOGIC_PAGE, pageAddr);
  reloadCR3();
}

uint64_t get_pdp_offset(void * addr)
{
  return ((uint64_t)addr >> 30) & 0x1FF;
}

uint64_t get_pd_offset(void * addr)
{
  return ((uint64_t)addr >> 21) & 0x1FF;
}

void map_physical(void * logic, void * physical)
{
  PDEntry_t * PDAddrLog = (PDEntry_t *)(PDPAddr[get_pdp_offset(logic)] & 0xFFFFFFFFFF000);
  PDEntry_t * PDAddrPhys = (PDEntry_t *)(PDPAddr[get_pdp_offset(physical)] & 0xFFFFFFFFFF000);

  PDAddrLog[get_pd_offset(logic)] = PDAddrPhys[get_pd_offset(physical)];
}

void map_physical_reload(void * logic, void * physical)
{
  map_physical(logic, physical);

  reloadCR3();
}

void map_pagemap_list(pagemap_list_t pm_list)
{
  if(pm_list == NULL)
    return;
  each_pagemap(pm_list, map_physical);
  reloadCR3();
}

void * get_logical_userland_page()
{
	return (void *)USERLAND_LOGIC_PAGE;
}

void * get_logical_userland_stack_page()
{
	return (void *)USERLAND_STACK_LOGIC_PAGE;
}

void * get_logical_userland_heap_base_page()
{
  return (void *)USERLAND_HEAP_BASE_PAGE;
}

void * translate_addr_page(void * addr, void * page)
{
  uint64_t addr_page= (uint64_t)addr;

  addr_page = (addr_page >> 21) << 21;
  return (void *)((uint64_t)addr - addr_page + (uint64_t)page);
}
