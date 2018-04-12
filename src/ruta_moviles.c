
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <ruta_moviles.h>
#include <mypthread.h>

#define ALTA 1
#define BAJA 0

int ID_PROGRES = 1;

/* Inicia lista de autos
 * 
 * */
autos*inicia_lista_carros(){
	autos*elem = calloc(1, sizeof(autos));
	elem->list_carros = (carro**)calloc(SIZE_AUTOS, sizeof(carro*));
	elem->len = 0;
	return elem;
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
	my_thread_yield();
	printf("    0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24\n");
	for(int i = 0; i < SIZE_MATRIZ;i++){
		printf("%d [ ",i);
		for(int j = 0;j < SIZE_MATRIZ;j++){
			printf("%d ", matriz_ciudad[i][j]);
		}
		printf("]\n");
	}
	printf("----------------------------\n");
	my_thread_yield();
	return NULL;
}

void borrar_poss_anterior(int i ,int j){
	matriz_ciudad[i][j] = 0;
	my_thread_yield();
}

/* Se crea la ruta para el hilo actual.
 * Esta funcion recorre la ruta recibida con validaciones de choques de carros.
 * Si un carro va a chocar con otro se pone en condicion de dar pase (numero negativo)
 * y espera un turno o los que sean necesarios para no chocar.
 * */
void*arrancar_carro(void*arg){
	my_thread_yield();
	int **tupla = genera_ruta_carro();
	int i = 0, j = 0;    //posicion a la que se movera
	int xi = 0, xj = 0;  //posicion actual
	int len = tupla[0][0];
	
	int x0 = 0, y0 = 0;  //diagonales 1
	int x1 = 0, y1 = 0;  //diagonales 2
	pid_t id = __mythread_gettid();
	id = (id%10)+1;
	
	//recorrer los arreglos de ruta
	for(int a = 1;a < len ;a++){		
		i = tupla[0][a];
		j = tupla[1][a];
		
		if(a > 1){//si no es el primer movimiento
			//posicion antes del cambio
			xi = tupla[0][a-1];
			xj = tupla[1][a-1];
			
			//mira a que lado va izq o der
			if(j > tupla[1][a-1]){//Direccion DERECHA
					printf("derecha %d > %d\n",j,tupla[1][a-1]);
				//diagonal superior a posicion actual
				x0 = ( xi - 1 < 0 )? 0  : xi-1;
				y0 = ( xj + 1 > 24)? 24 : xj+1;
				//diagonal inferior a posicion actual
				x1 = ( xi + 1 > 24)? 24 : xi+1;
				y1 = ( xj + 1 < 0)? 0   : xj+1;
				
				//si estan libres los campos
					printf("diagonal arriba %d -- diagonal abajo %d\n",matriz_ciudad[x0][y0],matriz_ciudad[x1][y1]);
				if( matriz_ciudad[x0][y0] == 0 &&
				    matriz_ciudad[x1][y1] == 0 &&
				    matriz_ciudad[i][j] == 0 ){
						printf("no hay choque%d\n",id);
					matriz_ciudad[i][j] = id;
					//borrar el anterior
					borrar_poss_anterior(xi,xj);
				}
				//te estan dando pase
				else if(matriz_ciudad[x0][y0] < 0 || matriz_ciudad[x1][y1] < 0){
					matriz_ciudad[i][j] = id;
					//borrar el anterior
					borrar_poss_anterior(tupla[0][a-1],tupla[1][a-1]);
				}
				else{
						printf("\t\thay choque %d\n",id);
					matriz_ciudad[xi][xj] = id*-1	;
					a--;
				}
			}
			
			//DIR IZQUIERDA
			else{
					printf("izquierda %d < %d\n",j,tupla[1][a-1]);
				//diagonal superior a posicion actual
				x0 = ( xi - 1 < 0 )? 0 : xi-1;
				y0 = ( xj - 1 < 0)? 0 : xj-1;
				//diagonal inferior a posicion actual
				x1 = ( xi + 1 > 24)? 24 : xi + 1;
				y1 = ( xj - 1 < 0)? 0   : xj - 1;
					printf("diagonal arriba %d -- diagonal abajo %d\n",matriz_ciudad[x0][y0],matriz_ciudad[x1][y1]);
				if( matriz_ciudad[x0][y0] == 0 &&
				    matriz_ciudad[x1][y1] == 0 &&
				    matriz_ciudad[i][j] == 0 ){
						printf("no hay choque %d\n",id);
					matriz_ciudad[i][j] = id;
					//borrar el anterior
					borrar_poss_anterior(xi,xj);
				}
				//te estan dando pase
				else if( (matriz_ciudad[x0][y0] < 0 || matriz_ciudad[x1][y1] < 0)){
						printf("hay choque, te estan dando pase\n");
					matriz_ciudad[i][j] = id;
					//borrar el anterior
					borrar_poss_anterior(xi,xj);
				}
				else{//se queda en el mismo lugar con negativo
						printf("\t\thay choque %d\n",id);
					matriz_ciudad[ xi ][ xj ] = id*-1	;
					a--;
				}
			}
		}
		//si es primer elemento
		else{
			matriz_ciudad[i][j] = id;
		}
		my_thread_yield();
		print_matriz();
		//sleep(1);//pausa para el movimiento
	}
	matriz_ciudad[i][j] = 0;
	//Elimina los espacios de memoria
	free(tupla[0]);
	free(tupla[1]);
	free(tupla);
	my_thread_yield();
	my_thread_end(NULL);//el hilo muere
	return NULL;
}
