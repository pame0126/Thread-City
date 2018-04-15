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




void*prints(void*arg){
	printf("INICIO\n");
	mythread_yield();
	mythread_end(NULL);
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
	int count[NTHREADS];
	int i;
	char *status;
	mythread_init();

	for(int x = 0; x < 10;x++){
		mythread_create(&threads[x], NULL, arrancar_carro, &(count[x]), NOT_RT);
	}

	mythread_chsched(2);

	for (i = 0; i < 10; i++) {

		mythread_join(threads[i], (void **)&status);

	}
	mythread_end(NULL);

	return 0;
}
