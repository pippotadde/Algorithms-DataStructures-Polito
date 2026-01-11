#include "pg.h"

static stat_t stat_zero(void) {
    stat_t s = {0, 0, 0, 0, 0, 0};
    return s;
}

static stat_t stat_add(stat_t a, stat_t b) {
    stat_t r;
    r.hp  = a.hp  + b.hp;
    r.mp  = a.mp  + b.mp;
    r.atk = a.atk + b.atk;
    r.def = a.def + b.def;
    r.mag = a.mag + b.mag;
    r.spr = a.spr + b.spr;
    return r;
}

static stat_t pg_computeEquipStat(pg_t *pgp, invArray_t invArray) {
    int i;
    stat_t sum = stat_zero();

    if (pgp == NULL || invArray == NULL || pgp->equip == NULL) return sum;

    for (i = 0; i < EQUIP_SLOT; i++) {
        int idx = equipArray_getEquipByIndex(pgp->equip, i);
        if (idx != -1) {
            inv_t *invp = invArray_getByIndex(invArray, idx);
            if (invp != NULL) sum = stat_add(sum, inv_getStat(invp));
        }
    }
    return sum;
}

int pg_read(FILE *fp, pg_t *pgp) {
    if (fp == NULL || pgp == NULL) return 0;
    if (fscanf(fp, "%s %s %s %d %d %d %d %d %d",
               pgp->cod, pgp->nome, pgp->classe,
               &pgp->b_stat.hp, &pgp->b_stat.mp, &pgp->b_stat.atk,
               &pgp->b_stat.def, &pgp->b_stat.mag, &pgp->b_stat.spr) != 9) {
        return 0;
    }
    pgp->equip = equipArray_init();
    return 1;
}

void pg_clean(pg_t *pgp) {
    if (pgp == NULL) return;
    if (pgp->equip != NULL) {
        equipArray_free(pgp->equip);
        pgp->equip = NULL;
    }
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray) {
    stat_t tot;
    if (fp == NULL || pgp == NULL) return;

    pgp->eq_stat = pg_computeEquipStat(pgp, invArray);
    tot = stat_add(pgp->b_stat, pgp->eq_stat);

    fprintf(fp, "PG %s %s %s\n", pgp->cod, pgp->nome, pgp->classe);
    fprintf(fp, "  Base : ");
    stat_print(fp, &pgp->b_stat, 0);
    fprintf(fp, "\n");

    fprintf(fp, "  Equip: ");
    stat_print(fp, &pgp->eq_stat, 0);
    fprintf(fp, "\n");

    fprintf(fp, "  Tot  : ");
    stat_print(fp, &tot, MIN_STAT);
    fprintf(fp, "\n");

    if (pgp->equip != NULL && equipArray_inUse(pgp->equip) > 0) {
        equipArray_print(fp, pgp->equip, invArray);
    }
}

void pg_updateEquip(pg_t *pgp, invArray_t invArray) {
    if (pgp == NULL || invArray == NULL || pgp->equip == NULL) return;
    equipArray_update(pgp->equip, invArray);
    pgp->eq_stat = pg_computeEquipStat(pgp, invArray);
}