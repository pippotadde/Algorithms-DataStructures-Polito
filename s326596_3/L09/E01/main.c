#include "Graph.h"

int main() {
    char nomefile[30];
    printf("Inserire nome file: ");
    scanf("%29s", nomefile);

    FILE *fp = fopen(nomefile, "r");
    if (fp == NULL) {
        printf("Errore apertura file.\n");
        return 0;
    }
    Graph G = GRAPHload(fp);
    fclose(fp);

    if (G == NULL) return 0;

    printf("\n");
    printf("Grafo caricato");

    int bestK, bestWeight;
    int *removedEdges = costruisciDAG(G, &bestK, &bestWeight);

    printf("\nRisultati:\n");
    printf("Numero minimo archi rimossi (bestK) = %d\n", bestK);
    printf("Peso totale massimo rimosso (bestWeight) = %d\n", bestWeight);

    GRAPHlongestPath(G, removedEdges);

    free(removedEdges);
    GRAPHfree(G);
    return 0;
}