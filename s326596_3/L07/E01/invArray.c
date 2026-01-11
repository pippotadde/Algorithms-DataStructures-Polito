#include "invArray.h"

struct invArray_s {
    inv_t *vettInv;
    int nInv;
    int maxInv;
};

invArray_t invArray_init() {
    invArray_t invArray = (invArray_t)malloc(sizeof(*invArray));
    if (invArray == NULL) {
        fprintf(stderr, "Errore malloc invArray\n");
        exit(EXIT_FAILURE);
    }
    invArray->vettInv = NULL;
    invArray->nInv = 0;
    invArray->maxInv = 0;
    return invArray;
}

void invArray_free(invArray_t invArray) {
    if (invArray == NULL) return;
    free(invArray->vettInv);
    invArray->vettInv = NULL;
    invArray->nInv = 0;
    invArray->maxInv = 0;
    free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray) {
    int n, i;
    if (fp == NULL || invArray == NULL) return;

    if (fscanf(fp, "%d", &n) != 1 || n < 0) {
        fprintf(stderr, "Formato inventario non valido\n");
        return;
    }

    free(invArray->vettInv);
    invArray->vettInv = NULL;
    invArray->nInv = 0;
    invArray->maxInv = 0;

    if (n == 0) return;

    invArray->vettInv = (inv_t *)malloc((size_t)n * sizeof(inv_t));
    if (invArray->vettInv == NULL) {
        fprintf(stderr, "Errore malloc inventario\n");
        exit(EXIT_FAILURE);
    }
    invArray->nInv = n;
    invArray->maxInv = n;

    for (i = 0; i < n; i++) {
        inv_read(fp, &invArray->vettInv[i]);
    }
}

void invArray_print(FILE *fp, invArray_t invArray) {
    int i;
    if (fp == NULL || invArray == NULL) return;
    fprintf(fp, "Inventario (%d oggetti):\n", invArray->nInv);
    for (i = 0; i < invArray->nInv; i++) {
        fprintf(fp, "%2d) ", i);
        inv_print(fp, &invArray->vettInv[i]);
    }
}

void invArray_printByIndex(FILE *fp, invArray_t invArray, int index) {
    inv_t *invp;
    if (fp == NULL || invArray == NULL) return;
    invp = invArray_getByIndex(invArray, index);
    if (invp == NULL) {
        fprintf(fp, "[indice %d non valido]\n", index);
        return;
    }
    inv_print(fp, invp);
}

inv_t *invArray_getByIndex(invArray_t invArray, int index) {
    if (invArray == NULL || invArray->vettInv == NULL) return NULL;
    if (index < 0 || index >= invArray->nInv) return NULL;
    return &invArray->vettInv[index];
}

int invArray_searchByName(invArray_t invArray, char *name) {
    int i;
    if (invArray == NULL || invArray->vettInv == NULL || name == NULL) return -1;
    for (i = 0; i < invArray->nInv; i++) {
        if (strcmp(invArray->vettInv[i].nome, name) == 0) return i;
    }
    return -1;
}
