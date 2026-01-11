#include <stdio.h>
#include <stdlib.h>

#define nomeFile "testo.txt"

int leggiFile(int ***mat, int *nr, int *nc);
void malloc2dP(int ***mat, int nr, int nc);
void separa(int **mat, int nr, int nc, int **neri, int **bianchi, int nNeri, int nBianchi);
void stampa(int *vettore, int n);
void free2d(int **mat, int nr);

int main(void) {
    int **mat;
    int nr, nc;
    int *neri, *bianchi;

    leggiFile(&mat, &nr, &nc);
    int nNeri = (nr * nc + 1) / 2;
    int nBianchi = (nr * nc) / 2;
    separa(mat, nr, nc, &neri, &bianchi, nNeri, nBianchi);

    printf("Vettore biachi: \n");
    stampa(bianchi, nBianchi);
    printf("Vettore neri: \n");
    stampa(neri, nNeri);

    free2d(mat, nr);
    free(neri);
    free(bianchi);
    return 0;
}

int leggiFile(int ***mat, int *nr, int *nc) {
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s\n", nomeFile);
        exit(1);
    }

    fscanf(fp, "%d %d", nr, nc);
    malloc2dP(mat, *nr, *nc);

    for (int i = 0; i < *nr; i++) {
        for (int j = 0; j < *nc; j++) {
            fscanf(fp, "%d", &(*mat)[i][j]);
        }
    }

    fclose(fp);
    return 0;
}

void malloc2dP(int ***mat, int nr, int nc) {
    *mat = malloc(nr * sizeof(int*));
    if (*mat == NULL) {
        printf("Errore allocazione righe!\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        (*mat)[i] = malloc(nc * sizeof(int));
        if ((*mat)[i] == NULL) {
            printf("Errore allocazione colonne!\n");
            exit(1);
        }
    }
}

void free2d(int **mat, int nr) {
    for (int i = 0; i < nr; i++)
        free(mat[i]);
    free(mat);
}

void separa(int **mat, int nr, int nc, int **neri, int **bianchi, int nNeri, int nBianchi) {
    int kNeri = 0;
    int kBianchi = 0;
    *neri = malloc(nNeri * sizeof(int));
    *bianchi = malloc(nBianchi * sizeof(int));
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            if ((i+j)%2==0) {
                (*neri)[kNeri++] = mat[i][j];
            } else if ((i+j)%2== 1) {
                (*bianchi)[kBianchi++] = mat[i][j];
            }
        }
    }
}

void stampa(int *vettore, int n) {
    for (int i = 0; i < n; i++) {
        printf(" %d ", vettore[i]);
    }
    printf("\n");
}