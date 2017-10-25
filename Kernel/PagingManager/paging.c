#include <stdint.h>
#include <lib.h>

#define USERLAND_LOGIC_PAGE 255

typedef uint64_t PDEntry_t;

PDEntry_t * PDAddr = (PDEntry_t *)0x10000;
uint64_t userlandPhysicalPage = 10;

void mapProcess(uint64_t pageAddr)
{
  uint64_t pageNum = pageAddr/0x200000;
  /* Copio la entrada correspondiente a la pagina fisica recibida
     en la entrada correspondiente la pagina logica donde corren
     los procesos.
  */
  PDAddr[USERLAND_LOGIC_PAGE] = PDAddr[pageNum];
	reloadCR3();
}

uint64_t getLogicalUserlandPage()
{
	return USERLAND_LOGIC_PAGE*0x200000;
}
