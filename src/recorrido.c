

#include <stdio.h>
#include <stdlib.h>


#define INF 9999


#define V 15

/**
 * Valor minimo
 */
int Minimo(int x, int y) {
	if (x < y) {
		return x;
	}
	return y;
}
//retorna fila del elemento
int returnFila(int valor){
	int re = valor%10;
	//printf("f %d  %d\n",re, valor);
	return re;
}
//retorna filas del arreglo
int* returnFilas(int* arreglo){
	int tan = arreglo[0];
	int j=0;
	int* arreglox=(int*)calloc(tan, sizeof(int));
	arreglox[0]=tan;
	for(int i=1;i<=tan;i++){
		arreglox[i]=returnFila(arreglo[i]);
	}
	return arreglox;
}
//retorna columna del valor
int returnColumna(int valor){
	int re= valor/10;
	//printf("c %d  %d\n",re, valor);
	return re;
}
//retorna columnas del arreglo
int* returnColumnas(int* arreglo){
	int tan = arreglo[0];
	int j=0;
	int* arregloy=(int*)calloc(tan, sizeof(int));
	arregloy[0]=tan;
	for(int i=1;i<=tan;i++){
		arregloy[i]=returnColumna(arreglo[i]);
	}
	return arregloy;
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
 * encuentra el camino mas corto
 */
int* dijkstra(int graph[V][V], int ini, int dest) {

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
		 * copia valores marcados al siguente linea de peso
		 */
		for (i = 0; i < VerticesMarcadosIdx; i++) {
			c = VerticesMarcados[i];
			TablaPesos[wtTablaR][c] = TablaPesos[wtTablaR - 1][c];
		}

		/**
		 * encuentra peso del peso del vertice a los demas vertices
		 * conectados y que no han sido marcados
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
		 * encuentra vertices mas pequeno no marcado 
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
		 * nuevo vertice para marcar
		 */
		VerticesMarcados[VerticesMarcadosIdx++] = tmpC;
		ActualVertex = tmpC;
		
		/**
		 * variables actualizadas
		 */
		wtTablaR++;

	}

	/**
	 * vertices del camino mas corto
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
	 * imprime camino
	 */
	int* solucion=(int*)calloc(CaminoCortoVerticeIdx, sizeof(int));
	printf("Camino mas corta %d and %d\n", ini, dest);
	solucion[0]=CaminoCortoVerticeIdx;
	int j=1;
	for (i = CaminoCortoVerticeIdx-1; i >= 0; i--) {
		printf("%d",verticesCaminoCorto[i]);
		solucion[j]=verticesCaminoCorto[i];
		if (i > 0) {
			printf(" --> ");
		}
		j++;
	}
	printf("\n");
	printf("Peso del camino: %d\n", TablaPesos[wtTablaR-1][dest]);
	return solucion;

}

int main(void) {

	/**
	 * arreglo de la matriz
	 */	
	int graph[V][V] = {
		{0,1,1,INF,1},
		{1,0,1,1,INF},
		{1,1,0,1,1},
		{INF,1,1,0,1},
		{1,INF,1,1,0},
	};
	/*int matriz[63][63]={
		//0  1   2  3   4   5   6  7 8    9     10  11  12  13  14  15  16  17  18 19
		{0,INF,INF,INF,INF,INF,INF,1,INF,INF,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,0,INF,INF,INF,INF,INF,INF,1,INF,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,0,INF,INF,INF,INF,INF,INF,1,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,0,INF,INF,INF,INF,INF,INF, 1,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,0,INF,INF,INF,INF,INF, INF,1,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,INF,0,INF,INF,INF,INF, INF,INF,1,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,INF,INF,0,INF,INF,INF, INF,INF,INF,1,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		
		{1,INF,INF,INF,INF,INF,INF,0,1,INF,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,1,INF,INF,INF,INF,INF,INF,0,1,     INF,INF,INF,INF,INF,1,INF,INF,INF,INF,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,1,INF,INF,INF,INF,INF,INF,0,   1,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,1,INF,INF,INF,INF,INF,INF, 0,1,INF,INF,INF,INF,INF,1,INF,INF,      INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,1,INF,INF,INF,INF,INF, INF,0,1,INF,INF,INF,INF,INF,INF,INF,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,INF,1,INF,INF,INF,INF, INF,INF,0,1,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,INF,INF,1,INF,INF,INF, INF,INF,INF,0,1,INF,INF,INF,INF,1,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,      INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		
		{1,INF,1,1,0},
		;*/
int matriz[V][V]={
		//0  1   2  3   4   5   6  7 8    9     10  11  12  13  14  15  16  17  18 19
		{0,INF,INF,INF,INF,INF,INF,1,INF,INF,  INF,INF,INF,INF,INF  },
		{INF,0,INF,INF,INF,INF,INF,INF,1,INF,  INF,INF,INF,INF,INF  },
		{INF,INF,0,INF,INF,INF,INF,INF,INF,1,  INF,INF,INF,INF,INF   },
		{INF,INF,INF,0,INF,INF,INF,INF,INF,INF, 1,INF,INF,INF,INF  },
		{INF,INF,INF,INF,0,INF,INF,INF,INF,INF, INF,1,INF,INF,INF  },
		{INF,INF,INF,INF,INF,0,INF,INF,INF,INF, INF,INF,1,INF,INF },
		{INF,INF,INF,INF,INF,INF,0,INF,INF,INF, INF,INF,INF,1,INF },
		
		{1,INF,INF,INF,INF,INF,INF,0,1,INF,     INF,INF,INF,INF,INF},
		{INF,1,INF,INF,INF,INF,INF,INF,0,1,     INF,INF,INF,INF,INF},
		{INF,INF,1,INF,INF,INF,INF,INF,INF,0,   1,INF,INF,INF,INF},
		{INF,INF,INF,1,INF,INF,INF,INF,INF,INF, 0,1,INF,INF,INF},
		{INF,INF,INF,INF,1,INF,INF,INF,INF,INF, INF,0,1,INF,INF},
		{INF,INF,INF,INF,INF,1,INF,INF,INF,INF, INF,INF,0,1,INF},
		{INF,INF,INF,INF,INF,INF,1,INF,INF,INF, INF,INF,INF,0,1}};
		
	/**
	 * inicio y destino
	 */
	int src = 0;
	int dest = 4;

	/**
	 * encuentra camino mas corto
	 */
	int* solucion = (int*)calloc(50, sizeof(int));
	solucion=dijkstra(matriz, src, dest);
	for(int i=0;i<=solucion[0];i++){
	  printf(" %d ",solucion[i]);	
	}
	printf("\n");
	int *x =(int*)calloc(solucion[0], sizeof(int));
	x=returnColumnas(solucion);
	for(int i=0;i<=solucion[0];i++){
	  printf(" %d ",x[i]);	
	}
	printf("\n");
	int *y =(int*)calloc(solucion[0], sizeof(int));
	y=returnFilas(solucion);
	for(int i=0;i<=solucion[0];i++){
	  printf(" %d ",y[i]);	
	}
	printf("\n");
	return 0;
}
