#include "ginnastica.h"

struct elemento_s {
    char nome[MAXS];
    int tipologia;
    int ingresso;
    int uscita;
    int precedenza;
    int finale;
    float valore;
    int difficolta;
};

struct lista_s {
    elemento_t *e;
    int n;
};

struct seq_s {
    elemento_t s[MAXE];
    int n;
    int dd;
};

struct atleta_s {
    seq_t seq[Nseq];
    float punteggio;
    int tot_diff;
};

elemento_t elemento_init() {
    elemento_t e = malloc(sizeof(*e));
    if (e == NULL) { return NULL; };
    e->nome[0] = '\0';
    e->tipologia = 0;
    e->ingresso = 0;
    e->uscita = 0;
    e->precedenza = 0;
    e->finale = 0;
    e->valore = 0.0;
    e->difficolta = 0;
    return e;
}

void elemento_free(elemento_t elemento) {
    if (elemento != NULL) {
        free(elemento);
    }
}

lista_t lista_init() {
    lista_t list = malloc(sizeof(*list));
    if (list == NULL) { return NULL; }
    list->e = NULL;
    list->n = 0;
    return list;
}

void lista_free(lista_t list) {
    if (list == NULL) { return; }
    for (int i = 0; i < list->n; i++) {
        elemento_free(list->e[i]);
    }
    free(list->e);
    free(list);
}

seq_t seq_init(void) {
    seq_t seq = malloc(sizeof(struct seq_s));
    if (seq != NULL) {
        seq->dd = 0;
        seq->n = 0;
    }
    return seq;
}

void seq_free(seq_t seq) {
    if (seq != NULL) {
        free(seq);
    }
}

atleta_t atleta_init(void) {
    atleta_t atleta = malloc(sizeof(struct atleta_s));
    if (atleta != NULL) {
        for (int i = 0; i < Nseq; i++) {
            atleta->seq[i] = seq_init();
        }
        atleta->punteggio = 0.0;
        atleta->tot_diff = 0;
    }
    return atleta;
}

void atleta_free(atleta_t a) {
    if(a != NULL) {
        for(int k=0; k<Nseq; k++) {
            seq_free(a->seq[k]);
        }
        free(a);
    }
}

void leggiElemento(elemento_t e, FILE *fp) {
    if (e == NULL || fp == NULL)  { return; }
    fscanf(fp, "%99s %d %d %d %d %d %f %d",
           e->nome,
           &e->tipologia,
           &e->ingresso,
           &e->uscita,
           &e->precedenza,
           &e->finale,
           &e->valore,
           &e->difficolta);
}

int leggiFile(lista_t list, FILE *fp) {
    if (list == NULL || fp == NULL) { return 0; }

    int n;
    if (fscanf(fp, "%d", &n) != 1) { return 0; }
    list->e = malloc(n * sizeof(elemento_t));
    if (list->e == NULL) { return 0; }
    list->n = n;

    for (int i = 0; i < n; i++) {
        list->e[i] = elemento_init();
        leggiElemento(list->e[i], fp);
    }
    return n;
}

elemento_t lista_get(lista_t l, int i) {
    if (i < 0 || i >= l->n) { return NULL; }
    return l->e[i];
}

int checkcondizioni(seq_t seq) {
    elemento_t el1 = seq->s[0];
    if (el1->ingresso != 1) { return 1; }
    if (el1->precedenza != 0) return 1;

    if (seq->n >= 1) {
        for (int i=1; i<seq->n; i++) {
            elemento_t el_pr = seq->s[i-1];
            elemento_t el_dp = seq->s[i];
            if (el_pr->uscita != el_dp->ingresso) { return 1; }
            if (el_pr->finale == 1) { return 1; }
        }
    }
    return 0;
}

int seq_DD(seq_t seq, int DD) {
    if (seq->dd > DD) {
        return 1;
    }
    return 0;
}

int atleta_DP(atleta_t atleta, int DP) {
    if (atleta->tot_diff > DP) {
        return 1;
    }
    return 0;
}


int check_almeno_un_acrobatico(seq_t seq) {
    for (int i=0; i<seq->n; i++) {
        if (seq->s[i]->tipologia > 0) return 1;
    }
    return 0;
}

int check_consecutivi(atleta_t atleta) {
    for (int i=0; i<Nseq; i++) {
        seq_t seq = atleta->seq[i];
        for (int j=0; j < seq->n - 1; j++) {
            if (seq->s[j]->tipologia > 0 && seq->s[j+1]->tipologia > 0) {
                return 1; // Trovata
            }
        }
    }
    return 0; // Nessuna trovata
}

int check_avanti_indietro(atleta_t atleta) {
    int avanti = 0;
    int indietro = 0;
    for (int i=0; i<Nseq; i++) {
        seq_t seq_i = atleta->seq[i];
        for (int j=0; j<seq_i->n; j++) {
            elemento_t el = seq_i->s[j];
            if (el->tipologia == 2) { avanti = 1; }
            if (el->tipologia == 1) { indietro = 1; }
        }
    }
    if (avanti == 1 && indietro == 1) {
        return 1; // OK
    }
    return 0;
}

void calcola_punteggio_atleta(atleta_t atleta) {
    atleta->punteggio = 0.0;
    atleta->tot_diff = 0;

    for (int i=0; i<Nseq; i++) {
        float p_parz = 0.0;
        atleta->tot_diff += atleta->seq[i]->dd;

        for(int j=0; j<atleta->seq[i]->n; j++) {
            p_parz += atleta->seq[i]->s[j]->valore;
        }

        // Bonus ultima diagonale
        if (i == Nseq-1 && atleta->seq[i]->n > 0) {
            seq_t ult = atleta->seq[i];
            if (ult->s[ult->n - 1]->difficolta >= 8) {
                p_parz *= 1.5;
            }
        }
        atleta->punteggio += p_parz;
    }
}

void atleta_copia(atleta_t best, atleta_t atleta) {
    best->punteggio = atleta->punteggio;
    best->tot_diff = atleta->tot_diff;
    for (int i=0; i<Nseq; i++) {
        best->seq[i]->n = atleta->seq[i]->n;
        best->seq[i]->dd = atleta->seq[i]->dd;
        for (int j=0; j<atleta->seq[i]->n; j++) {
            best->seq[i]->s[j] = atleta->seq[i]->s[j];
        }
    }
}

int crea_esercizio(atleta_t atleta, lista_t lista, int DD, int DP) {
    for(int i=0; i<Nseq; i++) {
        atleta->seq[i]->n = 0;
        atleta->seq[i]->dd = 0;
    }
    atleta->punteggio = 0.0;
    atleta->tot_diff = 0;

    atleta_t best_atleta = atleta_init();
    best_atleta->punteggio = -1.0;

    solve_ricorsiva(atleta, best_atleta, lista, DD, DP, 0);

    if (best_atleta->punteggio > 0) {
        atleta_copia(atleta, best_atleta);
        atleta_free(best_atleta);
        return 1;
    }

    atleta_free(best_atleta);
    return 0;
}

void solve_ricorsiva(atleta_t atleta, atleta_t best_atleta, lista_t lista, int DD, int DP, int k_seq) {
    // prova soluzione
    if (k_seq == Nseq) {
        calcola_punteggio_atleta(atleta);
        // Controlli finali
        if (atleta_DP(atleta, DP) != 0) { return; }
        if (check_consecutivi(atleta) == 0) { return; }
        if (check_avanti_indietro(atleta) == 0) { return; }

        // punteggio migliore
        if (atleta->punteggio > best_atleta->punteggio) {
            atleta_copia(best_atleta, atleta);
        }
        return;
    }

    seq_t seq_corr = atleta->seq[k_seq];

    // Tentativo chiusura
    if (seq_corr->n >= 1 && check_almeno_un_acrobatico(seq_corr)) {
        solve_ricorsiva(atleta, best_atleta, lista, DD, DP, k_seq + 1);
    }

    // Se sequenza piena
    if (seq_corr->n == MAXE) {
        return;
    }

    // Aggiungo elementi
    for (int i = 0; i < lista->n; i++) {
        elemento_t el = lista_get(lista, i);

        // Difficoltà Diagonale
        if (seq_corr->dd + el->difficolta > DD) continue;

        // Aggiunta elemento
        seq_corr->s[seq_corr->n] = el;
        seq_corr->n++;
        seq_corr->dd += el->difficolta;

        if (checkcondizioni(seq_corr) == 0) {
            solve_ricorsiva(atleta, best_atleta, lista, DD, DP, k_seq);
        }

        // Backtrack
        seq_corr->dd -= el->difficolta;
        seq_corr->n--;
    }
}

void stampa(atleta_t atleta) {
    printf("TOT PUNTEGGIO = %.3f\n", atleta->punteggio);
    printf("TOT DIFFICOLTA = %d\n", atleta->tot_diff);

    for (int i = 0; i < Nseq; i++) {
        seq_t s = atleta->seq[i];
        printf("DIAG #%d > DD: %d", i+1, s->dd);

        if (i == Nseq-1 && s->n > 0) {
            if (s->s[s->n-1]->difficolta >= 8) {
                printf(" * BONUS 1.5 APPLICATO");
            }
        }
        printf("\n");

        for (int j = 0; j < s->n; j++) {
            printf("  %s (val:%.1f diff:%d)\n", s->s[j]->nome, s->s[j]->valore, s->s[j]->difficolta);
        }
    }
}