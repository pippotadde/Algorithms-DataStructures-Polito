#include "esercizio.h"
#include <float.h>

struct lista_s {
    elemento_t *e;
    int n;
};

struct seq_s {
    elemento_t s[MAXE];
    int n;
    int diff_curr;
    int ha_acro;
    int dir_uscita_attuale;
};

struct atleta_s {
    seq_t seq[Nseq];
    int diff_totale;
    int has_avanti;
    int has_indietro;
    int has_seq;
    int budget_avanti;
    int budget_indietro;
    int DP_effettivo;
    int limit_DD;
    int limit_DP;
};

lista_t lista_init(void) {
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
    seq_t seq = malloc(sizeof(*seq));
    if (seq != NULL) {
        seq->n = 0;
        seq->diff_curr = 0;
        seq->ha_acro = 0;
        seq->dir_uscita_attuale = 1;
    }
    return seq;
}

void seq_free(seq_t seq) {
    if (seq != NULL) free(seq);
}

atleta_t atleta_init(void) {
    atleta_t atleta = malloc(sizeof(*atleta));
    if (atleta != NULL) {
        for (int i = 0; i < Nseq; i++) {
            atleta->seq[i] = seq_init();
        }
        atleta->diff_totale = 0;
        atleta->has_avanti = 0;
        atleta->has_indietro = 0;
        atleta->has_seq = 0;
        atleta->budget_avanti = 0;
        atleta->budget_indietro = 0;
        atleta->DP_effettivo = 0;
        atleta->limit_DD = 0;
        atleta->limit_DP = 0;
    }
    return atleta;
}

void atleta_free(atleta_t a) {
    if (a != NULL) {
        for (int i = 0; i < Nseq; i++) {
            seq_free(a->seq[i]);
        }
        free(a);
    }
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
        if (leggiElemento(list->e[i], fp) == 0) { return 0; }
    }
    return n;
}

static void trova_minimi(lista_t lista, int *min_a, int *min_i) {
    *min_a = 9999;
    *min_i = 9999;
    for (int i = 0; i < lista->n; i++) {
        elemento_t e = lista->e[i];
        if (e->tipologia == 2 && e->difficolta < *min_a) { *min_a = e->difficolta; }
        if (e->tipologia == 1 && e->difficolta < *min_i) { *min_i = e->difficolta; }
    }
    if (*min_a == 9999) { *min_a = 0; }
    if (*min_i == 9999) { *min_a = 0; }
}

static float calcola_appetibilita(elemento_t e, atleta_t sol, int k) {
    float bonus = 0.0;
    float val;
    if (e->difficolta > 0) {
        val = e->valore / (float)e->difficolta;
    } else {
        val = e->valore * 10.0;
    }

    if (sol->has_avanti == 0 && e->tipologia == 2) { bonus += 100.0; }
    if (sol->has_indietro == 0 && e->tipologia == 1) { bonus += 100.0; }
    if (sol->seq[k]->ha_acro == 0 && e->tipologia != 0) { bonus += 50.0; }
    if (sol->has_seq == 0 && sol->seq[k]->n > 0) {
        elemento_t prev = sol->seq[k]->s[sol->seq[k]->n - 1];
        if (prev->tipologia != 0 && e->tipologia != 0) { bonus += 80.0; }
    }
    if (k == 2 && e->difficolta >= 8) {
        bonus += 30.0;
        if (e->finale) {
            bonus += 20.0;
        }
    }
    if (sol->has_indietro == 0 && e->dir_uscita == 0) { bonus += 10.0; }
    if (sol->has_avanti == 0 && e->dir_uscita == 1 && sol->seq[k]->dir_uscita_attuale == 0) { bonus += 10.0; }
    if (e->finale && sol->seq[k]->n < 2) { bonus -= 50.0; }

    return val + bonus;
}

static int check_ammissibile(elemento_t e, atleta_t sol, int k) {
    seq_t d = sol->seq[k];
    if (e->dir_ingresso != d->dir_uscita_attuale) { return 0; }
    if (d->n == 0 && e->precedenza) { return 0; }
    if (d->diff_curr + e->difficolta > sol->limit_DD) { return 0; }

    int budget = sol->DP_effettivo - sol->diff_totale;
    if (sol->has_avanti == 0 && e->tipologia == 2) budget += sol->budget_avanti;
    if (sol->has_indietro == 0 && e->tipologia == 1) budget += sol->budget_indietro;

    if (e->difficolta > budget) { return 0; }
    return 1;
}

void crea_esercizio(atleta_t atleta, lista_t lista, int DD, int DP) {
    if (atleta == 0 || lista == 0) { return; }
    atleta->limit_DD = DD;
    atleta->limit_DP = DP;

    int ma, mi;
    trova_minimi(lista, &ma, &mi);
    atleta->budget_avanti = ma;
    atleta->budget_indietro = mi;
    atleta->DP_effettivo = DP - ma - mi;

    int progress = 1;
    while(progress) {
        progress = 0;
        for (int k = 0; k < Nseq; k++) {
            seq_t d = atleta->seq[k];
            if (d->n >= MAXE) { continue; }
            if (d->n > 0 && d->s[d->n - 1]->finale) { continue; }

            int b_idx = -1;
            float max_app = -FLT_MAX;

            for (int i = 0; i < lista->n; i++) {
                elemento_t cur = lista->e[i];
                if (check_ammissibile(cur, atleta, k) == 0) { continue; }
                float app = calcola_appetibilita(cur, atleta, k);
                if (app > max_app) {
                    max_app = app;
                    b_idx = i;
                }
            }

            if (b_idx != -1) {
                elemento_t s = lista->e[b_idx];
                d->s[d->n++] = s;
                d->diff_curr += s->difficolta;
                d->dir_uscita_attuale = s->dir_uscita;
                atleta->diff_totale += s->difficolta;

                if (s->tipologia != 0) d->ha_acro = 1;

                if (s->tipologia == 2 && !atleta->has_avanti) {
                    atleta->has_avanti = 1;
                    atleta->DP_effettivo += atleta->budget_avanti;
                }
                if (s->tipologia == 1 && !atleta->has_indietro) {
                    atleta->has_indietro = 1;
                    atleta->DP_effettivo += atleta->budget_indietro;
                }

                if (d->n >= 2) {
                    elemento_t prev = d->s[d->n - 2];
                    if (prev->tipologia != 0 && s->tipologia != 0) atleta->has_seq = 1;
                }
                progress = 1;
            }
        }
    }
}

void stampa(atleta_t atleta) {
    printf("\n");
    if (atleta == 0) { return; }

    float grand_total = 0.0;

    for (int i = 0; i < Nseq; i++) {
        float diag_val = 0.0;
        int diff_last = 0;
        if (atleta->seq[i]->n > 0) {
            diff_last = atleta->seq[i]->s[atleta->seq[i]->n - 1]->difficolta;
        }
        for (int j = 0; j < atleta->seq[i]->n; j++) {
            diag_val += atleta->seq[i]->s[j]->valore;
        }
        if (i == 2 && diff_last >= 8) {
            diag_val *= 1.5;
        }
        grand_total += diag_val;
    }

    printf("DD = %d DP = %d\n", atleta->limit_DD, atleta->limit_DP);
    printf("TOT = %.3f\n", grand_total);

    for (int i = 0; i < Nseq; i++) {
        float base_val = 0.0;
        int diff_last = 0;
        if (atleta->seq[i]->n > 0) {
            diff_last = atleta->seq[i]->s[atleta->seq[i]->n - 1]->difficolta;
        }
        for (int j = 0; j < atleta->seq[i]->n; j++) {
            base_val += atleta->seq[i]->s[j]->valore;
        }

        printf("DIAG #%d > %.3f", i + 1, base_val);
        if (i == 2 && diff_last >= 8) {
            printf(" * 1.5 (BONUS)");
        }
        printf("\n");

        for (int j = 0; j < atleta->seq[i]->n; j++) {
            printf("%s", atleta->seq[i]->s[j]->nome);
            if (j < atleta->seq[i]->n - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}