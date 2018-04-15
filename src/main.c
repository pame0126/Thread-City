#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mypthread.h>

#include <time.h>
#include <ruta_moviles.h>

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
	my_thread_yield();
	*count = *count + 50;
	LOG_PRINTF("Thread %ld: Incremented count by 50 and will now exit\n", (unsigned long)mythread_self().tid);
	my_thread_end(NULL);
	return NULL;
}

void*prints(void*arg){
	printf("INICIO\n");
	my_thread_yield();
	my_thread_end(NULL);
	return NULL;
}

void*pausa(void*arg){
	my_thread_yield();
	while(1){
	sleep(1);
	print_matriz();
	my_thread_yield();
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
	my_thread_init();
	//imprimir matriz
	int x = 0;
	for(; x < 4;x++){
		my_thread_create(&threads[x], NULL, arrancar_carro, &((lis->list_carros)[x]), NOT_RT);
	}
	my_thread_create(&threads[x], NULL, control_semaforos, &((lis->list_carros)[x]), 1);x++;
	my_thread_create(&threads[x], NULL, puente_un_carril, &((lis->list_carros)[x]), 1);x++;
	my_thread_create(&threads[x], NULL, pausa, &((lis->list_carros)[x]), 1);

	my_thread_chsched(0);
	
	for (i = 0; i < x; i++) {
		//arreglar el error 
		my_thread_join(threads[i], (void **)&status);
	}
	my_thread_end(NULL);
	
	return 0;
}
