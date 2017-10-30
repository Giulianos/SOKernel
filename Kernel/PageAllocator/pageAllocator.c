#include "pageAllocator.h"

//0x5020: direccion donde está la cantidad de RAM de la computadora
static uint32_t * ramAmount = ((uint32_t *) 0x5020);
static uint8_t pageMap[MAX_PHYSICAL_PAGES];
static uint64_t allocatedPages[CONCURRENT_ALLOCATED_PAGES];
uint64_t allocateSinglePage()
{
  uint16_t i;
  for(i = LAST_KERNEL_PAGE+1; i < MAX_PHYSICAL_PAGES && i < *(ramAmount)/PAGE_SIZE; i++)
  {
    if(pageMap[i] != USED_PAGE)
    {
      pageMap[i] = USED_PAGE;
      return i*PAGE_SIZE*0x100000; // i * 2MB
    }
  }
}

uint64_t pageSize()
{
	return PAGE_SIZE * 0x100000;
}

uint64_t allocatePage()
{
  static uint8_t allocate = 1;
  static uint8_t currentPage = 0;
  uint8_t ret, i;

  if(currentPage == CONCURRENT_ALLOCATED_PAGES)
    allocate = 1;
  if(allocate == 1)
  {
    for(i=0; i<CONCURRENT_ALLOCATED_PAGES; i++)
    {
      allocatedPages[i] = allocateSinglePage();
    }
    allocate = 0;
    currentPage = 0;
  }
  ret = currentPage;
  currentPage++;
  return allocatedPages[ret];
}

void freePage(uint64_t pageAddress)
{
  pageMap[pageAddress/(PAGE_SIZE*0x100000)] = !USED_PAGE;
}
