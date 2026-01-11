#include "equipArray.h"

struct equipArray_s {
    int inUso;
    int vettEq[EQUIP_SLOT];
};

static void equipArray_setEmpty(equipArray_t eq) {
    int i;
    eq->inUso = 0;
    for (i = 0; i < EQUIP_SLOT; i++) eq->vettEq[i] = -1;
}

equipArray_t equipArray_init() {
    equipArray_t eq = (equipArray_t)malloc(sizeof(*eq));
    if (eq == NULL) {
        fprintf(stderr, "Errore malloc equipArray\n");
        exit(EXIT_FAILURE);
    }
    equipArray_setEmpty(eq);
    return eq;
}

void equipArray_free(equipArray_t equipArray) {
    if (equipArray == NULL) return;
    free(equipArray);
}

int equipArray_inUse(equipArray_t equipArray) {
    if (equipArray == NULL) return 0;
    return equipArray->inUso;
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index) {
    if (equipArray == NULL) return -1;
    if (index < 0 || index >= EQUIP_SLOT) return -1;
    return equipArray->vettEq[index];
}

static int equipArray_addIndex(equipArray_t eq, int objIndex) {
    int i;
    if (eq->inUso >= EQUIP_SLOT) return 0;
    /* evita duplicati */
    for (i = 0; i < EQUIP_SLOT; i++)
        if (eq->vettEq[i] == objIndex) return 0;

    for (i = 0; i < EQUIP_SLOT; i++) {
        if (eq->vettEq[i] == -1) {
            eq->vettEq[i] = objIndex;
            eq->inUso++;
            return 1;
        }
    }
    return 0;
}

static int equipArray_removeIndex(equipArray_t eq, int objIndex) {
    int i, j;
    for (i = 0; i < EQUIP_SLOT; i++) {
        if (eq->vettEq[i] == objIndex) {
            for (j = i; j < EQUIP_SLOT - 1; j++)
                eq->vettEq[j] = eq->vettEq[j + 1];
            eq->vettEq[EQUIP_SLOT - 1] = -1;
            if (eq->inUso > 0) eq->inUso--;
            return 1;
        }
    }
    return 0;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray) {
    int i;
    if (fp == NULL || equipArray == NULL) return;
    fprintf(fp, "Equip (%d oggetti):\n", equipArray->inUso);
    for (i = 0; i < EQUIP_SLOT; i++) {
        int idx = equipArray->vettEq[i];
        if (idx != -1) {
            fprintf(fp, "  [%d] ", i);
            invArray_printByIndex(fp, invArray, idx);
        }
    }
}

void equipArray_update(equipArray_t equipArray, invArray_t invArray) {
    int scelta;
    char nome[LEN];
    int idx;

    if (equipArray == NULL || invArray == NULL) return;

    printf("\n--- Modifica equipaggiamento ---\n");
    printf("1) Aggiungi oggetto\n");
    printf("2) Rimuovi oggetto\n");
    printf("Scelta: ");
    if (scanf("%d", &scelta) != 1) {
        printf("Input non valido\n");
        return;
    }

    printf("Nome oggetto: ");
    scanf("%s", nome);
    idx = invArray_searchByName(invArray, nome);
    if (idx < 0) {
        printf("Oggetto '%s' non trovato in inventario\n", nome);
        return;
    }

    if (scelta == 1) {
        if (equipArray_addIndex(equipArray, idx))
            printf("Aggiunto '%s'\n", nome);
        else
            printf("Impossibile aggiungere (equip pieno o duplicato)\n");
    } else if (scelta == 2) {
        if (equipArray_removeIndex(equipArray, idx))
            printf("Rimosso '%s'\n", nome);
        else
            printf("Oggetto non equipaggiato\n");
    } else {
        printf("Scelta non valida\n");
    }
}
