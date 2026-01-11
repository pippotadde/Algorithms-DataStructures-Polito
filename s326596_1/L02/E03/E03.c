#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define nomeFile1 "testo.txt"
#define nomeFile2 "stampa.txt"
#define MAXC 30

typedef struct {
    char codice[MAXC+1];
    char partenza[MAXC+1];
    char destinazione[MAXC+1];
    char data[MAXC+1];
    char ora_partenza[MAXC+1];
    char ora_arrivo[MAXC+1];
    int ritardo;
} corsa_t;

void stampa(corsa_t c, int stampo);
void ric_codice(corsa_t c[], int n, int stampa);
void ric_part(corsa_t c[], int n, int stampa);
void insertionSort(corsa_t *c[], int n, int (*cmp)(corsa_t *, corsa_t *), int s);
int cmpDataOra(corsa_t *a, corsa_t *b);
int cmpCodice(corsa_t *a, corsa_t *b);
int cmpPartenza(corsa_t *a, corsa_t *b);
int cmpArrivo(corsa_t *a, corsa_t *b);
int leggiFile(corsa_t **corse, char nomeFile[]);
void malloc2dP(corsa_t **corse, int n);
void free2d(corsa_t *corse);
void inizializzaVettori(corsa_t *corse, int n, corsa_t ***c_data, corsa_t ***c_codice, corsa_t ***c_part, corsa_t ***c_arr);

int main(void) {
    corsa_t *corse;
    corsa_t **c_data, **c_codice, **c_part, **c_arr;
    int n, opzione;
    int stampa = 0;
    char nomeFile3[MAXC];

    n = leggiFile(&corse, nomeFile1);
    inizializzaVettori(corse, n, &c_data, &c_codice, &c_part, &c_arr);

    do {
        printf("\nMENU:\n");
        printf("(Default stampa su video)\n");
        printf("1) Ordinamento per data/ora\n");
        printf("2) Ordinamento per codice\n");
        printf("3) Ordinamento per partenza\n");
        printf("4) Ordinamento per arrivo\n");
        printf("5) Ricerca per codice\n");
        printf("6) Ricerca per partenza\n");
        printf("7) Impostare stampa su file\n");
        printf("8) Impostare stampa su video\n");
        printf("9) Impostare nuovo file di lettura\n");
        printf("0) Uscire\n");
        printf("Digitare numero opzione: ");
        scanf("%d", &opzione);
        printf("\n");

        switch(opzione) {
            case 1: insertionSort(c_data, n, cmpDataOra, stampa); break;
            case 2: insertionSort(c_codice, n, cmpCodice, stampa); break;
            case 3: insertionSort(c_part, n, cmpPartenza, stampa); break;
            case 4: insertionSort(c_arr, n, cmpArrivo, stampa); break;
            case 5: ric_codice(corse, n, stampa); break;
            case 6: ric_part(corse, n, stampa); break;
            case 7: stampa = 1; printf("Impostata stampa su file.\n"); break;
            case 8: stampa = 0; printf("Impostata stampa su video.\n"); break;
            case 9:
                free2d(corse);
                free(c_data);
                free(c_codice);
                free(c_part);
                free(c_arr);

                printf("Inserire nome file: ");
                scanf("%s", nomeFile3);
                n = leggiFile(&corse, nomeFile3);

                inizializzaVettori(corse, n, &c_data, &c_codice, &c_part, &c_arr);
                break;


            case 0:
                printf("Uscita dal programma.\n");
                break;

            default:
                printf("Scelta non valida!\n");
                break;
        }

    } while (opzione != 0);

    free2d(corse);
    free(c_data);
    free(c_codice);
    free(c_part);
    free(c_arr);

    return 0;
}

void free2d(corsa_t *corse) {
    free(corse);
}

void malloc2dP(corsa_t **corse, int n) {
    *corse = malloc(n * sizeof(corsa_t));
    if (*corse == NULL) {
        printf("Errore allocazione\n");
        exit(1);
    }
}

int leggiFile(corsa_t **corse, char nomeFile[]) {
    int n;
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) {
        printf("Errore apertura file\n");
        exit(1);
    }
    fscanf(fp, "%d", &n);
    malloc2dP(corse, n);
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %s %s %s %s %s %d",
               (*corse)[i].codice,
               (*corse)[i].partenza,
               (*corse)[i]. destinazione,
               (*corse)[i].data,
               (*corse)[i].ora_partenza,
               (*corse)[i].ora_arrivo,
               &(*corse)[i].ritardo);
    }
    fclose(fp);
    printf("File letto correttamente\n");
    return n;
}

void stampa(corsa_t c, int stampa) {
    if (stampa == 0) {
        printf("%s %s %s %s %s %s %d\n",
               c.codice, c.partenza, c.destinazione,
               c.data, c.ora_partenza, c.ora_arrivo, c.ritardo);
    } else if (stampa == 1) {
        FILE *fp = fopen(nomeFile2, "a");
        if (fp == NULL)
            printf("Errore apertura file\n");
        else {
            fprintf(fp, "%s %s %s %s %s %s %d\n",
                    c.codice, c.partenza, c.destinazione,
                    c.data, c.ora_partenza, c.ora_arrivo, c.ritardo);
            fclose(fp);
        }
    } else {
        printf("Errore stampa\n");
    }
}

void ric_codice(corsa_t c[], int n, int s) {
    int trovato = 0;
    char codice[MAXC];
    printf("Digitare codice: ");
    scanf("%s", codice);
    for (int i = 0; i < n; i++) {
        if (strcmp(c[i].codice, codice) == 0) {
            stampa(c[i], s);
            trovato = 1;
        }
    }
    if (!trovato)
        printf("Nessun codice trovato.\n");
}

void ric_part(corsa_t c[], int n, int s) {
    int trovato = 0;
    char partenza[MAXC];
    printf("Digitare stazione di partenza: ");
    scanf("%s", partenza);
    for (int i = 0; i < n; i++) {
        if (strncmp(c[i].partenza, partenza, strlen(partenza)) == 0) {
            stampa(c[i], s);
            trovato = 1;
        }
    }
    if (!trovato)
        printf("Nessuna tratta trovata.\n");
}

void insertionSort(corsa_t *c[], int n, int (*cmp)(corsa_t *, corsa_t *), int s) {
    for (int i = 1; i < n; i++) {
        corsa_t *key = c[i];
        int j = i - 1;
        while (j >= 0 && cmp(c[j], key) > 0) {
            c[j + 1] = c[j];
            j--;
        }
        c[j + 1] = key;
    }
    for (int i = 0; i < n; i++)
        stampa(*c[i], s);
}

int cmpDataOra(corsa_t *a, corsa_t *b) {
    int cmp = strcmp(a->data, b->data);
    if (cmp != 0)
        return cmp;
    return strcmp(a->ora_partenza, b->ora_partenza);
}

int cmpCodice(corsa_t *a, corsa_t *b) {
    return strcmp(a->codice, b->codice);
}

int cmpPartenza(corsa_t *a, corsa_t *b) {
    return strcmp(a->partenza, b->partenza);
}

int cmpArrivo(corsa_t *a, corsa_t *b) {
    return strcmp(a->destinazione, b->destinazione);
}

void inizializzaVettori(corsa_t *corse, int n,
                        corsa_t ***c_data,
                        corsa_t ***c_codice,
                        corsa_t ***c_part,
                        corsa_t ***c_arr) {

    *c_data   = malloc(n * sizeof(corsa_t *));
    *c_codice = malloc(n * sizeof(corsa_t *));
    *c_part   = malloc(n * sizeof(corsa_t *));
    *c_arr    = malloc(n * sizeof(corsa_t *));

    if (!(*c_data) || !(*c_codice) || !(*c_part) || !(*c_arr)) {
        printf("Errore allocazione\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        (*c_data)[i]   = &corse[i];
        (*c_codice)[i] = &corse[i];
        (*c_part)[i]   = &corse[i];
        (*c_arr)[i]    = &corse[i];
    }
}