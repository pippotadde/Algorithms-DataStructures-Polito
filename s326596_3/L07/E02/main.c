#include "ginnastica.h"

int main() {
    int DD, DP;

    printf("Inserire DD: ");
    if(scanf("%d", &DD)<=0) return 1;
    printf("Inserire DP: ");
    if(scanf("%d", &DP)<=0) return 1;

    lista_t lista = lista_init();
    FILE *fp = fopen("elementi.txt", "r");
    if (fp == NULL) {
        printf("Errore apertura file elementi.txt\n");
        return 1;
    }
    leggiFile(lista, fp);
    fclose(fp);

    atleta_t atleta = atleta_init();

    printf("\nCalcolo soluzione ottima (un pò lento ma funziona)...\n");
    if (crea_esercizio(atleta, lista, DD, DP)) {
        stampa(atleta);
    } else {
        printf("Nessuna soluzione trovata\n");
    }

    lista_free(lista);
    atleta_free(atleta);
    return 0;
}