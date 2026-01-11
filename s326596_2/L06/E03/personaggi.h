#ifndef PERSONAGGI_H
#define PERSONAGGI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"

#define EQUIP_SLOT_MAX 8

typedef struct {
    int inUso;
    int vettEq[EQUIP_SLOT_MAX];
} equip_t;

typedef struct {
    char codice[MAXC+1];
    char nome[MAXC+1];
    char classe[MAXC+1];
    stat_t stat;
    equip_t equip;
} pg_t;

typedef struct nodoPg_s *link;

typedef struct nodoPg_s {
    pg_t pg;
    link next;
} nodoPg_t;

typedef struct {
    link headPg;
    link tailPg;
    int  nPg;
} tabPg_t;

void pgList_init(tabPg_t *tabPg);
void pgList_read(char nomeFile[], tabPg_t *tabPg);
void pgList_insert(tabPg_t *tabPg, pg_t pg);
void pgList_remove(tabPg_t *tabPg, char codice[]);
pg_t *pgList_searchByCode(tabPg_t *tabPg, char codice[]);
void pgList_free(tabPg_t *tabPg);

void pg_print(pg_t *pg, tabInv_t *tabInv, FILE *fp, int stampaEquip);
void pg_printAll(tabPg_t *tabPg, tabInv_t *tabInv, FILE *fp);
int  pgEquip_add(tabPg_t *tabPg, tabInv_t *tabInv, char codicePg[], char nomeOggetto[]);
int  pgEquip_remove(tabPg_t *tabPg, tabInv_t *tabInv, char codicePg[], char nomeOggetto[]);
void pg_statsCompute(pg_t *pg, tabInv_t *tabInv, stat_t *res);

#endif
