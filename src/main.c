#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mypthreads.h>

#include <time.h>
#include <ruta_moviles.h>
#include "recorrido.h"

//numero de hilos, se puede definir desde una funcion
//#define NTHREADS	8
#define NOT_RT		-1



/*
 * aumenta el contador en 50,
 * cuando tiene el control de regreso le suma otros 50 y sale
 */
void *thread_func(void *arg)
{
	int *count = (int *)arg;

	*count = *count + 50;
	LOG_PRINTF("Thread %ld: Incremented count by 50 and will now yield\n", (unsigned long)mythread_self().tid);
	mythread_yield();
	*count = *count + 50;
	LOG_PRINTF("Thread %ld: Incremented count by 50 and will now exit\n", (unsigned long)mythread_self().tid);
	mythread_end(NULL);
	return NULL;
}

void*prints(void*arg){
	printf("INICIO\n");
	mythread_yield();
	mythread_end(NULL);
	return NULL;
}

void*pausa(void*arg){
	mythread_yield();
	while(1){
	sleep(1);
	print_matriz();
	mythread_yield();
	}

	return NULL;
}

void cargarMatriz()
{
	int largo, ancho, tmp, i = 0, j = 0;
	scanf("%d %d", &largo, &ancho);
	while(i < largo)
	{
		j = 0;
		while (j < ancho) {
			scanf("%d", &tmp);
			matriz[i][j] = tmp;
			j++;
		}
		printf("\n");
		i++;
	}
}

void imprimirMatriz()
{
	int largo = 63, ancho = 63, i = 0, j = 0;
	while(i < largo)
	{
		j = 0;
		while (j < ancho) {
			printf("%d ", matriz[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

/*
 * ejemplo de uso de la biblioteca
 */
int main(int argc, char *argv[])
{
	srand (time(NULL));
	autos*lis = inicia_lista_carros();
	cargarMatriz();

	printf("largo %d\n", lis->len);

	mythread_t threads[NTHREADS];
	//int count[NTHREADS];
	int i;
	char *status;
	mythread_init();
	//imprimir matriz
	int x = 0;
	/*
	for(; x < 4;x++){
		mythread_create(&threads[x], NULL, arrancar_carro, &((lis->list_carros)[x]), NOT_RT);
	}*/
	//mythread_create(&threads[x], NULL, control_semaforos, &((lis->list_carros)[x]), 1);x++;
	mythread_create(&threads[x], NULL, puente_un_carril, &((lis->list_carros)[x]), 1);x++;
	mythread_create(&threads[x], NULL, pausa, &((lis->list_carros)[x]), 1);

	mythread_chsched(0);

	for (i = 0; i < x; i++) {
		//arreglar el error
		mythread_join(threads[i], (void **)&status);
	}
	mythread_end(NULL);

	return 0;
}
