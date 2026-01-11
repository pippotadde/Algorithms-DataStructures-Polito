#include "grafo.h"

#include <stdlib.h>
#include <string.h>

struct node_s {
    int indice;        // indice
    int flusso;        // peso (flusso)
    node_t next;
};

// grafo
struct graph_s {
    int V;              // numero nodi
    char **nomi;        // indice → nome nodo
    int E;              // numero archi
    int **madj;         // matrice simboli
    node_t *ladj;       // lista adaicenza
};

graph_t GRAPHinit(void) {
    graph_t G = malloc(sizeof(*G));
    if (G == NULL) return NULL;
    G->V = 0;
    G->E = 0;
    G->nomi = NULL;
    G->madj = NULL;
    G->ladj = NULL;
    return G;
}

void GRAPHfree(graph_t G) {
    if (G == NULL) return;

    // libera liste di adiacenza
    if (G->ladj != NULL) {
        for (int i = 0; i < G->V; i++) {
            node_t p = G->ladj[i];
            while (p != NULL) {
                node_t t = p->next;
                free(p);
                p = t;
            }
        }
        free(G->ladj);
    }

    // libera matrice di adiacenza
    if (G->madj != NULL) {
        for (int i = 0; i < G->V; i++) {
            free(G->madj[i]);
        }
        free(G->madj);
    }

    // libera nomi
    if (G->nomi != NULL) {
        for (int i = 0; i < G->V; i++) {
            free(G->nomi[i]);
        }
        free(G->nomi);
    }

    free(G);
}


// trova indice o crea nodo
int GRAPHgetIndex(graph_t G, char *name) {
    for (int i = 0; i < G->V; i++) {
        if (strcmp(G->nomi[i], name) == 0) {
            return i;
        }
    }

    G->nomi = realloc(G->nomi, (G->V + 1) * sizeof(char *));
    G->nomi[G->V] = strdup(name);

    G->madj = realloc(G->madj, (G->V + 1) * sizeof(int *));
    G->madj[G->V] = malloc((G->V + 1) * sizeof(int));

    for (int i = 0; i <= G->V; i++) {
        G->madj[G->V][i] = -1;
    }

    for (int i = 0; i < G->V; i++) {
        G->madj[i] = realloc(G->madj[i], (G->V + 1) * sizeof(int));
        G->madj[i][G->V] = -1;
    }

    return G->V++;
}

void GRAPHinsertArc(graph_t G, int i, int j, int flusso) {
    if (i < 0 || j < 0 || i == j || i >= G->V || j >= G->V || G == NULL) { return; }
    if (G->madj[i][j] == -1) {
        G->E++;
    }
    G->madj[i][j] = flusso;
    G->madj[j][i] = flusso;
}


graph_t GRAPHload(FILE *fp) {
    graph_t G = GRAPHinit();
    char e1[MAXC], r1[MAXC], e2[MAXC], r2[MAXC];
    int flusso;

    while (fscanf(fp, "%30s %30s %30s %30s %d",
                  e1, r1, e2, r2, &flusso) == 5) {
        int i = GRAPHgetIndex(G, e1);
        int j = GRAPHgetIndex(G, e2);
        GRAPHinsertArc(G, i, j, flusso);
    }
    return G;
}

static graph_t Gcopia = NULL;

static int cmpIdxByName(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return strcmp(Gcopia->nomi[ia], Gcopia->nomi[ib]);
}

void GRAPHorder(graph_t G) {
    if (G == NULL || G->V <= 0) { return; }

    int *ord = malloc(G->V * sizeof(int));
    if (ord == NULL) { return; }

    for (int i = 0; i < G->V; i++) {
        ord[i] = i;
    }

    Gcopia = G;
    qsort(ord, G->V, sizeof(int), cmpIdxByName);
    Gcopia = NULL;

    printf("\nVertici (alfabetico) e archi incidenti (alfabetico):\n");
    for (int k = 0; k < G->V; k++) {
        int ver1 = ord[k];

        int *nb = malloc(G->V * sizeof(int));
        if (nb == NULL) { free(ord); return; }

        int i = 0;
        for (int ver2 = 0; ver2 < G->V; ver2++) {
            if (G->madj[ver1][ver2] != -1) {
                nb[i++] = ver2;
            }
        }

        if (i > 1) {
            Gcopia = G;
            qsort(nb, i, sizeof(int), cmpIdxByName);
            Gcopia = NULL;
        }

        // stampa
        printf("%s:", G->nomi[ver1]);
        for (int t = 0; t < i; t++) {
            int ver2 = nb[t];
            printf(" %s(%d)", G->nomi[ver2], G->madj[ver1][ver2]);
        }
        printf("\n");
        free(nb);
    }
    free(ord);
}

int GRAPHgetIndex2(graph_t G, char *name) {
    for (int i = 0; i < G->V; i++) {
        if (strcmp(G->nomi[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

int GRAPHcheck3Matrix(graph_t G, char *n1, char *n2, char *n3) {
    int ind1 = GRAPHgetIndex2(G, n1);
    int ind2 = GRAPHgetIndex2(G, n2);
    int ind3 = GRAPHgetIndex2(G, n3);
    if (ind1 == -1 || ind2 == -1 || ind3 == -1) { return 0; }
    if (G->madj[ind1][ind2] == -1 || G->madj[ind2][ind1] == -1) { return 0; }
    if (G->madj[ind1][ind3] == -1 || G->madj[ind3][ind1] == -1) { return 0; }
    if (G->madj[ind2][ind3] == -1 || G->madj[ind3][ind2] == -1) { return 0; }
    return 1;
}

static int isAdjacentList(graph_t G, int i, int j) {
    for (node_t x = G->ladj[i]; x != NULL; x = x->next) {
        if (x->indice == j)
            return 1;
    }
    return 0;
}

int GRAPHcheck3List(graph_t G, char *n1, char *n2, char *n3) {
    if (G == NULL || G->ladj == NULL) { return 0; }

    int a = GRAPHgetIndex2(G, n1);
    int b = GRAPHgetIndex2(G, n2);
    int c = GRAPHgetIndex2(G, n3);
    if (a < 0 || b < 0 || c < 0) { return 0; }

    if (isAdjacentList(G, a, b) &&
        isAdjacentList(G, a, c) &&
        isAdjacentList(G, b, c))
    {
        return 1;
    }
    return 0;
}

node_t newNode(int indice, int flusso, node_t next) {
    node_t x = malloc(sizeof(*x));
    if (x == NULL) return NULL;
    x->indice = indice;
    x->flusso = flusso;
    x->next   = next;
    return x;
}

void resetladj(graph_t G) {
    if (G == NULL || G->ladj == NULL) return;

    for (int v = 0; v < G->V; v++) {
        node_t nodox = G->ladj[v];
        while (nodox != NULL) {
            node_t nodot = nodox->next;
            free(nodox);
            nodox = nodot;
        }
        G->ladj[v] = NULL;
    }
    free(G->ladj);
    G->ladj = NULL;
}


void GRAPHconvertToList(graph_t G) {
    resetladj(G);
    G->ladj = calloc(G->V, sizeof(node_t));
    if (G->ladj == NULL) { return; }
    for (int i = 0; i < G->V; i++) {
        for (int j = 0; j < G->V; j++) {
            if (G->madj[i][j] != -1) {
                G->ladj[i] = newNode(j, G->madj[i][j], G->ladj[i]);
            }
        }
    }
}

void GRAPHprintList(graph_t G) {
    if (G == NULL || G->ladj == NULL) { return; }

    printf("Lista di adiacenza:\n");
    for (int i = 0; i < G->V; i++) {
        printf("%s:", G->nomi[i]);
        for (node_t p = G->ladj[i]; p != NULL; p = p->next) {
            printf(" -> %s(%d)", G->nomi[p->indice], p->flusso);
        }
        printf("\n");
    }
}

void GRAPHprintMatrix(graph_t G) {
    if (G == NULL) return;
    printf("Matrice di adiacenza:\n    ");
    for (int i = 0; i < G->V; i++) {
        printf("%4s", G->nomi[i]);
    }
    printf("\n");
    for (int i = 0; i < G->V; i++) {
        printf("%4s", G->nomi[i]);
        for (int j = 0; j < G->V; j++) {
            if (G->madj[i][j] == -1) {
                printf("%4s", ".");
            } else {
                printf("%4d", G->madj[i][j]);
            }
        }
        printf("\n");
    }
}
