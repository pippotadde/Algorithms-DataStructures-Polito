#include "pgList.h"

typedef struct nodePg_s {
    pg_t pg;
    struct nodePg_s *next;
} nodePg_t;

struct pgList_s {
    nodePg_t *head;
    nodePg_t *tail;
    int nPg;
};

static nodePg_t *newNode(pg_t pg, nodePg_t *next) {
    nodePg_t *x = (nodePg_t *)malloc(sizeof(*x));
    if (x == NULL) {
        fprintf(stderr, "Errore malloc pgList node\n");
        exit(EXIT_FAILURE);
    }
    x->pg = pg; /* copia per valore; equipArray rimane posseduto dal pg */
    x->next = next;
    return x;
}

pgList_t pgList_init() {
    pgList_t l = (pgList_t)malloc(sizeof(*l));
    if (l == NULL) {
        fprintf(stderr, "Errore malloc pgList\n");
        exit(EXIT_FAILURE);
    }
    l->head = l->tail = NULL;
    l->nPg = 0;
    return l;
}

void pgList_insert(pgList_t pgList, pg_t pg) {
    nodePg_t *x;
    if (pgList == NULL) return;
    x = newNode(pg, NULL);
    if (pgList->head == NULL) {
        pgList->head = pgList->tail = x;
    } else {
        pgList->tail->next = x;
        pgList->tail = x;
    }
    pgList->nPg++;
}

void pgList_read(FILE *fp, pgList_t pgList) {
    pg_t pg;
    if (fp == NULL || pgList == NULL) return;
    while (pg_read(fp, &pg) != 0) {
        pgList_insert(pgList, pg);
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray) {
    nodePg_t *x;
    if (fp == NULL || pgList == NULL) return;
    fprintf(fp, "Lista personaggi (%d):\n", pgList->nPg);
    for (x = pgList->head; x != NULL; x = x->next) {
        pg_print(fp, &x->pg, invArray);
        fprintf(fp, "\n");
    }
}

pg_t *pgList_searchByCode(pgList_t pgList, char *cod) {
    nodePg_t *x;
    if (pgList == NULL || cod == NULL) return NULL;
    for (x = pgList->head; x != NULL; x = x->next) {
        if (strcmp(x->pg.cod, cod) == 0) return &x->pg;
    }
    return NULL;
}

void pgList_remove(pgList_t pgList, char *cod) {
    nodePg_t *x, *p;
    if (pgList == NULL || cod == NULL) return;
    x = pgList->head;
    p = NULL;
    while (x != NULL && strcmp(x->pg.cod, cod) != 0) {
        p = x;
        x = x->next;
    }
    if (x == NULL) {
        printf("Personaggio %s non trovato\n", cod);
        return;
    }

    if (p == NULL) {
        pgList->head = x->next;
    } else {
        p->next = x->next;
    }
    if (pgList->tail == x) pgList->tail = p;

    pg_clean(&x->pg);
    free(x);
    pgList->nPg--;
}

void pgList_free(pgList_t pgList) {
    nodePg_t *x, *tmp;
    if (pgList == NULL) return;

    x = pgList->head;
    while (x != NULL) {
        tmp = x;
        x = x->next;
        pg_clean(&tmp->pg);
        free(tmp);
    }
    free(pgList);
}