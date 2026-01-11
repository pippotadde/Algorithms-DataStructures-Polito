#include <stdio.h>
#include <stdlib.h>

void leggiFile(char ****lista, int *N, int *n);
int platylist(int pos, char ***lista,  char **sol,  int N, int cnt);

int main() {
    char ***lista;
    int N, n;

    leggiFile(&lista, &N, &n);
    char **sol = malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++){
        sol[i] = malloc(255);
    }
    platylist(0, lista, sol, N, 0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < n; j++) { free(lista[i][j]); }
        free(lista[i]);
    }
    free(lista);
    return 0;
}

void leggiFile(char ****lista, int *N,int *n) {
    FILE *fp = fopen("brani.txt", "r");
    if (fp == NULL) { return; }
    fscanf(fp, "%d", N);
    (*lista) = malloc((*N) * sizeof(char **));
    for (int i = 0; i < (*N); i++) {
        fscanf(fp, "%d", n);
        (*lista)[i] = malloc((*n + 1) * sizeof(char *));
        for (int j = 0; j < (*n); j++) {
            (*lista)[i][j] = malloc(255);
            fscanf(fp, "%254s", (*lista)[i][j]);
        }
        (*lista)[i][*n] = NULL;
    }
}

int platylist(int pos, char ***lista,  char **sol, int N, int cnt) {
    if (pos == N) {
        for (int i = 0; i < N; i++) {
            printf(" ");
            printf("%s", sol[i]);
        }
        printf("\n");
        return (cnt+1);
    }
    for (int i = 0; lista[pos][i] != NULL; i++){
        sol[pos] = lista[pos][i];
        cnt = platylist(pos+1, lista, sol, N, cnt);
    }
    return cnt;
}