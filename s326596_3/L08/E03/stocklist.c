#include "stocklist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct snode_s {
    Stock stock;
    struct snode_s *next;
} SNode;

struct stocklist_s {
    SNode *head;
    int n;
};

static SNode *nodo_nuovo(const char *code) {
    SNode *nodo;

    nodo = (SNode *)malloc(sizeof(*nodo));
    if (nodo == NULL) { exit(1); }

    strncpy(nodo->stock.code, code, STOCK_CODE_MAX);
    nodo->stock.code[STOCK_CODE_MAX] = '\0';

    nodo->stock.quotes = albq_init();
    nodo->next = NULL;

    return nodo;
}

StockList lista_init(void) {
    StockList lista;

    lista = (StockList)malloc(sizeof(*lista));
    if (lista == NULL) { exit(1); }

    lista->head = NULL;
    lista->n = 0;

    return lista;
}

void lista_free(StockList sl) {
    SNode *cur;

    if (sl == NULL) { return; }

    cur = sl->head;
    while (cur != NULL) {
        SNode *prox;

        prox = cur->next;

        albq_free(cur->stock.quotes);
        free(cur);

        cur = prox;
    }

    free(sl);
}

int lista_size(StockList sl) {
    if (sl == NULL) { return 0; }
    return sl->n;
}

Stock *lista_cerca(StockList sl, const char *code) {
    SNode *cur;

    if (sl == NULL) { return NULL; }
    if (code == NULL) { return NULL; }

    cur = sl->head;
    while (cur != NULL) {
        int cmp;

        cmp = strcmp(cur->stock.code, code);

        if (cmp == 0) { return &cur->stock; }
        if (cmp > 0) { return NULL; }

        cur = cur->next;
    }

    return NULL;
}

Stock *lista_prendi(StockList sl, const char *code) {
    SNode *prev;
    SNode *cur;

    if (sl == NULL) { return NULL; }
    if (code == NULL) { return NULL; }

    prev = NULL;
    cur = sl->head;

    while (cur != NULL) {
        int cmp;

        cmp = strcmp(cur->stock.code, code);

        if (cmp < 0) {
            prev = cur;
            cur = cur->next;
        } else {
            break;
        }
    }

    if (cur != NULL) {
        int cmp;

        cmp = strcmp(cur->stock.code, code);
        if (cmp == 0) { return &cur->stock; }
    }

    {
        SNode *nodo;

        nodo = nodo_nuovo(code);

        if (prev == NULL) {
            nodo->next = sl->head;
            sl->head = nodo;
        } else {
            nodo->next = prev->next;
            prev->next = nodo;
        }

        sl->n = sl->n + 1;
        return &nodo->stock;
    }
}

void lista_stampa(StockList sl, FILE *fp) {
    SNode *cur;

    if (fp == NULL) { fp = stdout; }

    if (sl == NULL) {
        fprintf(fp, "(nessun titolo caricato)\n");
        return;
    }
    if (sl->head == NULL) {
        fprintf(fp, "(nessun titolo caricato)\n");
        return;
    }

    fprintf(fp, "Titoli (%d):\n", sl->n);

    cur = sl->head;
    while (cur != NULL) {
        fprintf(fp,
                "- %s (giorni=%d, profMin=%d, profMax=%d)\n",
                cur->stock.code,
                albq_size(cur->stock.quotes),
                albq_profmin(cur->stock.quotes),
                albq_profmax(cur->stock.quotes));

        cur = cur->next;
    }
}
