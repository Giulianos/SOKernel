/*backEnd.c*/


#include "include/blobsBack.h"
#include "include/blobsFront.h"
#include "include/stdio.h"
#include "include/string.h"

#define NULL 0
#define ON 1
#define OFF 0
#include "blobsBack.h"
void *malloc(unsigned nbytes);
int buscarMovimientoSimple(tipoJuego * m, int * movimiento)
{
	int i, j, k, l, arr, abj, izq, der, flag = 0;
	for(i = 0; i < (*m).tablero.alto && flag == 0; i++)
	{
		for(j = 0; j < (*m).tablero.ancho && flag == 0; j++)
		{
			if((*m).turno == (*m).tablero.matriz[i][j])
			{
				izq = (j == 0)? 0 : 1;
				der = (((*m).tablero.ancho - 1) == j)? 0 : 1;
				arr = (i == 0)? 0 : 1;
				abj = (((*m).tablero.alto - 1) == i)? 0 : 1;
				for(k = i - arr; flag == 0 && k < i + abj; k++)
				{
					for(l = j - izq; flag == 0 && l < j + der; l++)
					{
						if((*m).tablero.matriz[k][l] == 0)
						{
							flag = 1;
							movimiento[0] = i;
							movimiento[1] = j;
							movimiento[2] = k;
							movimiento[3] = l;
						}
					}
				}
			}		
		}
	}
	return flag;	
}
void resetear(tipoJuego * Partida)
	/*hace free de todas las estructuras una vez terminada la partida*/
{	int x;
	for (x=0;x<Partida->tablero.alto;x++)
		free(Partida->tablero.matriz[x]);
	free(Partida->tablero.matriz);
	free(Partida->archivo.nombreDeArchivo);
	Partida->modo = 0;
	Partida->jugadorUno.cantidadFichas = 2;
	Partida->jugadorDos.cantidadFichas = 2;
}
void movimientoDeMaquina(tipoJuego * Partida, int * movimiento)
{
	if(!buscarAtaque(Partida, movimiento))
	{		
		buscarMovimientoSimple(Partida, movimiento);
	}
}

/*buscamos el mejor ataque y si hay varios uno rand entre todos esos*/
int buscarAtaque(tipoJuego * Partida, int * movimiento)
{
	int i,j,flag = 0;
	for(i=0;i<Partida->tablero.alto;i++)	
		for(j=0;j<Partida->tablero.ancho;j++)
			if(Partida->turno == Partida->tablero.matriz[i][j])
				flag = ataqueOptimo(Partida, i, j, movimiento, flag);
	return flag;
}
int ataqueOptimo(tipoJuego * Partida, int fil, int col, int * movimiento, int max){
	int izq, der, arr, abj, k, l, cant;
	izq = (col <= 2)? col : 2;
	der = ((*Partida).tablero.ancho - col - 1 <= 2)? (*Partida).tablero.ancho - col - 1 : 2;
	arr = (fil <= 2)? fil : 2;
	abj = ((*Partida).tablero.alto - fil - 1 <= 2)? (*Partida).tablero.alto - fil - 1 : 2;
	for(k = fil - arr; k <= fil + abj;k++)
		for(l=col - izq;l <= col + der;l++)
			if(Partida->tablero.matriz[k][l] == 0){
				cant = fichasAfectadas(Partida, k, l);
				if(cant > max){
					max = cant;
					movimiento[0] = fil;
					movimiento[1] = col;
					movimiento[2] = k;
					movimiento[3] = l;
				}
			}	
	return max;
}

int fichasAfectadas(tipoJuego * Partida, int fil, int col)
{
	int izq, der, arr, abj, k, l, cant = 0;
	izq = (col == 0)? 0 : 1;
	der = ((Partida->tablero.ancho - 1) == col)? 0 : 1;
	arr = (fil == 0)? 0 : 1;
	abj = ((Partida->tablero.alto - 1) == fil)? 0 : 1;
	for(k = fil - arr; k <= fil + abj;k++)
		for(l=col - izq;l <= col + der;l++)
			if(Partida->tablero.matriz[k][l] == ((-1)*Partida->turno))
				cant++;
	return cant;		
}


void efectuarMovimiento(tipoJuego * m, int * movimiento)
{
	if(abs(movimiento[0] - movimiento[2]) > 1 || abs(movimiento[1] - movimiento[3]) > 1)
	{
		(*m).tablero.matriz[movimiento[0]][movimiento[1]] = 0;
		if(m->turno == 1)
		{
			(m->jugadorUno.cantidadFichas)--;
		}
		else
		{
			(m->jugadorDos.cantidadFichas)--;
		}
		
	}
	if(m->turno == 1)
	{
		(m->jugadorUno.cantidadFichas)++;
	}
	else
	{
		(m->jugadorDos.cantidadFichas)++;
	}
	(*m).tablero.matriz[movimiento[2]][movimiento[3]] = (*m).turno;
	atacar(m, movimiento);	
}



int
analizarComando(tipoJuego * Partida, int * movimiento, char * ingreso){
	char caracterFinal='\0', corchete, flag = -1;
		if(strcmp("quit", ingreso)){
			flag=0;
		}else {
			int a=sscanf("[%d,%d][%d,%d%c",ingreso, &movimiento[0], &movimiento[1], &movimiento[2], &movimiento[3],&corchete);
			if (a == 5 && corchete==']')
			{
			if(validarMovimiento(Partida, movimiento))
				{flag=1;}
			}
	}
	return flag;
}

int decidirMenu(tipoJuego * Partida)
{	Partida->error.codigoError=0;
	switch(Partida->modo)
	{
		case 1: 
			Partida->jugadorDos.flagMaquina=OFF;
			break;
		case 2:
			Partida->jugadorDos.flagMaquina=ON;
			break;
		case 3:
			break;
	}
return 0;
}


int 
crearTablero(tipoJuego * Partida)
{
 /* Se reserva espacio suficiente en memoria para toda la Partida->*/
int **aux = NULL;
int *aux2=NULL;
int k;
 aux = malloc( Partida->tablero.alto * sizeof(int *));
 if(aux != NULL)
 	{Partida->tablero.matriz = aux;
	for (k=0;k< Partida->tablero.alto;k++)
		{aux2=malloc(Partida->tablero.ancho*sizeof(int));
		if (aux2 != NULL)
			Partida->tablero.matriz[k]=aux2;
		}
	}
for(int x=0;x<10;x++)
	for(int y=0; y<10;y++)
		Partida->tablero.matriz[x][y]=0;


 /* Se ponen las primeras dos fichas por jugador en las puntas.*/
 Partida->tablero.matriz[0][0]=1;
 Partida->tablero.matriz[0][(Partida->tablero.ancho - 1)]=-1;
 Partida->tablero.matriz[(Partida->tablero.alto -1)][0]=1;
 Partida->tablero.matriz[(Partida->tablero.alto -1)][(Partida->tablero.ancho -1)]=-1;
 Partida->jugadorUno.cantidadFichas = 2;
 Partida->jugadorDos.cantidadFichas = 2;
return 1;
}



/*buscamos un movimiento cualquiera para comprobar que la matriz no se bloquee*/
int buscarMovimiento(tipoJuego * miPartida, int * movimiento)
{
	int i, j, flag = 0;
	for(i = 0; i < (*miPartida).tablero.alto && flag == 0; i++)
		for(j = 0; j < (*miPartida).tablero.ancho && flag == 0; j++)
			if((*miPartida).turno == (*miPartida).tablero.matriz[i][j])
				flag = hallarCero(miPartida, i, j, movimiento);
	return flag;
}



int hallarCero(tipoJuego * m, int fil, int col, int * movimiento)
{
	int izq = (col <= 2)? col : 2;
	int der = ((*m).tablero.ancho - col - 1 <= 2)? (*m).tablero.ancho - col - 1 : 2;
	int arr = (fil <= 2)? fil : 2;
	int abj = ((*m).tablero.alto - fil - 1 <= 2)? (*m).tablero.alto - fil - 1 : 2;
	int i, j, flag = 0;
	for(i = fil - arr; i <= fil + abj && flag == 0; i++)
	{
		for(j = col - izq; j <= col + der && flag == 0; j++)
		{
			if((*m).tablero.matriz[i][j] == 0)
			{
				flag = 1;
				movimiento[0] = fil;
				movimiento[1] = col;
				movimiento[2] = i;
				movimiento[3] = j;
				
			}
		}
	}
	return flag;
}


void atacar(tipoJuego * m, int * movimiento)
{
	int k,l;
	int izq = (movimiento[3] == 0)? 0 : 1;
	int der = (((*m).tablero.ancho - movimiento[3] - 1) == 0)? 0 : 1;
	int arr = (movimiento[2] == 0)? 0 : 1;
	int abj = (((*m).tablero.alto - movimiento[2] - 1) == 0)? 0 : 1;
	for(k = movimiento[2] - arr; k <= movimiento[2] + abj; k++)
		for(l = movimiento[3] - izq; l <= movimiento[3] + der; l++)
		{
			if(m->tablero.matriz[k][l] != 0 && m->tablero.matriz[k][l] != m->turno)
			{
				if(m->turno == 1)
				{
					(m->jugadorUno.cantidadFichas)++;
					(m->jugadorDos.cantidadFichas)--;
				}
				else
				{
					(m->jugadorDos.cantidadFichas)++;
					(m->jugadorUno.cantidadFichas)--;
				}
			}
			m->tablero.matriz[k][l] = (m->tablero.matriz[k][l]*m->tablero.matriz[k][l]) * m->turno; 
		}
			
}
void 
rellenar(tipoJuego *partida){
		int a,b;
		for (a=0;a<(partida->tablero.alto);a++)
			for(b=0;b<(partida->tablero.ancho);b++)
				if (partida->tablero.matriz[a][b]==0)
				{
					partida->tablero.matriz[a][b]=(partida->turno) *(-1);
					if(partida->turno == 1)
						(partida->jugadorDos.cantidadFichas)++;
					else
						(partida->jugadorUno.cantidadFichas)++;
				}
}

int validarMovimiento(tipoJuego * Partida, int * movimiento)
{				/*los primeros dos if son si hace un movimiento fuera del limite, el 3 no es tu ficha, 4 demasiado largo, */
	int flag = 0;		/*5 si se quiere mover al mismo lugar que esta y 6 si el lugar esta ocupado*/
		if(movimiento[0] >= 0 && movimiento[1] >= 0 && movimiento[2] >= 0 && movimiento[3] >=0){		
				if(movimiento[0] < Partida->tablero.alto && movimiento[1] < Partida->tablero.ancho && movimiento[2] < Partida->tablero.alto && movimiento[3] < Partida->tablero.ancho){
					if(Partida->tablero.matriz[movimiento[0]][movimiento[1]] == Partida->turno){
						if(abs(movimiento[0] - movimiento[2]) <= 2 && abs(movimiento[1] - movimiento[3]) <= 2){
							if(movimiento[0] != movimiento[2] || movimiento[1] != movimiento[3]){
								if(Partida->tablero.matriz[movimiento[2]][movimiento[3]]==0)
									flag = 1;
								else
									Partida->error.codigoError = 6;
							}else{Partida->error.codigoError = 5;}
						}else{Partida->error.codigoError = 4;}
					}else{Partida->error.codigoError = 3;}
				}else{Partida->error.codigoError = 2;}
		}else{Partida->error.codigoError = 2;}
									
					
	return flag;
}
