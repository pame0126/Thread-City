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
		printf("%d %d\n",a,b);
		dimension--;
	}
	int**res;
	for(int i = 0;i < 20;i++){
		res = genera_ruta();
		printf("%d\n",res[0][0]);
	}
	
	
	/*srand (time(NULL));
	
	mythread_t threads[NTHREADS];
	
	int i;
	char *status;
	mythread_init();
	//imprimir matriz
	int x = 0;
	//cruza el puente, falta validar si se queda en medio del puente el carro
	 // Usar cuando se tenga el generado de caminos
	 //arreglar el semaforo
	for(; x < 3;x++){
		mythread_create(&threads[x], NULL, carro, NULL, 1);
	}
	mythread_create(&threads[x], NULL, planta_nuclear, NULL, 7);x++;
	mythread_create(&threads[x], NULL, barco, NULL, 2);x++;
	mythread_create(&threads[x], NULL, puente_un_carril, NULL, 3);x++;
	mythread_create(&threads[x], NULL, pausa, NULL, 5);

	mythread_chsched(0);
	
	for (i = 0; i < x; i++) {
		mythread_join(threads[i], (void **)&status);
	}
	
	mythread_end(NULL);*/
	
	return 0;
}
