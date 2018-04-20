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
#define INF 9999

/**
 * total number of vertices in the graph
 */
#define V 110

/**
 * Valor minimo
 */
int Minimo(int x, int y) {
	if (x < y) {
		return x;
	}
	return y;
}

/**
 * revisa si esta marcado, el elemento de la lista
 */
int EstaMarcado(int v, int VerticesMarcados[], int markedVerticesIdx) {

	int i = 0;
	for (i = 0; i < markedVerticesIdx; i++) {
		if (v == VerticesMarcados[i]) {
			return 1;
		}
	}

	return 0;
}

/**
 * this function will find shortest path between source and destination vertex
 */
void dijkstra(int graph[V][V], int ini, int dest) {

	//variables
	int i, r, c,
		tmpC,
		min,
		ActualVertex,
		finalWt = 0,
		destValor = 0,
		ValorMarcado = 0,
		wtTablaR = 0,
		VerticesMarcadosIdx = 0;

	/**
	 * vertices del arreglo del thread
	 */
	int verticesCaminoCorto[V] = {-1};
	int CaminoCortoVerticeIdx = 0;

	/**
	 * guarda el peso
	 */
	int TablaPesos[V][V];
	for (r = 0; r < V; r++) {
		for (c = 0; c < V; c++) {
			TablaPesos[r][c] = INF;
		}
	}
	TablaPesos[wtTablaR++][ini] = 0;

	/**
	 * marca los pesos
	 */
	int VerticesMarcados[V] = {-1};
	VerticesMarcados[VerticesMarcadosIdx++] = ini;
	ActualVertex = ini;

	/**
	 * encuentra camino
	 */
	while(ActualVertex != dest) {

		/**
		 * copy marked values to the next row of weightTable
		 */
		for (i = 0; i < VerticesMarcadosIdx; i++) {
			c = VerticesMarcados[i];
			TablaPesos[wtTablaR][c] = TablaPesos[wtTablaR - 1][c];
		}

		/**
		 * find the weight from the current vertex to all the other
		 * vertices that are directly connected and not yet marked
		 */
		for (c = 0; c < V; c++) {

			if (c != ActualVertex && !EstaMarcado(c, VerticesMarcados, VerticesMarcadosIdx)) {

				finalWt = graph[ActualVertex][c];
				destValor = TablaPesos[wtTablaR - 1][c];
				ValorMarcado = TablaPesos[wtTablaR][ActualVertex];

				min = Minimo(destValor, ValorMarcado + finalWt);

				TablaPesos[wtTablaR][c] = min;

			}

		}

		/**
		 * find minimum unmarked vertices in current row
		 */
		min = INF;
		for (c = 0; c < V; c++) {

			if (!EstaMarcado(c, VerticesMarcados, VerticesMarcadosIdx)) {
				if (TablaPesos[wtTablaR][c] < min) {
					min = TablaPesos[wtTablaR][c];
					tmpC = c;
				}
			}

		}

		/**
		 * found a new vertex for marking
		 */
		VerticesMarcados[VerticesMarcadosIdx++] = tmpC;
		ActualVertex = tmpC;

		/**
		 * variables update
		 */
		wtTablaR++;

	}

	/**
	 * compute shortest path vertices
	 */
	c = dest;
	verticesCaminoCorto[CaminoCortoVerticeIdx++] = c;
	ValorMarcado = TablaPesos[wtTablaR - 1][dest];
	for (r = wtTablaR - 2; r >= 0; r--) {

		if (TablaPesos[r][c] != ValorMarcado) {
			c = VerticesMarcados[r];
			ValorMarcado = TablaPesos[r][c];
			verticesCaminoCorto[CaminoCortoVerticeIdx++] = c;
		}

	}

	/**
	 * display the weight and shortest path
	 */
	printf("Camino mas corta %d and %d\n", ini, dest);
	for (i = CaminoCortoVerticeIdx-1; i >= 0; i--) {
		printf("%d",verticesCaminoCorto[i]);
		if (i > 0) {
			printf(" --> ");
		}
	}
	printf("\n");
	printf("Peso del camino: %d\n", TablaPesos[wtTablaR-1][dest]);

}



int main(int argc, char *argv[])
{
	/*
	 * Este codigo sirve, no quitar!!!
	int a, b;
	int dimension;
	scanf("%d",&dimension);
	int matriz[dimension][dimension];
	for(int i = 0;i < dimension;i++){
		for(int j = 0;j < dimension;j++){
			matriz[i][j] = INF;
		}		
	}
	while(scanf("%d %d",&a, &b )!=EOF){
		printf("%d %d\n",a,b);
		matriz[a][b] = 1;
	}
	//diagonales = 0
	for (int i = 0;i < dimension;i++){
		matriz[i][i] = 0;
	}
	//inicio y destino
	int src = 31;
	int dest = 91;

	dijkstra(matriz, src, dest);*/

	srand (time(NULL));
	
	mythread_t threads[NTHREADS];
	
	int i;
	char *status;
	mythread_init();
	//imprimir matriz
	int x = 0;
	//cruza el puente, falta validar si se queda en medio del puente el carro
	 // Usar cuando se tenga el generado de caminos
	 //arreglar el semaforo
	for(; x < 1;x++){
		mythread_create(&threads[x], NULL, carro, NULL, 1);
	}
	mythread_create(&threads[x], NULL, barco, NULL, 2);x++;
	mythread_create(&threads[x], NULL, puente_un_carril, NULL, 3);x++;
	mythread_create(&threads[x], NULL, pausa, NULL, 5);

	mythread_chsched(0);
	
	for (i = 0; i < x; i++) {
		mythread_join(threads[i], (void **)&status);
	}
	
	mythread_end(NULL);
	
	return 0;
}
