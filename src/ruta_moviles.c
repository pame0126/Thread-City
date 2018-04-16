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
	int *rutaX = calloc(12, sizeof(int));
	int *rutaY = calloc(12, sizeof(int));
	rutaX[0] = 12; rutaY[0] = 12;
	int x = 1, y = 1;
	rutaX[1] = 1; rutaY[1] = 0;
	int i = 2;
	for(; i < 8;i++){
		rutaX[i] = x; rutaY[i] = y;
		x++;
	}
	//rutaX[i] = x; rutaY[i] = y;
	for(;i < 12;i++){
		rutaX[i] = x; rutaY[i] = y;
		y++;
	}
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
	//para el rio "Default \e[44mBlue"
	printf("    0 1 2 3 4 5 6\n");
	for(int i = 0; i < SIZE_X;i++){
		printf("%d [ ",i);
		for(int j = 0;j < SIZE_Y;j++){
			if(matriz_ciudad[i][j] == ROJO){
				printf("\e[31m%c\e[0m ", 'R');
			}
			else if(matriz_ciudad[i][j] == PUENTE_BLOQUEADO){
				printf("  ");
			}
			else if(matriz_ciudad[i][j] == PUENTE){
				printf("\e[37m%s\e[0m ", "░");
			}
			else if(matriz_ciudad[i][j] == BARCO){
				printf("\e[33m%c\e[0m ", 'B');
			}
			else if(( (i==4 || i==5 || i==6)&&
					(j==0 || j ==2 || j==3 || j==6) )
					&& matriz_ciudad[i][j] != BARCO){
				printf("\e[94m%c\e[0m ", '~');
			}
			else if(matriz_ciudad[i][j] != 0){
				printf("\e[36m%c\e[0m ", 'C');
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
				y0 = ( xj + 1 > SIZE_Y-1)? SIZE_Y-1 : xj+1;
				//diagonal inferior a posicion actual
				x1 = ( xi + 1 > SIZE_X-1)? SIZE_X-1 : xi+1;
				y1 = ( xj + 1 < 0)? 0 : xj+1;
			}
			//DIR IZQUIERDA
			else if(j < tupla[1][a-1]){
					//printf("izquierda %d < %d\n",j,tupla[1][a-1]);
				//diagonal superior a posicion actual
				x0 = ( xi - 1 < 0 )? 0 : xi-1;
				y0 = ( xj - 1 < 0)? 0 : xj-1;
				//diagonal inferior a posicion actual
				x1 = ( xi + 1 > SIZE_X-1)? SIZE_X-1 : xi + 1;
				y1 = ( xj - 1 < 0)? 0   : xj - 1;
					//printf("diagonal arriba %d -- diagonal abajo %d\n",matriz_ciudad[x0][y0],matriz_ciudad[x1][y1]);
			}
			//DIR ARRIBA
			else if(i < tupla[0][a-1]){
					//printf("arriba %d < %d\n",i,tupla[0][a-1]);
				//diagonal superior izquierda
				x0 = ( xi - 1 < 0)? 0 : xi-1;
				y0 = ( xj - 1 < 0)? 0 : xj-1;
				//diagonal superior derecha
				x1 = ( xi - 1 < 0)? 0 : xi-1;
				y1 = ( xj + 1 > SIZE_Y-1)? SIZE_Y-1 : xj+1;
					//printf("diagonal izq %d -- der %d\n",matriz_ciudad[x0][y0],matriz_ciudad[x1][y1]);
			}
			//DIR ABAJO
			else if(i > tupla[0][a-1]){
					//printf("abajo %d < %d\n",i,tupla[0][a-1]);
				//diagonal inferior izquierda
				x0 = ( xi + 1 > SIZE_X-1)? SIZE_X-1 : xi+1;
				y0 = ( xj - 1 < 0)? 0 : xj-1;
				//diagonal inferior derecha
				x1 = ( xi + 1 > SIZE_X-1)? SIZE_X-1 : xi + 1;
				y1 = ( xj + 1 > SIZE_Y-1)? SIZE_Y-1 : xj + 1;
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
				//si esta en el puente
				else if(matriz_ciudad[i][j] == PUENTE_BLOQUEADO){
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

/* 
 */
void*puente_un_carril(void*arg){
	mythread_yield();
	int *posX = calloc(3, sizeof(int));
	posX[0] = 4; posX[1] = 5; posX[2] = 6;
	int posY = 1;
	
	//semaforos del puente
	int semaf1X = 3, semaf1Y = 1;
	int semaf2X = 7, semaf2Y = 1;
	//valores del puente
	int valor1 = VERDE, valor2 = VERDE;
	int bandera = 1;//verde
	int contador = 5;
	//int bloqueado = 0;
	
	while(1){
		//si tiene un barco a la par por la izquierda o derecha
		if( matriz_ciudad[posX[0]][posY-1] == BARCO ||
		    matriz_ciudad[posX[1]][posY-1] == BARCO ||
		    matriz_ciudad[posX[2]][posY-1] == BARCO ){
				//espera que no haya carros en el puente
			//Bloqueo el puente
			for (int i = 0; i < 3; i++) {
				matriz_ciudad[posX[i]][posY] = PUENTE_BLOQUEADO;
			}
			printf("puente bloqueado\n");
			//bloqueado = 1;
		}
		//si esta pasando el puente
		else if(matriz_ciudad[posX[0]][posY] == BARCO ||
				matriz_ciudad[posX[1]][posY] == BARCO ||
				matriz_ciudad[posX[2]][posY] == BARCO ){
				printf("pasa el barco\n");
				//se espera hasta que no haya barcos a la par del puente
				mythread_yield();
				NULL;
		}
		else{
			printf("puente normal\n");
			//pone banderas de puente
			for(int i = 0;i < 3;i++){
				matriz_ciudad[posX[i]][posY] = (matriz_ciudad[posX[i]][posY] == 0 ||
												matriz_ciudad[posX[i]][posY] == PUENTE_BLOQUEADO)?
												 PUENTE : matriz_ciudad[posX[i]][posY];
			}
			if (contador == 5){
				valor1 =  (bandera)? ROJO : VERDE;
				valor2 = (bandera)? VERDE : ROJO;
				bandera = (bandera)? 0 : 1;
				contador = 0;
			}
			if(matriz_ciudad[semaf1X][semaf1Y]==VERDE || matriz_ciudad[semaf1X][semaf1Y]==ROJO){
				matriz_ciudad[semaf1X][semaf1Y] = valor1;//semaforo arriba
			}
			if(matriz_ciudad[semaf2X][semaf2Y]==VERDE || matriz_ciudad[semaf2X][semaf2Y]==ROJO){
				matriz_ciudad[semaf2X][semaf2Y] = valor2;//semaforo abajo
			}
			contador++;
		}
		mythread_yield();
	}
	mythread_end(NULL);
	return NULL;
}
/* Genera la ruta a los barcos
 */
int**ruta_barco(){
	int**ruta = calloc(2, sizeof(int*));
	int*rutaX = calloc(4, sizeof(int));
	int*rutaY = calloc(4, sizeof(int));
	//punto de inicio y final
	int iniX = (rand()%3)+4;
	int iniY = 0;
	
	int terY = (rand()%2 == 0)? 4:5;
	//largo
	rutaX[0] = terY; rutaY[0] = terY;
	for (int i = 1;i < terY;i++){
		rutaX[i] = iniX;
		rutaY[i] = iniY;
		iniY++;
	}
	ruta[0] = rutaX;
	ruta[1] = rutaY;
	return ruta;
}

void*barco(void*arg){
	mythread_yield();
	int**ruta = ruta_barco();
	int len = ruta[0][0];
	int i , j;
	int antX, antY;
	int px=-1, py=-1;
	
	for (int a = 1;a < len;a++){
		i = ruta[0][a];
		j = ruta[1][a];
		if(a > 1){
			antX = ruta[0][a-1]; antY = ruta[1][a-1];
			if(matriz_ciudad[i][j] == 0){
				matriz_ciudad[i][j] = BARCO;
				borrar_poss_anterior(antX, antY);
				if(px!=-1 && py!=-1){
					matriz_ciudad[px][py] = PUENTE_BLOQUEADO;
				}
			}
			else if(matriz_ciudad[i][j] == PUENTE_BLOQUEADO){
				matriz_ciudad[i][j] = BARCO;
				borrar_poss_anterior(antX,antY);
				px = i; py=j;
			}
			
			else if(matriz_ciudad[i][j] == PUENTE){
				//matriz_ciudad[antX][antY] = BARCO;
				a--;
			}
		}
		else{
			matriz_ciudad[i][j] = BARCO;
		}
		mythread_yield();
	}
	matriz_ciudad[i][j] = 0;
	mythread_end(NULL);
	return NULL;
}
