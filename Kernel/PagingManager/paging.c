#include <stdint.h>

#define USERLAND_LOGIC_PAGE 255

typedef uint64_t PDEntry_t;

PDEntry_t * PDAddr = (PDEntry_t *)0x10000;
uint64_t userlandPhysicalPage = 10;

void mapUserspace(uint64_t pageAddr)
{
  // Como inicialmente tenemos mapeo identidad, obtenemos la direccion fisica de la pagina 10 buscando la entrada
  // numero 10 de la PD.
  uint64_t pageNum = pageAddr/0x200000;
  PDAddr[USERLAND_LOGIC_PAGE] = PDAddr[pageNum];
}
