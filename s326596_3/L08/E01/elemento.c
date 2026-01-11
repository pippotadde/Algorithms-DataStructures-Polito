#include "elemento.h"

elemento_t elemento_init(void) {
    elemento_t e = malloc(sizeof(*e));
    if (e == NULL) { return NULL; };
    e->nome[0] = '\0';
    e->tipologia = 0;
    e->dir_ingresso = 0;
    e->dir_uscita = 0;
    e->precedenza = 0;
    e->finale = 0;
    e->valore = 0.0;
    e->difficolta = 0;
    return e;
}

void elemento_free(elemento_t e) {
    if (e != NULL) {
        free(e);
    }
}

int leggiElemento(elemento_t e, FILE *fp) {
    if (e == NULL || fp == NULL)  { return 0; }
    if (fscanf(fp, "%99s %d %d %d %d %d %f %d",
               e->nome,
               &e->tipologia,
               &e->dir_ingresso,
               &e->dir_uscita,
               &e->precedenza,
               &e->finale,
               &e->valore,
               &e->difficolta) != 8) {
        return 0;
    }
    return 1;
}