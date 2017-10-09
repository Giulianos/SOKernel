/*blobsFront.h*/ 

#ifndef _blobsFront_h
#define _blobsFront_h

#include "blobsBack.h"

int juego();
void pedirDimensiones(tipoJuego *);
void imprimirError(tipoJuego *);
void imprimirTablero(tipoJuego *);
int menuInicial();
void imprimirResultados(tipoJuego *);
int siNo(char*mensaje);
void pedirComando(char [90]);
#endif
