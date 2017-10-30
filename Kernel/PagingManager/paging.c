#include <stdint.h>
#include <lib.h>
#include "paging.h"

#define USERLAND_LOGIC_PAGE 0x1FE00000

typedef uint64_t PDEntry_t;

PDEntry_t * PDAddr = (PDEntry_t *)0x10000;
uint64_t userlandPhysicalPage = 10;

void mapProcess(uint64_t pageAddr)
{
  mapPhysical(USERLAND_LOGIC_PAGE, pageAddr);
}

void mapPhysical(uint64_t logic, uint64_t physical)
{
  uint64_t logicPageNum = logic/0x200000;
  uint64_t physicalPageNum = physical/0x200000;

  PDAddr[logicPageNum] = PDAddr[physicalPageNum];
	reloadCR3();
}

uint64_t getLogicalUserlandPage()
{
	return USERLAND_LOGIC_PAGE;
}
