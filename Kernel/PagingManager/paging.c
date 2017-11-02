#include <stdint.h>
#include <lib.h>
#include "paging.h"

#define USERLAND_LOGIC_PAGE 0x1FE00000

extern void reloadCR3();

typedef uint64_t PDEntry_t;

PDEntry_t * PDAddr = (PDEntry_t *)0x10000;
uint64_t userlandPhysicalPage = 10;

void mapProcess(void * pageAddr)
{
  mapPhysical((void *)USERLAND_LOGIC_PAGE, pageAddr);
}

void mapPhysical(void * logic, void * physical)
{
  unsigned long logicPageNum = (unsigned long)((unsigned long)logic/0x200000);
  unsigned long physicalPageNum = (unsigned long)((unsigned long)physical/0x200000);

  PDAddr[logicPageNum] = PDAddr[physicalPageNum];
	reloadCR3();
}

void * getLogicalUserlandPage()
{
	return (void *)USERLAND_LOGIC_PAGE;
}
