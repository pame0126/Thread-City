/**
 * file: dijkstra.c
 * author: yusuf shakeel
 * date: 2014-03-03
 *
 * description: find shortest path betweem two vertices
 *
 * vertices are represented using numbers.
 * vertex A becomes 0
 * vertex B becomes 1
 * and so on...
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * contant to represent infinity
 * it is assumed that no edge in the vertex will have weight equal
 * to this value.
 */
#define INF 9999

/**
 * total number of vertices in the graph
 */
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
		{INF,1,INF,INF,INF,INF,INF,INF,0,1,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,1,INF,INF,INF,INF,INF,INF,0,   1,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,1,INF,INF,INF,INF,INF,INF, 0,1,INF,INF,INF,INF,INF,INF,INF,INF,      INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,  INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,1,INF,INF,INF,INF,INF, INF,0,1,INF,INF,INF,INF,INF,INF,INF,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,INF,1,INF,INF,INF,INF, INF,INF,0,1,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},
		{INF,INF,INF,INF,INF,INF,1,INF,INF,INF, INF,INF,INF,0,1,INF,INF,INF,INF,INF,   INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,      INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,     INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,    INF,INF,INF},

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
	 * inicio o destino
	 */
	int src = 0;
	int dest = 4;

	/**
	 * find shortest path
	 */
	dijkstra(matriz, src, dest);

	return 0;
}
