#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mypthreads.h>

#include <time.h>
#include <ruta_moviles.h>

//numero de hilos, se puede definir desde una funcion
//#define NTHREADS	8
#define NOT_RT		-1


void*pausa(void*arg){
	mythread_yield();
	while(1){
	sleep(1);
	print_matriz();
	mythread_yield();
	}
	
	return NULL;
}

/*
 * ejemplo de uso de la biblioteca
 */
int main(int argc, char *argv[])
{
	srand (time(NULL));
	autos*lis = inicia_lista_carros();

	printf("largo %d\n", lis->len);

	mythread_t threads[NTHREADS];
	//int count[NTHREADS];
	int i;
	char *status;
	mythread_init();
	//imprimir matriz
	int x = 0;
	for(; x < 4;x++){
		mythread_create(&threads[x], NULL, arrancar_carro, &((lis->list_carros)[x]), NOT_RT);
	}
	mythread_create(&threads[x], NULL, control_semaforos, &((lis->list_carros)[x]), 1);x++;
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
