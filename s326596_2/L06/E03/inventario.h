#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <stdio.h>

#define MAXC 50

typedef struct {
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
} stat_t;

typedef struct {
    char nome[MAXC+1];
    char tipo[MAXC+1];
    stat_t stat;
} inv_t;

typedef struct {
    inv_t *vettInv;
    int nInv;
    int maxInv;
} tabInv_t;

void inv_read(char nomeFile[], tabInv_t *tabInv);
void inv_free(tabInv_t *tabInv);
void inv_print(tabInv_t *tabInv, FILE *fp);
int  inv_searchByName(tabInv_t *tabInv, char nome[]);
inv_t *inv_getByIndex(tabInv_t *tabInv, int index);
void inv_printItem(inv_t *item, FILE *fp);

#endif
