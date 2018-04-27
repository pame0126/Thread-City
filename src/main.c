#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mypthreads.h>

#include <time.h>
#include <ruta_moviles.h>
#include <recorrido.h>


int main(int argc, char *argv[])
{
	srand (time(NULL));
	//leer la matriz
	int a, b;
	int dimension;
	scanf("%d",&dimension);
	//int matriz[dimension][dimension];
	ini_matriz_nodos();
	//Carga la matriz de adyacencia

	while(dimension){
		scanf("%d %d",&a, &b );
		matriz_nodos[a][b] = 1;
		dimension--;
	}
	//se cargan las rutas de los autos
	cargar_rutas();

	mythread_t threads[NTHREADS];

	// El menu no sirve
	int scheduler = 0;
	printf("1. RoundRobin\n");
	printf("2. Lottery\n");
	printf("3. Real time\n");
	printf("Ingrese el scheduler que desea usar: \n");
	scanf("%d", &scheduler);

	mythread_chsched(scheduler);

	int i;
	char *status;
	mythread_init();
	//imprimir matriz
	int x = 0;
	//cruza el puente, falta validar si se queda en medio del puente el carro
	 // Usar cuando se tenga el generado de caminos
	 //arreglar el semaforo
	for(; x < 5;x++){
		mythread_create(&threads[x], NULL, carro, NULL, 1);
	}
	for(; x < 3;x++){
		mythread_create(&threads[x], NULL, ambulancia, NULL, 10);
	}
	mythread_create(&threads[x], NULL, planta_nuclear, NULL, 7);x++;
	mythread_create(&threads[x], NULL, barco, NULL, 2);x++;
	mythread_create(&threads[x], NULL, puente_un_carril, NULL, 3);x++;
	mythread_create(&threads[x], NULL, pausa, NULL, 5);

	for (i = 0; i < x; i++) {
		mythread_join(threads[i], (void **)&status);
	}

	mythread_end(NULL);

	return 0;
}
