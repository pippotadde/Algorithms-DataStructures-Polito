#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaggi.h"
#include "inventario.h"

static void stampaMenu(void) {
    printf("\n=== MENU ===\n");
    printf("1) Carica personaggi da file\n");
    printf("2) Carica inventario da file\n");
    printf("3) Aggiungi nuovo personaggio\n");
    printf("4) Elimina personaggio\n");
    printf("5) Aggiungi oggetto all'equip di un personaggio\n");
    printf("6) Rimuovi oggetto dall'equip di un personaggio\n");
    printf("7) Stampa dettagli di un personaggio\n");
    printf("8) Stampa tutti i personaggi\n");
    printf("9) Stampa inventario\n");
    printf("0) Uscita\n");
    printf("Scelta: ");
}

int main(void) {
    tabPg_t tabPg;
    tabInv_t tabInv;
    int fine = 0;
    int scelta;
    int caricatiPg = 0;
    int caricatiInv = 0;
    pgList_init(&tabPg);
    tabInv.vettInv = NULL;
    tabInv.nInv = tabInv.maxInv = 0;

    while (!fine) {
        stampaMenu();
        if (scanf("%d", &scelta) != 1) {
            printf("Input non valido\n");
            return 0;
        }
        if (scelta == 0) {
            fine = 1;
        } else if (scelta == 1) {
            char nomeFile[256];
            printf("Nome file personaggi (es. pg.txt): ");
            scanf("%s", nomeFile);
            pgList_read(nomeFile, &tabPg);
            caricatiPg = 1;
        } else if (scelta == 2) {
            char nomeFile[256];
            printf("Nome file inventario (es. inventario.txt): ");
            scanf("%s", nomeFile);
            inv_read(nomeFile, &tabInv);
            caricatiInv = (tabInv.vettInv != NULL);
        } else if (scelta == 3) {
            pg_t pg;
            printf("Inserisci codice nome classe: ");
            scanf("%s %s %s", pg.codice, pg.nome, pg.classe);
            printf("Inserisci hp mp atk def mag spr: ");
            scanf("%d %d %d %d %d %d",
                  &pg.stat.hp, &pg.stat.mp, &pg.stat.atk,
                  &pg.stat.def, &pg.stat.mag, &pg.stat.spr);
            /* equip vuoto */
            {
                int i;
                pg.equip.inUso = 0;
                for (i = 0; i < EQUIP_SLOT_MAX; i++)
                    pg.equip.vettEq[i] = -1;
            }
            pgList_insert(&tabPg, pg);
        } else if (scelta == 4) {
            char codice[ MAXC + 1 ];
            printf("Codice personaggio da eliminare: ");
            scanf("%s", codice);
            pgList_remove(&tabPg, codice);
        } else if (scelta == 5) {
            char codice[ MAXC + 1 ];
            char nomeOggetto[ MAXC + 1 ];
            if (!caricatiPg || !caricatiInv) {
                printf("Carica prima personaggi e inventario\n");
            } else {
                printf("Codice personaggio: ");
                scanf("%s", codice);
                printf("Nome oggetto da aggiungere: ");
                scanf("%s", nomeOggetto);
                pgEquip_add(&tabPg, &tabInv, codice, nomeOggetto);
            }
        } else if (scelta == 6) {
            char codice[ MAXC + 1 ];
            char nomeOggetto[ MAXC + 1 ];
            if (!caricatiPg || !caricatiInv) {
                printf("Carica prima personaggi e inventario\n");
            } else {
                printf("Codice personaggio: ");
                scanf("%s", codice);
                printf("Nome oggetto da rimuovere: ");
                scanf("%s", nomeOggetto);
                pgEquip_remove(&tabPg, &tabInv, codice, nomeOggetto);
            }
        } else if (scelta == 7) {
            char codice[ MAXC + 1 ];
            pg_t *pg;
            if (!caricatiPg) {
                printf("Nessun personaggio caricato\n");
            } else {
                printf("Codice personaggio da stampare: ");
                scanf("%s", codice);
                pg = pgList_searchByCode(&tabPg, codice);
                if (pg == NULL)
                    printf("Personaggio non trovato\n");
                else
                    pg_print(pg, caricatiInv ? &tabInv : NULL, stdout, 1);
            }
        } else if (scelta == 8) {
            if (!caricatiPg) {
                printf("Nessun personaggio caricato\n");
            } else {
                pg_printAll(&tabPg, caricatiInv ? &tabInv : NULL, stdout);
            }
        } else if (scelta == 9) {
            if (!caricatiInv || tabInv.vettInv == NULL) {
                printf("Inventario non caricato\n");
            } else {
                inv_print(&tabInv, stdout);
            }
        } else {
            printf("Scelta non valida\n");
        }
    }
    pgList_free(&tabPg);
    inv_free(&tabInv);
    return 0;
}
