#include "inv.h"

static int clamp_min(int v, int soglia) {
    if (soglia <= 0) { return v; }
    if (v < soglia) { return soglia; }
    else return v;
}

void stat_read(FILE *fp, stat_t *statp) {
    if (fp == NULL || statp == NULL) return;
    fscanf(fp, "%d %d %d %d %d %d",
           &statp->hp, &statp->mp, &statp->atk,
           &statp->def, &statp->mag, &statp->spr);
}

void stat_print(FILE *fp, stat_t *statp, int soglia) {
    if (fp == NULL || statp == NULL) return;
    fprintf(fp, "HP %d MP %d ATK %d DEF %d MAG %d SPR %d",
            clamp_min(statp->hp, soglia),
            clamp_min(statp->mp, soglia),
            clamp_min(statp->atk, soglia),
            clamp_min(statp->def, soglia),
            clamp_min(statp->mag, soglia),
            clamp_min(statp->spr, soglia));
}

void inv_read(FILE *fp, inv_t *invp){
    if (fp == NULL || invp == NULL) return;
    fscanf(fp, "%s %s", invp->nome, invp->tipo);
    stat_read(fp, &invp->stat);
}

void inv_print(FILE *fp, inv_t *invp){
    if (fp == NULL || invp == NULL) return;
    fprintf(fp, "%s %s ", invp->nome, invp->tipo);
    stat_print(fp, &invp->stat, 0);
    fprintf(fp, "\n");
}

stat_t inv_getStat(inv_t *invp) {
    stat_t vuota = {0, 0, 0, 0, 0, 0};
    if (invp == NULL) return vuota;
    return invp->stat;
}