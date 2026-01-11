#include "personaggi.h"

static link newNode(pg_t pg, link next) {
    link x = (link)malloc(sizeof(nodoPg_t));
    if (x == NULL) {
        printf("Errore malloc personaggi\n");
        exit(EXIT_FAILURE);
    }
    x->pg = pg;
    x->next = next;
    return x;
}

static void equip_init(equip_t *eq) {
    int i;
    eq->inUso = 0;
    for (i = 0; i < EQUIP_SLOT_MAX; i++)
        eq->vettEq[i] = -1;
}


void pgList_init(tabPg_t *tabPg) {
    tabPg->headPg = tabPg->tailPg = NULL;
    tabPg->nPg = 0;
}

void pgList_insert(tabPg_t *tabPg, pg_t pg) {
    link x = newNode(pg, NULL);
    if (tabPg->headPg == NULL) {
        tabPg->headPg = tabPg->tailPg = x;
    } else {
        tabPg->tailPg->next = x;
        tabPg->tailPg = x;
    }
    tabPg->nPg++;
}

void pgList_remove(tabPg_t *tabPg, char codice[]) {
    link x = tabPg->headPg, p = NULL;

    while (x != NULL && strcmp(x->pg.codice, codice) != 0) {
        p = x;
        x = x->next;
    }

    if (x == NULL) {
        printf("Personaggio %s non trovato\n", codice);
        return;
    }

    if (p == NULL) {
        tabPg->headPg = x->next;
        if (tabPg->tailPg == x)
            tabPg->tailPg = NULL;
    } else {
        p->next = x->next;
        if (tabPg->tailPg == x)
            tabPg->tailPg = p;
    }

    free(x);
    tabPg->nPg--;
}

pg_t *pgList_searchByCode(tabPg_t *tabPg, char codice[]) {
    link x = tabPg->headPg;
    while (x != NULL) {
        if (strcmp(x->pg.codice, codice) == 0)
            return &x->pg;
        x = x->next;
    }
    return NULL;
}

void pgList_free(tabPg_t *tabPg) {
    link x = tabPg->headPg, tmp;
    while (x != NULL) {
        tmp = x->next;
        free(x);
        x = tmp;
    }
    tabPg->headPg = tabPg->tailPg = NULL;
    tabPg->nPg = 0;
}

void pgList_read(char nomeFile[], tabPg_t *tabPg) {
    FILE *fp = fopen(nomeFile, "r");
    pg_t pg;

    if (fp == NULL) {
        printf("Errore apertura file personaggi '%s'\n", nomeFile);
        return;
    }

    pgList_init(tabPg);

    while (fscanf(fp, "%s %s %s %d %d %d %d %d %d",
                  pg.codice, pg.nome, pg.classe,
                  &pg.stat.hp, &pg.stat.mp, &pg.stat.atk,
                  &pg.stat.def, &pg.stat.mag, &pg.stat.spr) == 9) {
        equip_init(&pg.equip);
        pgList_insert(tabPg, pg);
    }

    fclose(fp);
}

void pg_statsCompute(pg_t *pg, tabInv_t *tabInv, stat_t *res) {
    int i;
    *res = pg->stat;

    if (tabInv == NULL || tabInv->vettInv == NULL)
        return;

    for (i = 0; i < EQUIP_SLOT_MAX && i < pg->equip.inUso; i++) {
        int idx = pg->equip.vettEq[i];
        inv_t *item = inv_getByIndex(tabInv, idx);
        if (item != NULL) {
            res->hp  += item->stat.hp;
            res->mp  += item->stat.mp;
            res->atk += item->stat.atk;
            res->def += item->stat.def;
            res->mag += item->stat.mag;
            res->spr += item->stat.spr;
        }
    }

    /* clamp a zero i valori negativi */
    if (res->hp  < 0) res->hp  = 0;
    if (res->mp  < 0) res->mp  = 0;
    if (res->atk < 0) res->atk = 0;
    if (res->def < 0) res->def = 0;
    if (res->mag < 0) res->mag = 0;
    if (res->spr < 0) res->spr = 0;
}

/* stampa un singolo personaggio, con o senza equipaggiamento */
void pg_print(pg_t *pg, tabInv_t *tabInv, FILE *fp, int stampaEquip) {
    int i;
    stat_t tot;

    if (pg == NULL)
        return;

    fprintf(fp, "PG %s %s %s\n", pg->codice, pg->nome, pg->classe);
    fprintf(fp, "  Stat base: HP %d MP %d ATK %d DEF %d MAG %d SPR %d\n",
            pg->stat.hp, pg->stat.mp, pg->stat.atk,
            pg->stat.def, pg->stat.mag, pg->stat.spr);

    pg_statsCompute(pg, tabInv, &tot);
    fprintf(fp, "  Stat tot : HP %d MP %d ATK %d DEF %d MAG %d SPR %d\n",
            tot.hp, tot.mp, tot.atk, tot.def, tot.mag, tot.spr);

    if (stampaEquip) {
        fprintf(fp, "  Equip (%d oggetti):\n", pg->equip.inUso);
        for (i = 0; i < EQUIP_SLOT_MAX && i < pg->equip.inUso; i++) {
            int idx = pg->equip.vettEq[i];
            inv_t *item = inv_getByIndex(tabInv, idx);
            if (item != NULL) {
                fprintf(fp, "    - ");
                inv_printItem(item, fp);
            }
        }
    }
}

void pg_printAll(tabPg_t *tabPg, tabInv_t *tabInv, FILE *fp) {
    link x = tabPg->headPg;
    while (x != NULL) {
        pg_print(&x->pg, tabInv, fp, 1);
        x = x->next;
    }
}

/* aggiunge un oggetto all'equip di un personaggio */
int pgEquip_add(tabPg_t *tabPg, tabInv_t *tabInv, char codicePg[], char nomeOggetto[]) {
    pg_t *pg = pgList_searchByCode(tabPg, codicePg);
    int idxOggetto;
    int i;

    if (pg == NULL) {
        printf("Personaggio %s non trovato\n", codicePg);
        return 0;
    }
    if (tabInv == NULL || tabInv->vettInv == NULL) {
        printf("Inventario non caricato\n");
        return 0;
    }

    idxOggetto = inv_searchByName(tabInv, nomeOggetto);
    if (idxOggetto < 0) {
        printf("Oggetto %s non trovato nell'inventario\n", nomeOggetto);
        return 0;
    }

    if (pg->equip.inUso >= EQUIP_SLOT_MAX) {
        printf("Equipaggiamento pieno per il personaggio %s\n", codicePg);
        return 0;
    }

    /* inserisco nel primo slot libero (o alla fine) */
    for (i = 0; i < EQUIP_SLOT_MAX; i++) {
        if (pg->equip.vettEq[i] == -1) {
            pg->equip.vettEq[i] = idxOggetto;
            pg->equip.inUso++;
            printf("Oggetto %s aggiunto a %s\n", nomeOggetto, codicePg);
            return 1;
        }
    }

    /* se non ho trovato slot vuoti (caso raro dato il controllo sopra) */
    printf("Nessuno slot disponibile (errore inatteso)\n");
    return 0;
}

int pgEquip_remove(tabPg_t *tabPg, tabInv_t *tabInv, char codicePg[], char nomeOggetto[]) {
    pg_t *pg = pgList_searchByCode(tabPg, codicePg);
    int idxOggetto;
    int i;

    if (pg == NULL) {
        printf("Personaggio %s non trovato\n", codicePg);
        return 0;
    }
    if (tabInv == NULL || tabInv->vettInv == NULL) {
        printf("Inventario non caricato\n");
        return 0;
    }

    idxOggetto = inv_searchByName(tabInv, nomeOggetto);
    if (idxOggetto < 0) {
        printf("Oggetto %s non trovato nell'inventario\n", nomeOggetto);
        return 0;
    }

    for (i = 0; i < EQUIP_SLOT_MAX; i++) {
        if (pg->equip.vettEq[i] == idxOggetto) {
            int j;
            for (j = i; j < EQUIP_SLOT_MAX - 1; j++)
                pg->equip.vettEq[j] = pg->equip.vettEq[j+1];
            pg->equip.vettEq[EQUIP_SLOT_MAX-1] = -1;
            if (pg->equip.inUso > 0)
                pg->equip.inUso--;
            printf("Oggetto %s rimosso da %s\n", nomeOggetto, codicePg);
            return 1;
        }
    }
    printf("Oggetto %s non era equipaggiato da %s\n", nomeOggetto, codicePg);
    return 0;
}
