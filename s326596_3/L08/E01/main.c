#include "esercizio.h"

int main(void) {
    int DD, DP;

    printf("Inserire DD: ");
    scanf("%d", &DD);
    printf("Inserire DP: ");
    scanf("%d", &DP);

    lista_t lista = lista_init();
    FILE *fp = fopen("elementi.txt", "r");
    if (fp == NULL) {
        lista_free(lista);
        return 1;
    }
    leggiFile(lista, fp);
    fclose(fp);

    atleta_t atleta = atleta_init();
    crea_esercizio(atleta, lista, DD, DP);
    stampa(atleta);

    lista_free(lista);
    atleta_free(atleta);
    return 0;
}