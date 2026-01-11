#include <stdio.h>
#include <string.h>

#define nomeFile "testo.txt"
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

int  leggiFile(corsa_t corse[]);
void menu(corsa_t corse[], int n);
void f_date(corsa_t corse[], int n);
void f_partenze(corsa_t corse[], int n);
void f_arrivi(corsa_t corse[], int n);
void f_intervallo(corsa_t corse[], int n);
void f_ritardi(corsa_t corse[], int n);
void stampaCorsa(corsa_t c);

int main(void) {
    corsa_t corse[MAXN];
    int n = leggiFile(corse);
    menu(corse, n);
    return 0;
}

int leggiFile(corsa_t corse[]) {
    int n;
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) {
        printf("Errore apertura file!\n");
        return 0;
    }

    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%s %s %s %s %s %s %d",
               corse[i].codice, corse[i].partenza, corse[i].destinazione,
               corse[i].data, corse[i].ora_partenza, corse[i].ora_arrivo,
               &corse[i].ritardo);
    }
    fclose(fp);
    return n;
}

void stampaCorsa(corsa_t c) {
    printf("%s %s %s %s %s %s %d\n",
           c.codice,
           c.partenza,
           c.destinazione,
           c.data,
           c.ora_partenza,
           c.ora_arrivo,
           c.ritardo);
}

void menu(corsa_t corse[], int n) {
    int opzione;
    do {
        printf("\nMENU:\n");
        printf("1) Elencare tutte le corse partite in un certo intervallo di date\n");
        printf("2) Elencare tutte le corse partite da una certa fermata\n");
        printf("3) Elencare tutte le corse che fanno capolinea in una certa fermata\n");
        printf("4) Elencare tutte le corse che hanno raggiunto la destinazione in ritardo in un certo intervallo di date\n");
        printf("5) Elencare il ritardo complessivo accumulato dalle corse identificate da un certo codice di tratta\n");
        printf("0) Uscire\n");
        printf("Digitare numero opzione: ");
        scanf("%d", &opzione);

        switch(opzione) {
            case 1: f_date(corse, n); break;
            case 2: f_partenze(corse, n); break;
            case 3: f_arrivi(corse, n); break;
            case 4: f_intervallo(corse, n); break;
            case 5: f_ritardi(corse, n); break;
            case 0: printf("Uscita dal programma.\n"); break;
            default: printf("Scelta non valida!\n"); break;
        }
    } while (opzione != 0);
}

void f_date(corsa_t corse[], int n) {
    char data_inizio[MAXC], data_fine[MAXC];
    printf("Inserisci data inizio (AAAA/MM/GG): ");
    scanf("%s", data_inizio);
    printf("Inserisci data fine (AAAA/MM/GG): ");
    scanf("%s", data_fine);

    printf("\nCorse tra %s e %s:\n", data_inizio, data_fine);
    int trovate = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(corse[i].data, data_inizio) >= 0 &&
            strcmp(corse[i].data, data_fine) <= 0) {
            stampaCorsa(corse[i]);
            trovate++;
        }
    }

    if (trovate == 0)
        printf("Nessuna corsa trovata in quell'intervallo.\n");
}

void f_partenze(corsa_t corse[], int n) {
    char partenza[MAXC];
    printf("Inserisci stazione di partenza: ");
    scanf("%s", partenza);

    printf("\nCorse partite da %s:\n", partenza);
    int trovate = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(corse[i].partenza, partenza) == 0) {
            stampaCorsa(corse[i]);
            trovate++;
        }
    }
    if (trovate == 0)
        printf("Nessuna corsa trovata da quella fermata.\n");
}

void f_arrivi(corsa_t corse[], int n) {
    char destinazione[MAXC];
    printf("Inserisci stazione di arrivo: ");
    scanf("%s", destinazione);

    printf("\nCorse che arrivano a %s:\n", destinazione);
    int trovate = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(corse[i].destinazione, destinazione) == 0) {
            stampaCorsa(corse[i]);
            trovate++;
        }
    }
    if (trovate == 0)
        printf("Nessuna corsa trovata con quella destinazione.\n");
}

void f_intervallo(corsa_t corse[], int n) {
    char data_inizio[MAXC], data_fine[MAXC];
    printf("Inserisci data inizio (AAAA/MM/GG): ");
    scanf("%s", data_inizio);
    printf("Inserisci data fine (AAAA/MM/GG): ");
    scanf("%s", data_fine);

    printf("\nCorse con ritardo tra %s e %s:\n", data_inizio, data_fine);
    int trovate = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(corse[i].data, data_inizio) >= 0 &&
            strcmp(corse[i].data, data_fine) <= 0 &&
            corse[i].ritardo > 0) {
            stampaCorsa(corse[i]);
            trovate++;
        }
    }
    if (trovate == 0)
        printf("Nessuna corsa in ritardo in quell'intervallo.\n");
}

void f_ritardi(corsa_t corse[], int n) {
    char codice[MAXC];
    printf("Inserisci codice tratta: ");
    scanf("%s", codice);

    int somma = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(corse[i].codice, codice) == 0)
            somma += corse[i].ritardo;
    }
    printf("Ritardo totale per tratta %s = %d minuti\n", codice, somma);
}
