#include <stdio.h>
#include <stdlib.h>

void leggiFile(int ***lista, int *E, int *N);
void vertex(int arc[], int N, int k, int start, int pos, int sol[], int **lista, int E);
int check(int sol[], int **lista, int E, int pos);

int main() {
    int **lista, E, N;

    leggiFile(&lista, &E, &N);
    int arc[N], sol[N];
    for(int i = 0; i<N; i++) {
        arc[i] = i;
    }
    for (int k = 1; k <= N; k++) {
        vertex(arc, N, k, 0, 0, sol, lista, E);
    }


    for (int i = 0; i < E; i++) {
        free(lista[i]);
    }
    free(lista);
    return 0;
}

void leggiFile(int ***lista, int *E, int *N) {
    FILE *fp = fopen("grafo.txt" , "r");
    if (fp == NULL) {
        printf("Errore nell'apertura del file.\n");
        return;
    }
    fscanf(fp,"%d %d", N, E);
    *lista = malloc(*E * sizeof(int*));
    for(int i = 0; i < *E; i++) {
        (*lista)[i] = malloc(2 * sizeof(int));
        fscanf(fp,"%d %d", &((*lista)[i][0]), &((*lista)[i][1]));
    }
    fclose(fp);
}

void vertex(int arc[], int N, int k, int start, int pos, int sol[], int **lista, int E) {
    if (pos == k) {
        int ok = check(sol, lista, E, pos);
        if (ok == 1) {
            for (int i=0; i< k; i++) {
                printf("%d", sol[i]);
            }
            printf("\n");
        }
        return;
    }
    for (int i = start; i < N; i++) {
        sol[pos] = arc[i];
        vertex(arc, N, k, i+1, pos +1, sol, lista, E);
    }
}

int check(int sol[], int **lista, int E, int pos) {
        int ok;
        for (int i = 0; i<E; i++) {
            ok = 0;
            for(int k = 0; k < pos; k++) {
                if (sol[k] == lista[i][0] || sol[k] == lista[i][1]) {
                    ok = 1;
                    break;
                }
            }
            if (!ok) {
                return 0;
            }
        }
    return 1;
}