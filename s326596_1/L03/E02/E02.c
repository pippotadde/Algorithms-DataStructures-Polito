#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC 50

typedef struct persona *link;

struct persona {
    char codice[MAXC];
    char nome[MAXC];
    char cognome[MAXC];
    char data_di_nascita[11];
    char via[MAXC];
    char citta[MAXC];
    int cap;
    link next;
};

static link head = NULL;

void leggiFile(const char *nomeFile);
void inserisciDaTastiera(void);
link inserzione(link h, char codice[MAXC], char nome[MAXC], char cognome[MAXC], char data_di_nascita[11], char via[MAXC], char citta[MAXC], int cap);
link aggiungi(link prev, char codice[MAXC], char nome[MAXC], char cognome[MAXC], char data_di_nascita[11], char via[MAXC], char citta[MAXC], int cap);
int confrontaDate(const char d1[11], const char d2[11]);
link ricercaPerCodice(link h, const char *codice, link *prev_out);
link cancellaPerCodice(const char *codice);
link estraiPrimoInIntervallo(const char *dmin, const char *dmax);
void stampaLista(link h);
void stampaListaSuFile(const char *nomeFile, link h);
void liberaNodo(link x);

int main(void) {
    int op;
    leggiFile("anag1.txt");
    do {
        printf("\nMENU\n");
        printf("1) Inserisci da tastiera\n");
        printf("2) Inserisci da file\n");
        printf("3) Ricerca per codice\n");
        printf("4) Cancella per codice\n");
        printf("5) Cancella per intervallo date\n");
        printf("6) Stampa lista\n");
        printf("7) Stampa su file\n");
        printf("0) Esci\n ");
        printf("Inserire numero: ");
        if (scanf("%d", &op) != 1) return 0;

        if (op == 1) {
            inserisciDaTastiera();
        } else if (op == 2) {
            char nf[128];
            printf("Nome file: ");
            scanf("%s", nf);
            leggiFile(nf);
        } else if (op == 3) {
            char cod[MAXC];
            printf("Codice: ");
            scanf("%s", cod);
            link prev = NULL;
            link x = ricercaPerCodice(head, cod, &prev);
            if (x) printf("%s %s %s %s %s %s %d\n", x->codice, x->nome, x->cognome, x->data_di_nascita, x->via, x->citta, x->cap);
            else printf("Elemento non trovato\n");
        } else if (op == 4) {
            char cod[MAXC];
            printf("Codice: ");
            scanf("%s", cod);
            link x = cancellaPerCodice(cod);
            if (x) { printf("%s %s %s %s %s %s %d\n", x->codice, x->nome, x->cognome, x->data_di_nascita, x->via, x->citta, x->cap); liberaNodo(x); }
            else printf("Elemento non trovato\n");
        } else if (op == 5) {
            char d1[11], d2[11];
            printf("d1 gg/mm/aaaa: "); scanf("%s", d1);
            printf("d2 gg/mm/aaaa: "); scanf("%s", d2);
            for (link x = estraiPrimoInIntervallo(d1, d2); x != NULL; x = estraiPrimoInIntervallo(d1, d2)) {
                printf("%s %s %s %s %s %s %d\n", x->codice, x->nome, x->cognome, x->data_di_nascita, x->via, x->citta, x->cap);
                liberaNodo(x);
            }
        } else if (op == 6) {
            stampaLista(head);
        } else if (op == 7) {
            char nf[128];
            printf("Nome file: ");
            scanf("%s", nf);
            stampaListaSuFile(nf, head);
        }
    } while (op != 0);
    return 0;
}

void leggiFile(const char *nomeFile) {
    char codice[MAXC], nome[MAXC], cognome[MAXC], data_di_nascita[11], via[MAXC], citta[MAXC];
    int cap;
    FILE *fp = fopen(nomeFile, "r");
    if (!fp) {
        perror("Errore apertura file"); exit(1);
    }
    while (fscanf(fp, "%s %s %s %s %s %s %d", codice, nome, cognome, data_di_nascita, via, citta, &cap) == 7) {
        head = inserzione(head, codice, nome, cognome, data_di_nascita, via, citta, cap);
    }
    fclose(fp);
}

void inserisciDaTastiera(void) {
    char codice[MAXC], nome[MAXC], cognome[MAXC], data_di_nascita[11], via[MAXC], citta[MAXC];
    int cap;
    printf("codice nome cognome data(gg/mm/aaaa) via citta cap\n> ");
    if (scanf("%s %s %s %s %s %s %d", codice, nome, cognome, data_di_nascita, via, citta, &cap) != 7) return;
    head = inserzione(head, codice, nome, cognome, data_di_nascita, via, citta, cap);
}

link inserzione(link h, char codice[MAXC], char nome[MAXC], char cognome[MAXC], char data_di_nascita[11], char via[MAXC], char citta[MAXC], int cap) {
    if (h == NULL || confrontaDate(data_di_nascita, h->data_di_nascita) < 0) {
        link n = aggiungi(NULL, codice, nome, cognome, data_di_nascita, via, citta, cap);
        n->next = h;
        return n;
    }
    for (link x = h; x != NULL; x = x->next) {
        if (x->next == NULL || confrontaDate(data_di_nascita, x->next->data_di_nascita) < 0) {
            aggiungi(x, codice, nome, cognome, data_di_nascita, via, citta, cap);
            return h;
        }
    }
    return h;
}

link aggiungi(link prev, char codice[MAXC], char nome[MAXC], char cognome[MAXC], char data_di_nascita[11], char via[MAXC], char citta[MAXC], int cap) {
    link n = (link)malloc(sizeof *n);
    if (!n) {
        perror("malloc"); exit(1);
    }
    strcpy(n->codice, codice);
    strcpy(n->nome, nome);
    strcpy(n->cognome, cognome);
    strcpy(n->data_di_nascita, data_di_nascita);
    strcpy(n->via, via);
    strcpy(n->citta, citta);
    n->cap = cap;
    if (prev == NULL) { n->next = NULL; return n; }
    n->next = prev->next;
    prev->next = n;
    return n;
}

int confrontaDate(const char d1[11], const char d2[11]) {
    int g1, m1, a1, g2, m2, a2;
    sscanf(d1, "%d/%d/%d", &g1, &m1, &a1);
    sscanf(d2, "%d/%d/%d", &g2, &m2, &a2);
    if (a1 != a2) return a1 - a2;
    if (m1 != m2) return m1 - m2;
    return g1 - g2;
}

link ricercaPerCodice(link h, const char *codice, link *prev_out) {
    link prev = NULL;
    for (link x = h; x != NULL; prev = x, x = x->next) {
        if (strcmp(x->codice, codice) == 0) {
            if (prev_out) *prev_out = prev;
            return x;
        }
    }
    if (prev_out) *prev_out = NULL;
    return NULL;
}

link cancellaPerCodice(const char *codice) {
    link prev = NULL;
    link x = ricercaPerCodice(head, codice, &prev);
    if (!x) return NULL;
    if (prev == NULL) head = x->next; else prev->next = x->next;
    x->next = NULL;
    return x;
}

link estraiPrimoInIntervallo(const char *dmin, const char *dmax) {
    link prev = NULL;
    for (link x = head; x != NULL; prev = x, x = x->next) {
        if (confrontaDate(x->data_di_nascita, dmin) >= 0 && confrontaDate(x->data_di_nascita, dmax) <= 0) {
            if (prev == NULL) head = x->next; else prev->next = x->next;
            x->next = NULL;
            return x;
        }
    }
    return NULL;
}

void stampaLista(link h) {
    for (link x = h; x != NULL; x = x->next) {
        printf("%s %s %s %s %s %s %d\n", x->codice, x->nome, x->cognome, x->data_di_nascita, x->via, x->citta, x->cap);
    }
}

void stampaListaSuFile(const char *nomeFile, link h) {
    FILE *fp = fopen(nomeFile, "w");
    if (!fp) { perror("fopen"); return; }
    for (link x = h; x != NULL; x = x->next) {
        fprintf(fp, "%s %s %s %s %s %s %d\n", x->codice, x->nome, x->cognome, x->data_di_nascita, x->via, x->citta, x->cap);
    }

    fclose(fp);
}

void liberaNodo(link x) {
    free(x);
}
