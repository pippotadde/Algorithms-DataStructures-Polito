#include <stdio.h>
#include <stdlib.h>

#define nomeFile "att1.txt"

typedef struct {
    int si;
    int fi;
    int di;
} att;

int leggiFile(att **V);
void attSel(int N, att *V);
int comb(att *V, int N, int i, int somma, int *lista, int k, int *bestLista, int *bestK, int *bestSomma);
void stampa(att *V, int *bestLista, int bestSomma, int bestK);

int main() {
    att *V;
    int N = leggiFile(&V);
    attSel(N, V);
    free(V);
    return 0;
}

int leggiFile(att **V) {
    int n;
    FILE *fp = fopen(nomeFile, "r");
    if (!fp) { printf("Errore apertura file\n"); exit(1); }
    fscanf(fp, "%d", &n);
    *V = malloc(n * sizeof(att));
    if (!*V) { printf("Errore malloc\n"); exit(1); }
    for (int i=0; i<n; i++) {
        fscanf(fp, "%d %d", &(*V)[i].si, &(*V)[i].fi);
        (*V)[i].di = (*V)[i].fi-(*V)[i].si;
    }
    fclose(fp);
    return n;
}

void attSel(int N, att *V) {
    int lista[N];
    int bestLista[N];
    int k = 0;
    int bestK = 0;
    int bestSomma = 0;
    for (int i = 0; i < N; i++) {
        lista[0] = i;
        comb(V, N, i, V[i].di, lista, 1, bestLista, &bestK, &bestSomma);
    }
    stampa(V, bestLista, bestSomma, bestK);
}

int comb(att *V, int N, int i, int somma, int *lista, int k, int *bestLista, int *bestK, int *bestSomma) {
    if (somma > *bestSomma) {
        *bestSomma = somma;
        *bestK = k;
        for (int p = 0; p < k; p++) {
            bestLista[p] = lista[p];
        }
    }
    for(int j=0; j<N; j++) {
        if (V[i].fi<=V[j].si) {
            lista[k] = j;
            comb(V, N, j, somma + V[j].di, lista, k + 1, bestLista, bestK, bestSomma);
        }
    }
}

void stampa(att *V, int *bestLista, int bestSomma, int bestK) {
    printf("\n");
    printf("Attività compatibili con somma massima:\n{ ");
    for (int i = 0; i < bestK; i++) {
        int idx = bestLista[i];
        printf("(%d, %d) ", V[idx].si, V[idx].fi);
    }
    printf("}\nSomma = %d\n", bestSomma);
}