#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <ruta_moviles.h>
#include <mypthreads.h>

int CANT_RUTAS_QUEMADAS = 0;

int ID_PROGRES = 1;


/* Inicia lista de autos
 */
autos*inicia_lista_carros(){
	autos*elem = calloc(1, sizeof(autos));
	elem->list_carros = (carro**)calloc(SIZE_AUTOS, sizeof(carro*));
	elem->len = 0;
	return elem;
}

int **ruta_q_A(){
	int *rutaX = calloc(9, sizeof(int));
	int *rutaY = calloc(9, sizeof(int));
	rutaX[0] = 9; rutaY[0] = 9;
	
	rutaX[1] = 6; rutaY[1] = 5;
	rutaX[2] = 6; rutaY[2] = 6;
	rutaX[3] = 6; rutaY[3] = 7;
	rutaX[4] = 6; rutaY[4] = 8;
	rutaX[5] = 7; rutaY[5] = 8;
	rutaX[6] = 8; rutaY[6] = 8;
	rutaX[7] = 9; rutaY[7] = 8;
	rutaX[8] = 10;rutaY[8] = 8;
	int **res = (int**)calloc(2, sizeof(int*));
	res[0] = rutaX; res[1] = rutaY;
	return res;
}

int **ruta_q_B(){
	int *rutaX = calloc(8, sizeof(int));
	int *rutaY = calloc(8, sizeof(int));
	rutaX[0] = 8; rutaY[0] = 8;
	
	rutaX[1] = 8; rutaY[1] = 11;
	rutaX[2] = 7; rutaY[2] = 11;
	rutaX[3] = 7; rutaY[3] = 10;
	rutaX[4] = 7; rutaY[4] = 9;
	rutaX[5] = 7; rutaY[5] = 8;
	rutaX[6] = 7; rutaY[6] = 7;
	rutaX[7] = 7; rutaY[7] = 6;
	int **res = (int**)calloc(2, sizeof(int*));
	res[0] = rutaX; res[1] = rutaY;
	return res;
}

int **ruta_q_C(){
	int *rutaX = calloc(8, sizeof(int));
	int *rutaY = calloc(8, sizeof(int));
	rutaX[0] = 8; rutaY[0] = 8;
	
	rutaX[1] = 9; rutaY[1] = 11;
	rutaX[2] = 8; rutaY[2] = 11;
	rutaX[3] = 7; rutaY[3] = 11;
	rutaX[4] = 7; rutaY[4] = 10;
	rutaX[5] = 7; rutaY[5] = 9;
	rutaX[6] = 7; rutaY[6] = 8;
	rutaX[7] = 7; rutaY[7] = 7;
	int **res = (int**)calloc(2, sizeof(int*));
	res[0] = rutaX; res[1] = rutaY;
	return res;
}

int **ruta_q_D(){
	int *rutaX = calloc(9, sizeof(int));
	int *rutaY = calloc(9, sizeof(int));
	rutaX[0] = 9; rutaY[0] = 9;
	
	rutaX[1] = 8;  rutaY[1] = 19;
	rutaX[2] = 9;  rutaY[2] = 19;
	rutaX[3] = 10; rutaY[3] = 19;
	rutaX[4] = 11; rutaY[4] = 19;
	rutaX[5] = 12; rutaY[5] = 19;
	rutaX[6] = 13; rutaY[6] = 19;
	rutaX[7] = 14; rutaY[7] = 19;
	rutaX[8] = 15; rutaY[8] = 19;
	rutaX[9] = 16; rutaY[9] = 19;
	int **res = (int**)calloc(2, sizeof(int*));
	res[0] = rutaX; res[1] = rutaY;
	return res;
}

/*
 */
int **rutas_quemadas(){
	int **res = NULL;
	switch(CANT_RUTAS_QUEMADAS){
		case 0:
			res = ruta_q_A();
			break;
		case 1:
			res = ruta_q_B();
			break;
		case 2:
			res = ruta_q_C();
			break;
		case 3:
			res = ruta_q_D();
			break;
		default:
			break;
	}
	CANT_RUTAS_QUEMADAS++;
	return res;
}

/* Genera dos arreglos de largo 5 que contiene las rutas de cada automovil
 * los devuelve en una tupla.
 * */
int**genera_ruta_carro(){
	int *rutaX = calloc(6, sizeof(int));
	int *rutaY = calloc(6, sizeof(int));
	int dir = rand()%2;
	int i;
	int j;
	int num_dir;
	//si sale 1 va por la DERECHA
	//si sale 0 va por la IZQUIERDA
	if(dir){
		i = rand()%20;
		j = rand()%20;
		num_dir = 1;
	}
	else{
		i = (rand()%20) + 5;
		j = (rand()%20) + 5;
		num_dir = -1;
	}
	rutaX[0] = 5;//guarda en la primera posicion el largo de la ruta
	rutaY[0] = 5;
	for(int a = 1;a < 6;a++){
		rutaX[a]=i;
		rutaY[a]=j;
		printf("%d \t %d\n",i,j);
		i+= num_dir;
		j+= num_dir;
	}
	printf("\n");
	int **t = (int**)calloc(2,sizeof(int*));
	t[0]=rutaX;
	t[1]=rutaY;
	return t;
}


/* imprime la martiz de la cuidad
 * */
void *print_matriz(){
	printf("    0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24\n");
	int x;
	for(int i = 0; i < SIZE_MATRIZ;i++){
		printf("%d [ ",i);
		for(int j = 0;j < SIZE_MATRIZ;j++){
			if(matriz_ciudad[i][j] == ROJO){
				x = matriz_ciudad[i][j];
				printf("\e[31m%d\e[0m ", x);
			}
			else{
				printf("%d ", matriz_ciudad[i][j]);
			}
		}
		printf("]\n");
	}
	printf("----------------------------\n");
	return NULL;
}

void free_tupla_ruta(int **tupla){
	free(tupla[0]);
	free(tupla[1]);
	free(tupla);
}

void borrar_poss_anterior(int i ,int j){
	matriz_ciudad[i][j] = 0;
}

/* Se crea la ruta para el hilo actual.
 * Esta funcion recorre la ruta recibida con validaciones de choques de carros.
 * Si un carro va a chocar con otro se pone en condicion de dar pase (numero negativo)
 * y espera un turno o los que sean necesarios para no chocar.
 * */
void*arrancar_carro(void*arg){
	mythread_yield();
	int **tupla = rutas_quemadas();
	int i = 0, j = 0;    //posicion a la que se movera
	int xi = 0, xj = 0;  //posicion actual
	int len = tupla[0][0];
	
	int x0 = 0, y0 = 0;  //diagonales 1
	int x1 = 0, y1 = 0;  //diagonales 2
	pid_t id = __mythread_gettid();
		//printf("%d largo %d",id, len);
	//recorrer los arreglos de ruta
	for(int a = 1; a < len ;a++){		
		i = tupla[0][a];
		j = tupla[1][a];
		if(a > 1){//si no es el primer movimiento
			//posicion antes del cambio
			xi = tupla[0][a-1];
			xj = tupla[1][a-1];
			
			mythread_yield();
			//mira a que lado va izq o der
			if(j > tupla[1][a-1]){//Direccion DERECHA
					//printf("derecha %d > %d\n",j,tupla[1][a-1]);
				//diagonal superior a posicion actual
				x0 = ( xi - 1 < 0 )? 0  : xi-1;
				y0 = ( xj + 1 > 24)? 24 : xj+1;
				//diagonal inferior a posicion actual
				x1 = ( xi + 1 > 24)? 24 : xi+1;
				y1 = ( xj + 1 < 0)? 0   : xj+1;
			}
			//DIR IZQUIERDA
			else if(j < tupla[1][a-1]){
					//printf("izquierda %d < %d\n",j,tupla[1][a-1]);
				//diagonal superior a posicion actual
				x0 = ( xi - 1 < 0 )? 0 : xi-1;
				y0 = ( xj - 1 < 0)? 0 : xj-1;
				//diagonal inferior a posicion actual
				x1 = ( xi + 1 > 24)? 24 : xi + 1;
				y1 = ( xj - 1 < 0)? 0   : xj - 1;
					//printf("diagonal arriba %d -- diagonal abajo %d\n",matriz_ciudad[x0][y0],matriz_ciudad[x1][y1]);
				
			}
			//DIR ARRIBA
			else if(i < tupla[0][a-1]){
					//printf("arriba %d < %d\n",i,tupla[0][a-1]);
				//diagonal superior izquierda
				x0 = ( xi - 1 < 0 )? 0 : xi-1;
				y0 = ( xj - 1 < 0)? 0 : xj-1;
				//diagonal superior derecha
				x1 = ( xi - 1 < 0)? 0 : xi-1;
				y1 = ( xj + 1 > 24)? 24 : xj+1;
					//printf("diagonal izq %d -- der %d\n",matriz_ciudad[x0][y0],matriz_ciudad[x1][y1]);
			}
			//DIR ABAJO
			else if(i > tupla[0][a-1]){
					//printf("abajo %d < %d\n",i,tupla[0][a-1]);
				//diagonal inferior izquierda
				x0 = ( xi + 1 > 24 )? 24 : xi+1;
				y0 = ( xj - 1 < 0)? 0 : xj-1;
				//diagonal inferior derecha
				x1 = ( xi + 1 > 24)? 24 : xi + 1;
				y1 = ( xj + 1 > 24)? 24 : xj + 1;
					//printf("diagonal izq %d -- der %d\n",matriz_ciudad[x0][y0],matriz_ciudad[x1][y1]);
			}
				//Evalua el movimiento
				if( matriz_ciudad[x0][y0] == 0 &&
				    matriz_ciudad[x1][y1] == 0 &&
				    matriz_ciudad[i][j] == 0 ){
						//printf("no hay choque %d\n",id);
					matriz_ciudad[i][j] = id;
					borrar_poss_anterior(xi,xj);//borrar el anterior
				}
				//si esta en el puente
				else if(matriz_ciudad[i][j] == PUENTE){
					matriz_ciudad[i][j] = id;
					borrar_poss_anterior(xi,xj);//borrar el anterior
				}
				//te estan dando pase
				else if( (matriz_ciudad[x0][y0] < 0 || matriz_ciudad[x1][y1] < 0)){
						//printf("hay choque, te estan dando pase\n");
					matriz_ciudad[i][j] = id;
					//borrar el anterior
					borrar_poss_anterior(xi,xj);
				}
				else{//se queda en el mismo lugar con negativo
						//printf("\t\thay choque %d\n",id);
					matriz_ciudad[xi][xj] = id*-1;
					a--;
				}
		}
		//si es primer elemento
		else{
			matriz_ciudad[i][j] = id;
		}
		mythread_yield();
	}
	matriz_ciudad[i][j] = 0;
	//Elimina los espacios de memoria
	free_tupla_ruta(tupla);
	mythread_end(NULL);//el hilo muere
	return NULL;
}

/* Se guardan las posiciones de los semaforos
 * Son posiciones fijas.
 * No maneja los semaforos de los puentes
 */
int** ubicacion_semaforos(){
	int **semaforos = (int**)calloc(2, sizeof(int*));
	//dos semaforos
	semaforos[0] = calloc(3, sizeof(int*));
	semaforos[1] = calloc(3, sizeof(int*));
	//largo de la lista
	semaforos[0][0] = 3; semaforos[1][0] = 3;
	//posiciones X - Y
	semaforos[0][1] = 7; semaforos[1][1] = 8;
	semaforos[0][2] = 11; semaforos[1][2] = 8;
	
	return semaforos;
}

/*
 * Va a manejar todos los semaforos menos el de los
 * puentes.
 */
void*control_semaforos(void*arg){
	mythread_yield();
	//filas y columnas de posiciones de semaforos
	int **semaforos = ubicacion_semaforos();
	int len = semaforos[0][0];
	int bandera = 1;//verde
	int valor;
	int i, x, y;
	int contador = 5;
	while (1){
		if (contador == 5){
			valor = (bandera)? ROJO : VERDE;
			bandera = (bandera)? 0 : 1;
			for ( i = 1 ; i < len ; i++ ){
				x = semaforos[0][i];
				y = semaforos[1][i];
				matriz_ciudad[x][y] = valor;
			}
			contador = 0;
		}
		contador++;
		mythread_yield();
	}
	
	mythread_end(NULL);//el hilo muere
	return NULL;
}

/* 
 */
void*puente_un_carril(void*arg){
	mythread_yield();
	int *posX = calloc(3, sizeof(int));
	posX[0] = 11; posX[1] = 12; posX[2] = 13;
	int posY = 19;
	
	//semaforos del puente
	int semaf1X = 10, semaf1Y = 19;
	int semaf2X = 14, semaf2Y = 19;
	int valor1, valor2;
	int bandera = 1;//verde
	int contador = 5;
	
	while(1){
		//pone banderas de puente
		for(int i = 0;i < 3;i++){
			matriz_ciudad[posX[i]][posY] = (matriz_ciudad[posX[i]][posY] == 0)? PUENTE : matriz_ciudad[posX[i]][posY];
		}
		if (contador == 5){
			valor1 =  (bandera)? ROJO : VERDE;
			valor2 = (bandera)? VERDE : ROJO;
			bandera = (bandera)? 0 : 1;
			matriz_ciudad[semaf1X][semaf1Y] = valor1;//semaforo arriba
			matriz_ciudad[semaf2X][semaf2Y] = valor2;//semaforo abajo
			contador = 0;
		}
		contador++;
		mythread_yield();
	}
	mythread_end(NULL);
	return NULL;
}
