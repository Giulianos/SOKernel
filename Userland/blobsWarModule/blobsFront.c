/*blobsFront.c*/

#include "include/blobsFront.h"
#include "include/blobsBack.h"

#include "include/stdio.h"
#include "include/string.h"

#define ON 1
#define OFF 0

/*main*/
void *malloc(unsigned nbytes);
int main()
{
	/*Declaraciones*/
	 int flagComando=-1;			
	 tipoJuego Partida;

	 char ingreso[90];
	 int movimiento[4], disponible = 0;
	 do{
	 	/*Inicia programa*/
	 	Partida.modo=menuInicial();
		
	 	decidirMenu(&Partida);
	 	if(Partida.modo!=4)
	 	{	
	 		if (Partida.modo != 3)
	 		{
				
	 			/*Nueva partida*/
	 			Partida.turno = 1;
	 			pedirDimensiones(&Partida);
	 			crearTablero(&Partida);
			}
			imprimirError(&Partida);
			/*Inicia la partida*/
			imprimirTablero(&Partida);
			while((disponible = buscarMovimiento(&Partida, movimiento)) && Partida.modo != 4)
			{
				printf("Turno de jugador %c.\n", (Partida.turno == 1)?'A':'Z');
				if(Partida.modo == 1 ||( Partida.turno == 1 && Partida.modo == 2))
				{
					/*Turno de jugador fisico*/
					do
					{
						pedirComando(ingreso);
						//printf("%s",ingreso);
						flagComando = analizarComando(&Partida, movimiento, ingreso);
						if (flagComando==-1)
						{
							printf("Ingreso erroneo\n");
							imprimirError(&Partida);
						}					
					}
					while(flagComando == -1);
					switch (flagComando)
						{
						case 0:
							return 0;
						case 1: 
							efectuarMovimiento(&Partida, movimiento);
							Partida.turno *= -1;
							break;
						case 2: 							
							break;
						}
				
				}else{
					/*Turno maquina*/
					movimientoDeMaquina(&Partida, movimiento);
					efectuarMovimiento(&Partida, movimiento);
					Partida.turno *= -1;
				
				}
				imprimirTablero(&Partida);
				imprimirResultados(&Partida);
				
			}
			/*Fin partida, se imprime resultados a menos que haya salido antes de terminarla*/
			if(disponible==0)
					printf("El jugador %c esta bloqueado \n",(Partida.turno==1)?'A':'Z');
			
			if (Partida.modo!=4 && disponible==0)
			{	rellenar(&Partida);
				//imprimirTablero(&Partida);
				imprimirResultados(&Partida);
				printf("Fin del juego\n");
			}
			resetear(&Partida);	
		}
		
	 }while(Partida.modo != 4);

	return 0;
}


/*funciones*/



void pedirComando(char ingreso[90])
{
char c; int a=0;	
printf("Ingrese un comando: \n");
getline(ingreso);
}

void 
imprimirError(tipoJuego * Partida)
{
	if (Partida->error.codigoError!=0)
	{	printf("Codigo de error %d: ",Partida->error.codigoError);
		switch(Partida->error.codigoError)
		{	
			case 1: 
				printf("No hay memoria suficiente.\n");
				break;			
			case 2: 
				printf("El movimiento no esta dentro de los margenes del tablero.\n");
				break;
			case 3: 
				printf("La ficha que intenta mover no es propia.\n");
				break;
			case 4: 
				printf("El movimiento es demasiado largo. Distancia maxima dos casilleros. \n");
				break;
			case 5: 
				printf("Se esta realizando un movimiento a la misma posicion inicial.\n");
				break;
			case 6: 
				printf("Casilla destino ocupada.\n");
				break;
		}
	}		
}

void 
imprimirTablero(tipoJuego * Partida)
{
 /* Se imprime el tablero con indices en sus bordes superior e izquierdo.A Y Zrepresentan los distintos jugadores.*/
 int i,j;
 printf("\n");
 putchar(' ');
 for(i=-1;i < Partida->tablero.alto ;i++)
	{
 	if (i != (-1))
 		printf("%d",i);
	for(j=0;j < Partida->tablero.ancho ;j++)
 		{
 		putchar(' ');
		putchar(' ');
 		if(i == (-1))
 			printf("%d",j);
 		else
 			switch (Partida->tablero.matriz[i][j])
 				{
 				case 1:
					putchar('A');putchar(' ');
 					break;
				case -1:putchar('Z');putchar(' ');
					break;
				default: putchar('0'); putchar(' ');
 				}
 		}
 		putchar('\n');
 	}
}

void
pedirDimensiones(tipoJuego * Partida)
{
	Partida->tablero.alto=0;
	Partida->tablero.ancho=0;
	char ancho[20];
	char alto[20];
 /* Se piden ancho y alto al usuario hasta que cumpla con las condiciones.*/
 do{
 	printf("ingrese ancho\n");
 	pedirComando(ancho);
 	for(int x=0;ancho[x]!='\0';x++){
 		
 		Partida->tablero.ancho=Partida->tablero.ancho*10+ancho[x]-'0';
 	}
 }while(!(Partida->tablero.ancho > 2 && Partida->tablero.ancho < 31 ));
 
 do{
 	printf("ingrese alto\n");
 	pedirComando(alto);
 	for(int x=0;alto[x]!='\0';x++){

 		Partida->tablero.alto=Partida->tablero.alto*10+alto[x]-'0';
 	}
 }while(!(Partida->tablero.alto > 2 && Partida->tablero.alto < 31 ));

printf("%d %d", Partida->tablero.alto,Partida->tablero.ancho);

}

void
imprimirResultados(tipoJuego * Partida)
{
	if(Partida->jugadorUno.cantidadFichas > Partida->jugadorDos.cantidadFichas)
		if(Partida->jugadorDos.flagMaquina)
 			printf("Gana el jugador! con %d fichas contra %d fichas de la maquina.\n", Partida->jugadorUno.cantidadFichas, Partida->jugadorDos.cantidadFichas);
 		else
 			printf("Gana el jugador A! con %d fichas contra %d fichas del jugador Z.\n", Partida->jugadorUno.cantidadFichas, Partida->jugadorDos.cantidadFichas);
 	else if(Partida->jugadorUno.cantidadFichas < Partida->jugadorDos.cantidadFichas)
 		if (Partida->jugadorDos.flagMaquina)
 			printf("Gana la maquina! con %d fichas contra %d fichas del jugador.\n", Partida->jugadorDos.cantidadFichas, Partida->jugadorUno.cantidadFichas);
 		else
 			printf("Gana el jugador Z! con %d fichas contra %d fichas del jugador A.\n", Partida->jugadorDos.cantidadFichas, Partida->jugadorUno.cantidadFichas);
 	else
		printf("Empate en %d fichas. \n", Partida->jugadorUno.cantidadFichas);
}

int 
menuInicial()
{
	//int aux;
	//do{
	 //	printf("\n\nMenu:\nIngrese un numero:\n1)Dos jugadores\n2)Un jugador\n3)Abrir juego\n4)terminar\n");
	//}while (aux<1 || aux >4);
	return 2;
}


int 
siNo(char*mensaje)
{
	char c, men[4];
	int a,flag=0,ret=0;
	printf("%s\n",mensaje);
	do{
		a=0;
		while((c=getchar())!='\n'&& a<2)
			men[a++]=c;
		men[a]='\0';
		if (men[1]=='\0')
		{
				if(men[0]=='s' )
				{
					flag=1;
					ret=1;
				}
				else if(men[0]=='n')
				{
					flag=1;
					ret=0;
				}
				else 
					{printf("Parametro erroneo, ingrese s o n.\n");
					while(getchar()!='\n'){}}
		}
		else 
			{printf("Parametro erroneo, ingrese s o n.\n");
			while(getchar()!='\n'){}}
	}while(!flag);
	return ret;
}
