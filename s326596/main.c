#include "es.h"

/* FILE test.txt (passato per argomento) deve avere questo formato:
* - prima riga: intero V = numero di persone
* - poi V righe: Cognome Nome dd/mm/yyyy dd/mm/yyyy   (morte = 0/0/0 se vivo)
* - poi una riga: intero E = numero di relazioni
* - poi E righe: CognomeGenitore NomeGenitore dd/mm/yyyy  CognomeFiglio NomeFiglio dd/mm/yyyy
*/

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("\nErrori argomenti");
        return 1;
    }

    const char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("\nErrore apertura file");
        return 1;
    }
    Graph G = GRAPHload(fp);
    fclose(fp);



    printf("\nTest domanda 5:\n");
    printf("\nInserire informazioni chiave1:");
    link k1 = readKey();
    printf("\nInserire informazioni chiave2:");
    link k2 = readKey();;
    int cmp = KEYcmp(k1, k2);
    printf("Risultato KEYcmp = %d\n", cmp);



    printf("\nTest domanda 6:\n");
    int k;
    char nome1[31], nome2[31];
    printf("\nNome1: ");
    scanf(" %64[^\n]", nome1);
    printf("\nInserire data nascita1 (dd mm aa): ");
    Date d1 = readDate();
    printf("\nNome2: ");
    scanf(" %64[^\n]", nome2);
    printf("\nInserire data nascita2 (dd mm aa): ");
    Date d2 = readDate();
    printf("\nInserire grado k: ");
    scanf("%d", &k);
    int rel = relationshipK(G, nome1, d1, nome2, d2, k);
    printf("Risultato relationshipK = %d\n", rel);



    printf("\nTest domanda 7:\n");
    printf("\n Inserire grado k: ");
    scanf("%d", &k);
    largestLineageK(G, k);

    GRAPHprint(G);
    key_free(k1);
    key_free(k2);
    date_free(d1);
    date_free(d2);
    graph_free(G);
    return 0;
}