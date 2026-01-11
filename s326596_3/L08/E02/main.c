#include <stdio.h>
#include "grafo.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Errore argomenti");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Errore apertura file");
        return 1;
    }

    graph_t G = GRAPHload(fp);
    fclose(fp);

    int scelta;
    char n1[MAXC], n2[MAXC], n3[MAXC];
    int list_fatta = 0;

    do {
        printf("\n--- MENU ---\n");
        printf("1. Ordina (alfabetico) + stampa matrice\n");
        printf("2. Verifica 3-clique su MATRICE\n");
        printf("3. Genera LISTE di adiacenza da MATRICE + stampa lista\n");
        printf("4. Verifica 3-clique su LISTA di adiacenza\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        switch (scelta) {
            case 1:
                GRAPHorder(G);
                GRAPHprintMatrix(G);
                list_fatta = 0;
                break;
            case 2:
                printf("Inserisci 3 nomi di vertici: ");
                if (scanf("%30s %30s %30s", n1, n2, n3) != 3) {
                    printf("Input non valido.\n");
                    break;
                }
                if (GRAPHcheck3Matrix(G, n1, n2, n3) == 1)
                    printf("I nodi formano un sottografo completo (MATRICE).\n");
                else
                    printf("I nodi NON formano un sottografo completo (MATRICE).\n");
                break;
            case 3:
                GRAPHconvertToList(G);
                GRAPHprintList(G);
                list_fatta = 1;
                break;
            case 4:
                if (list_fatta == 0) {
                    printf("Lista non generata. Esegui prima l'opzione 3.\n");
                    break;
                }
                printf("Inserisci 3 nomi di vertici: ");
                if (scanf("%30s %30s %30s", n1, n2, n3) != 3) {
                    printf("Input non valido.\n");
                    break;
                }
                if (GRAPHcheck3List(G, n1, n2, n3) == 1)
                    printf("I nodi formano un sottografo completo (LISTA).\n");
                else
                    printf("I nodi NON formano un sottografo completo (LISTA).\n");
                break;
            case 0:
                break;
            default:
                printf("Scelta non valida.\n");
        }
    } while (scelta != 0);
    GRAPHfree(G);
    return 0;
}
