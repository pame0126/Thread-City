#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <ruta_moviles.h>
#include <mypthreads.h>
#include <recorrido.h>

int CANT_RUTAS_QUEMADAS = 0;

int ID_PROGRES = 1;
int murio_planta_nuclear = 0;

/* Se llama para que haga una pausa de 1 segundo
 */
void*pausa(void*arg){
	mythread_yield();
	while(1){
	sleep(1);
	print_matriz();
	mythread_yield();
	}
	
	return NULL;
}

/* Ruta quemada de posicion de matriz (3, 2) --> (9, 2)
 */
int **ruta_q_A(){
	int *rutaX = calloc(12, sizeof(int));
	int *rutaY = calloc(12, sizeof(int));
	//largo 7
	rutaX[0] = 8; rutaY[0] = 8;
	int x = 3, y = 2;
	rutaX[1] = x; rutaY[1] = y;
	int i = 2;
	for(; i < 8;i++){
		x++;
		rutaX[i] = x; rutaY[i] = y;
	}
	int **res = (int**)calloc(2, sizeof(int*));
	res[0] = rutaX; res[1] = rutaY;
	return res;
}

/* Ruta quemada de posicion de matriz (3, 6) --> (9, 6)
 */
int **ruta_q_B(){
	int *rutaX = calloc(12, sizeof(int));
	int *rutaY = calloc(12, sizeof(int));
	//largo 7
	rutaX[0] = 8; rutaY[0] = 8;
	int x = 3, y = 6;
	rutaX[1] = x; rutaY[1] = y;
	int i = 2;
	
	for(; i < 8;i++){
		x++;
		rutaX[i] = x; rutaY[i] = y;
	}
	int **res = (int**)calloc(2, sizeof(int*));
	res[0] = rutaX; res[1] = rutaY;
	return res;
}

/* Ruta quemada de posicion de matriz (9, 7) --> (3, 7)
 */
int **ruta_q_C(){
	int *rutaX = calloc(12, sizeof(int));
	int *rutaY = calloc(12, sizeof(int));
	//largo 7
	rutaX[0] = 8; rutaY[0] = 8;
	int x = 9, y = 7;
	rutaX[1] = x; rutaY[1] = y;
	int i = 2;
	
	for(; i < 8;i++){
		x--;
		rutaX[i] = x; rutaY[i] = y;
	}
	int **res = (int**)calloc(2, sizeof(int*));
	res[0] = rutaX; res[1] = rutaY;
	return res;
}


/* rutas predefinidas quemadas para pruebas
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
		default:
			//res = ruta_q_A();
			break;
	}
	CANT_RUTAS_QUEMADAS++;
	CANT_RUTAS_QUEMADAS = CANT_RUTAS_QUEMADAS%4;
	return res;
}


/* imprime la martiz de la cuidad
 * */
void *print_matriz(){
	//imprimir la ciudad	
	for(int i = 0; i < SIZE_X;i++){
		printf("\t");
		for(int j = 0;j < SIZE_Y;j++){
			//pintar el semaforo
			if(matriz_ciudad[i][j] == ROJO){
				printf("\e[31m%s\e[0m  ", "🚦");
			}
			//pintar el puente bloqueado
			else if(matriz_ciudad[i][j] == PUENTE_BLOQUEADO){
				printf("\e[94m%s\e[0m ", "⛆");
			}
			//pintar el puente
			else if(matriz_ciudad[i][j] == PUENTE){
				printf("   ");
			}
			else if(matriz_ciudad[i][j] == PUENTE_ESPERA){
				printf("   ");
			}
			//pintar el barco
			else if(matriz_ciudad[i][j] == BARCO){
				printf("\e[33m%s\e[0m", "⛴");
			}
			//pintar el rio
			else if(( (i==5 || i==6 || i==7)&&
					(j==1 || j ==3 || j==4 || j==5 || j == 8) )
					&& matriz_ciudad[i][j] != BARCO){
				printf("\e[94m%s\e[0m", "⛆");
			}
			//planta nuclear
			else if(matriz_ciudad[i][j]==PLANTA_NUCLEAR){
				
				printf("\e[92m%s\e[0m ", "☢️");
			}
			//planta nuclear explotada
			else if(matriz_ciudad[i][j]==PLANTA_DESTRUIDA){
				
				printf("\e[91m%s\e[0m ", "💥");
			}
			
			//ambulancia
			else if(matriz_ciudad[i][j] == AMBULANCIA){
				printf("\e[1m%s\e[0m", "🚑");
			}
			//carros
			else if(matriz_ciudad[i][j] != 0){
				printf("\e[32m%s\e[0m", "🚕");
			}
			//edificios
			else if( (i == 1 && j == 1)||(i == 1 && j == 3)||
				     (i == 3 && j == 1)||(i == 3 && j == 3)|| (i == 3 && j == 5) ||
				     (i == 9 && j == 1)||(i == 9 && j == 3)|| (i == 9 && j == 5) ){
				printf("\e[1m%s\e[0m ", "🏠");
			}
			//hospital
			else if ( (i == 1 && j == 7)||(i == 1 && j == 8)||
				     (i == 3 && j == 7)||(i == 3 && j == 8)||
				     (i == 9 && j == 7)||(i == 9 && j == 8)){
				 printf("\e[1m%s\e[0m ", "🏥");
			}
			else{
				//la calle
				printf("   ");
			}
		}
		printf("\n");
	}
	printf("\t----------------------------\n");
	return NULL;
}

void borrar_poss_anterior(int i ,int j){
	matriz_ciudad[i][j] = 0;
}

/* escoge ruta del carro*/
int**escoger_ruta_carro(){
	int**res;
	int ruta = rand()%100;
	//rutas para la planta, probabilidad del 30 porciento
	if(ruta <= 30 && !murio_planta_nuclear){
		ruta = rand()%ARRAY_PLANTA_NUCLEAR;
		res = rutas_planta_nuclear[ruta];
	}
	//sino, ruta normal
	else{
		ruta = rand()%(R-1);
		res = rutas_carros[ruta];
	}
	
	return res;
}

/* Se crea la ruta para el hilo actual.
 * Esta funcion recorre la ruta recibida con validaciones de choques de carros.
 * Si un carro va a chocar con otro se pone en condicion de dar pase (numero negativo)
 * y espera un turno o los que sean necesarios para no chocar.
 * */
void*carro(void*arg){
	mythread_yield();
	int **tupla;
	int i, j;    //posicion a la que se movera
	int xi, xj;  //posicion actual
	int len;
	
	pid_t id = __mythread_gettid();
	
	int tiempo = 4;
	while(1){
		if(tiempo == 4){
			tupla = escoger_ruta_carro();
			i = 0, j = 0;    //posicion a la que se movera
			xi = 0, xj = 0;  //posicion actual
			len = tupla[0][0];
			
			//recorrer los arreglos de ruta
			for(int a = 1; a < len ;a++){		
				i = tupla[0][a];
				j = tupla[1][a];
				if(a > 1){//si no es el primer movimiento
					//posicion antes del cambio
					xi = tupla[0][a-1];
					xj = tupla[1][a-1];
					mythread_yield();
					///////////////////////////////////////////////////
					if(	matriz_ciudad[i][j] == 0 ){
						matriz_ciudad[i][j] = id;
						borrar_poss_anterior(xi,xj);//borrar el anterior
					}
					//si esta en el puente de dos carriles
					else if( ( ( i == 5 || i == 6 || i == 7 ) && ( j == 6 || j==7 ) ) &&
						matriz_ciudad[i][j] == 0 ){
						matriz_ciudad[i][j] = id;
						borrar_poss_anterior(xi,xj);//borrar el anterior
					}
					//si esta en el puente
					else if(matriz_ciudad[i][j] == PUENTE || 
						  matriz_ciudad[i][j] == PUENTE_ESPERA){
						matriz_ciudad[xi][xj] = matriz_ciudad[i][j];
						matriz_ciudad[i][j] = id;
						//borrar_poss_anterior(xi,xj);//borrar el anterior
							
					}
					//si esta en un semaforo
					else if(matriz_ciudad[i][j] == ROJO &&
						matriz_ciudad[4][2] == ROJO){
						//si estas en el puente bloqueado, salta el rojo
						matriz_ciudad[i][j] = id;
						borrar_poss_anterior(xi,xj);
					}
					//si llego a la planta nuclear
					else if( matriz_ciudad[i][j] == PLANTA_NUCLEAR || 
						matriz_ciudad[i][j] == PLANTA_DESTRUIDA ){
						borrar_poss_anterior(xi,xj);//borrar el anterior
					}
					else{//se queda en el mismo lugar con negativo
						matriz_ciudad[xi][xj] = id;
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
			tiempo = 0;
		}
		tiempo++;
	}
	mythread_end(NULL);//el hilo muere
	return NULL;
}

/* Verifica que en el puente existan o no carros
 */
int existen_carros_puente(int*posX, int posY){
	for (int i = 0; i < 3; i++) {
		if(matriz_ciudad[posX[i]][posY]!= PUENTE){
			return 1;
		}
	}
	return 0;
}

int puente_espera_libre(int*posX, int posY){
	for (int i = 0; i < 3; i++) {
		if(matriz_ciudad[posX[i]][posY]!= PUENTE_ESPERA){
			return 0;
		}
	}
	return 1;
}

/* coloca al puente en un estado de ESPERA para que terminen de salir los autos
 */
void puente_estado_espera(int*posX, int posY){
	
	for (int i = 0; i < 3; i++) {
		if( matriz_ciudad[posX[i]][posY] == 0 || matriz_ciudad[posX[i]][posY] == PUENTE){
			matriz_ciudad[posX[i]][posY] = PUENTE_ESPERA;
		}
	}
}

/* coloca al puente en un estado de BLOQUEO
 */
void puente_estado_bloqueado(int*posX, int posY){
	for (int i = 0; i < 3; i++) {
		matriz_ciudad[posX[i]][posY] = PUENTE_BLOQUEADO;
	}
}

int puente_con_barco_adyacente(int*posX, int posY){
	for (int i = 0; i < 3; i++) {
		if(matriz_ciudad[posX[i]][posY-1] == BARCO){
			return 1;
		}
	}
	return 0;
}

void*puente_un_carril(void*arg){
	mythread_yield();
	int *posX = calloc(3, sizeof(int));
	posX[0] = 5; posX[1] = 6; posX[2] = 7;
	int posY = 2;
	
	//semaforos del puente
	int semaf1X = 4, semaf1Y = 2;
	int semaf2X = 8, semaf2Y = 2;
	//valores del puente
	int valor1 = VERDE, valor2 = VERDE;
	int contador = 4;
	int bandera = 0;
	
	while(1){
		//si tiene un barco a la par por la izquierda o derecha
		if( puente_con_barco_adyacente(posX, posY) ){
				
			//coloca en rojo los semaforos
			matriz_ciudad[semaf1X][semaf1Y] = ROJO;//semaforo arriba
			matriz_ciudad[semaf2X][semaf2Y] = ROJO;//semaforo abajo
			//espera que no haya carros en el puente
			if ( puente_espera_libre(posX,posY) ){
				puente_estado_bloqueado(posX,posY);
			}
			else if ( existen_carros_puente(posX,posY) ){
				puente_estado_espera(posX,posY);
			}
			//Bloqueo el puente
			else{
				puente_estado_bloqueado(posX,posY);
			}
		}
		//si esta pasando el puente
		else if(matriz_ciudad[posX[0]][posY] == BARCO ||
				matriz_ciudad[posX[1]][posY] == BARCO ||
				matriz_ciudad[posX[2]][posY] == BARCO ){
				NULL;
		}
		else{
			//pone el estado de puente en funcionamiento
			for(int i = 0;i < 3;i++){
				matriz_ciudad[posX[i]][posY] = (matriz_ciudad[posX[i]][posY] == 0 ||
												matriz_ciudad[posX[i]][posY] == PUENTE_BLOQUEADO)?
												 PUENTE : matriz_ciudad[posX[i]][posY];
			}
			//cambio de luces para semaforo
			if (contador == 4){
				valor1 =  (bandera)? ROJO : VERDE;
				valor2 = (bandera)? VERDE : ROJO;
				bandera = (bandera)?0:1;
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
 * viajan en linea recta
 */
int**ruta_barco(){
	int**ruta = calloc(2, sizeof(int*));
	int*rutaX = calloc(7, sizeof(int));
	int*rutaY = calloc(7, sizeof(int));
	//punto de inicio y final
	int iniX = (rand()%3)+5;
	int iniY = 1;
	
	int terY = (rand()%3)+3;
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

/*
 * Se maneja solo un barco que sera repetitivo en su creacion
 */
void*barco(void*arg){
	int**ruta;
	int len;
	int i , j;
	int antX, antY;
	int px, py;
	int contador = 10;
	while(1){
		//cada 10 segundos reinicia el mismo barco con otras coordenadas
		if(contador == 10){
			
			ruta = ruta_barco();
			len = ruta[0][0];
			px=-1, py=-1;
			for (int a = 1;a < len;a++){
				i = ruta[0][a];
				j = ruta[1][a];
				if(a > 1){
					antX = ruta[0][a-1]; antY = ruta[1][a-1];//pos anterior
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
					else if(matriz_ciudad[i][j] != PUENTE_BLOQUEADO){
						a--;
					}
				}
				else{
					matriz_ciudad[i][j] = BARCO;
				}
				mythread_yield();
			}
			matriz_ciudad[i][j] = 0;
			contador = 0;
		}
		contador++;
		mythread_yield();
	}
	mythread_end(NULL);
	return NULL;
}

void quitar_ruta_planta_nuclear(){
	int ini = 1;
	for(int i = 0;i < 15;i++){
		//apunta a la ruta anterior
		rutas_carros[ini] = rutas_carros[ini-1];
		ini+=14;
	}
}

/* Solo existira una planta nuclear en la ciudad.
 */
void*planta_nuclear(void*arg){
	mythread_yield();
	int cant_autos = 0;
	int min = 7;
	int time = 0;
	int i = 1, j = 5;//posicion de la planta nuclear
	int bandera = 1;
	matriz_ciudad[i][j] = PLANTA_NUCLEAR;
	//minetras no explote
	while(bandera){
		
		//si llego algun auto a la planta
		cant_autos = ( matriz_ciudad[i][j] != PLANTA_NUCLEAR )? cant_autos+1 : cant_autos;
		
		//si ya pasaron 10 segundos o menos
		//y llegaron 7 autos o mas se reinicia
		if( time == 10 && cant_autos >= min ){
			time = 0;
			cant_autos = 0;
		}
		//sino explota
		else if(time > 10 && cant_autos < min){
			bandera = 0;
			matriz_ciudad[i][j] = PLANTA_DESTRUIDA;
		}
		time++;
		mythread_yield();
	}
	matriz_ciudad[i][j] = PLANTA_DESTRUIDA;
	//explota la planta
	//quitar_ruta_planta_nuclear();
	murio_planta_nuclear = 1;
	mythread_yield();
	mythread_end(NULL);
	return NULL;
}

/*Escoge ruta a los hospitales unicamente*/
int **escoger_ruta_ambulancia(){
	int ruta = rand()%ARRAY_AMBULANCIA;
	return rutas_ambulancias[ruta];
}


/* Ambulancia */
void *ambulancia(void*arg){
	mythread_yield();
	int **tupla;// = escoger_ruta_carro();//rutas_quemadas();
	int i, j;    //posicion a la que se movera
	int xi, xj;  //posicion actual
	int len;
	
	int id = AMBULANCIA;//__mythread_gettid();
	
	int tiempo = 4;
	while(1){
		if(tiempo == 4){
			tupla = escoger_ruta_ambulancia();//rutas_quemadas();
			i = 0, j = 0;    //posicion a la que se movera
			xi = 0, xj = 0;  //posicion actual
			len = tupla[0][0];
			
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
				
					//Evalua el movimiento si no hay choques
						if( matriz_ciudad[i][j] == 0 ){
							matriz_ciudad[i][j] = id;
							borrar_poss_anterior(xi,xj);//borrar el anterior
						}
						//si esta en el puente de dos carriles
						else if( ( ( i == 5 || i == 6 || i == 7 ) && ( j == 6 || j==7 ) ) &&
								matriz_ciudad[i][j] == 0 ){
							matriz_ciudad[i][j] = id;
							borrar_poss_anterior(xi,xj);//borrar el anterior
						}
						//si esta en el puente
						else if(matriz_ciudad[i][j] == PUENTE || 
								  matriz_ciudad[i][j] == PUENTE_ESPERA){
							matriz_ciudad[xi][xj] = matriz_ciudad[i][j];
							matriz_ciudad[i][j] = id;
							//borrar_poss_anterior(xi,xj);//borrar el anterior
							
						}
						//si esta en un semaforo
						else if(matriz_ciudad[i][j] == ROJO &&
								matriz_ciudad[ tupla[0][a-2] ][ tupla[1][a-2] ] == PUENTE_ESPERA){
							//si estas en el puente bloqueado, salta el rojo
								matriz_ciudad[i][j] = id;
								matriz_ciudad[xi][xj] = PUENTE_ESPERA;
						}
						else{//se queda en el mismo lugar con negativo
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
			tiempo = 0;
		}
		tiempo++;
	}
	mythread_end(NULL);//el hilo muere
}
