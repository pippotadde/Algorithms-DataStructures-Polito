#include <stdio.h>
#include <string.h>

#define nomeFile1 "testo.txt"
#define nomeFile2 "stampa.txt"
#define MAXN 1000
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

int leggiFile(corsa_t corse[]);
void stampa(corsa_t c, int stampo);
void ric_codice(corsa_t c[], int n, int stampa);
void ric_part(corsa_t c[], int n, int stampa);
void insertionSort(corsa_t *c[], int n, int (*cmp)(corsa_t *, corsa_t *), int s);
int cmpDataOra(corsa_t *a, corsa_t *b);
int cmpCodice(corsa_t *a, corsa_t *b);
int cmpPartenza(corsa_t *a, corsa_t *b);
int cmpArrivo(corsa_t *a, corsa_t *b);

int main(void) {
    corsa_t *c_data[MAXN], *c_codice[MAXN], *c_part[MAXN], *c_arr[MAXN];
    int n, opzione;
    int stampa = 0;
    corsa_t corse[MAXN];

    n = leggiFile(corse);

    for (int i = 0; i < n; i++) {
        c_data[i] = &corse[i];
        c_codice[i] = &corse[i];
        c_part[i] = &corse[i];
        c_arr[i] = &corse[i];
    }

    do {
        printf("\n");
        printf("\nMENU:\n");
        printf("(Defoult stampa su video)\n");
        printf("1) Ordinamento del vettore per data, e a parità di date per ora\n");
        printf("2) Ordinamento del vettore per codice di tratta\n");
        printf("3) Ordinamento del vettore per stazione di partenza\n");
        printf("4) Ordinamento del vettore per stazione di arrivo\n");
        printf("5) Ricerca di una tratta per codice\n");
        printf("6) Ricerca di una tratta per stazione di partenza\n");
        printf("7) Impostare stampa su file\n");
        printf("8) Impostare stampa su video\n");
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
            case 7: stampa = 1; printf("Impostata stampa su file correttamente\n"); break;
            case 8: stampa = 0; printf("Impostata stampa su video correttamente\n"); break;
            case 0: printf("Uscita dal programma.\n"); break;
            default: printf("Scelta non valida!\n"); break;
        }
    } while (opzione != 0);
    return 0;
}

int leggiFile(corsa_t corse[]) {
    int n;
    FILE *fp = fopen(nomeFile1, "r");
    if (fp == NULL) {
        printf("Errore apertura file!\n");
        return 0;
    }
    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %s %s %s %s %s %d", corse[i].codice, corse[i].partenza, corse[i].destinazione,
               corse[i].data, corse[i].ora_partenza, corse[i].ora_arrivo,
               &corse[i].ritardo);
    }
    fclose(fp);
    return n;
}

void stampa(corsa_t c, int stampa) {
    if (stampa == 0) {
        printf("%s %s %s %s %s %s %d\n",
               c.codice,
               c.partenza,
               c.destinazione,
               c.data,
               c.ora_partenza,
               c.ora_arrivo,
               c.ritardo);
    } else if (stampa == 1) {
        FILE *fp = fopen(nomeFile2, "a");
        if (fp == NULL) {
            printf("Errore apertura file!\n");
        } else {
            fprintf(fp, "%s %s %s %s %s %s %d\n",
                   c.codice,
                   c.partenza,
                   c.destinazione,
                   c.data,
                   c.ora_partenza,
                   c.ora_arrivo,
                   c.ritardo);
        }
        fclose(fp);
    } else {
        printf("Errore stampa \n");
    }
}

void ric_codice(corsa_t c[], int n, int s) {
    int t = 0;
    char codice[MAXC];
    printf("\n");
    printf("Digitare codice: ");
    scanf("%s", codice);
    int low = 0, high = n - 1;
    while (low <= high && t == 0) {
        int mid = (low + high) / 2;
        int cmp = strcmp(c[mid].codice, codice);
        if (cmp == 0) {
            stampa(c[mid], s);
            t = 1;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (t == 0) {
        printf("Nessun codice trovato.\n");
    }
}

void ric_part(corsa_t c[], int n, int s) {
    int t = 0;
    char partenza[MAXC];
    printf("\n");
    printf("Digitare stazione partenza: ");
    scanf("%s", partenza);
    for (int i = 0; i < n; i++) {
        if (strncmp(c[i].partenza, partenza, strlen(partenza)) == 0) {
            stampa(c[i], s);
            t = 1;
        }
    }
    if (t == 0) {
        printf("Nessuna tratta trovata.\n");
    }
}

void insertionSort(corsa_t *c[], int n, int (*cmp)(corsa_t *, corsa_t *), int s) {
    corsa_t *key;
    int j;
    for (int i = 1; i < n; i++) {
        key = c[i];
        j = i - 1;
        while ((j >= 0) && (cmp(c[j], key) > 0)) {
            c[j+1] = c[j];
            j--;
        }
        c[j+1] = key;
    }
    for (int i = 0; i < n; i++) {
        stampa(*c[i], s);
    }
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
