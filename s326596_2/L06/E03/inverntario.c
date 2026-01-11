#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"

void inv_read(char nomeFile[], tabInv_t *tabInv) {
    FILE *fp = fopen(nomeFile, "r");
    int n, i;

    if (fp == NULL) {
        printf("Errore apertura file inventario '%s'\n", nomeFile);
        tabInv->vettInv = NULL;
        tabInv->nInv = tabInv->maxInv = 0;
        return;
    }

    if (fscanf(fp, "%d", &n) != 1 || n <= 0) {
        printf("Formato file inventario non valido\n");
        fclose(fp);
        tabInv->vettInv = NULL;
        tabInv->nInv = tabInv->maxInv = 0;
        return;
    }

    tabInv->vettInv = (inv_t *)malloc(n * sizeof(inv_t));
    if (tabInv->vettInv == NULL) {
        printf("Errore malloc inventario\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    tabInv->nInv = n;
    tabInv->maxInv = n;

    for (i = 0; i < n; i++) {
        inv_t *item = &tabInv->vettInv[i];
        if (fscanf(fp, "%s %s %d %d %d %d %d %d",
                   item->nome, item->tipo,
                   &item->stat.hp, &item->stat.mp, &item->stat.atk,
                   &item->stat.def, &item->stat.mag, &item->stat.spr) != 8) {
            printf("Errore lettura oggetto %d\n", i);
            tabInv->nInv = i;
            break;
        }
    }

    fclose(fp);
}

void inv_free(tabInv_t *tabInv) {
    if (tabInv->vettInv != NULL) {
        free(tabInv->vettInv);
        tabInv->vettInv = NULL;
    }
    tabInv->nInv = tabInv->maxInv = 0;
}

void inv_printItem(inv_t *item, FILE *fp) {
    if (item == NULL)
        return;
    fprintf(fp, "%s %s ", item->nome, item->tipo);
    fprintf(fp, "HP %d MP %d ATK %d DEF %d MAG %d SPR %d\n",
            item->stat.hp, item->stat.mp, item->stat.atk,
            item->stat.def, item->stat.mag, item->stat.spr);
}

void inv_print(tabInv_t *tabInv, FILE *fp) {
    int i;
    if (tabInv->vettInv == NULL) {
        fprintf(fp, "Inventario vuoto\n");
        return;
    }
    fprintf(fp, "Inventario (%d oggetti):\n", tabInv->nInv);
    for (i = 0; i < tabInv->nInv; i++) {
        fprintf(fp, "%2d) ", i);
        inv_printItem(&tabInv->vettInv[i], fp);
    }
}

int inv_searchByName(tabInv_t *tabInv, char nome[]) {
    int i;
    if (tabInv->vettInv == NULL)
        return -1;
    for (i = 0; i < tabInv->nInv; i++) {
        if (strcmp(tabInv->vettInv[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

inv_t *inv_getByIndex(tabInv_t *tabInv, int index) {
    if (tabInv->vettInv == NULL || index < 0 || index >= tabInv->nInv)
        return NULL;
    return &tabInv->vettInv[index];
}
