#include <stdint.h>

#define USERLAND_LOGIC_PAGE 255

typedef uint64_t PDEntry_t;

PDEntry_t * PDAddr = (PDEntry_t *)0x10000;
uint64_t userlandPhysicalPage = 10;

void mapUserspace()
{
  // Como inicialmente tenemos mapeo identidad, obtenemos la direccion fisica de la pagina 10 buscando la entrada
  // numero 10 de la PD.
  PDAddr[USERLAND_LOGIC_PAGE] = PDAddr[userlandPhysicalPage];
}
