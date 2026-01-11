#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"

#define MAXC 31

typedef struct node *link;
struct node { int v; int wt; int id; link next; };
struct graph { int V; int E; int Emax; link *ladj; Edge *edges; ST tab; link z; };

static link NEW(int v, int wt, int id, link next) {
    link x = malloc(sizeof *x);
    if (x == NULL) { return NULL; }
    x->v = v;
    x->wt = wt;
    x->id = id;
    x->next = next;
    return x;
}

Graph GRAPHinit(int V) {
    int v;
    Graph G = malloc(sizeof *G);
    if (G == NULL) { return NULL; }

    G->V = V;
    G->E = 0;
    G->Emax = V;
    G->z = NEW(-1, 0, -1, NULL);
    G->ladj = malloc(G->V*sizeof(link));
    for (v = 0; v < G->V; v++) { G->ladj[v] = G->z; }

    G->tab = STinit(V);
    G->edges = malloc(G->Emax * sizeof(Edge));

    return G;
}

void GRAPHfree(Graph G) {
    int v;
    link t, next;
    for (v=0; v < G->V; v++) {
        for (t=G->ladj[v]; t != G->z; t = next) {
            next = t->next;
            free(t);
        }
    }
    STfree(G->tab);
    free(G->ladj);
    free(G->edges);
    free(G->z);
    free(G);
}

int GRAPHgetV(Graph G) {
    return G->V;
}

void GRAPHinsertE(Graph G, int id1, int id2, int wt) {
    if (G->E >= G->Emax) {
        G->Emax = (G->Emax == 0) ? 1 : 2 * G->Emax;
        G->edges = realloc(G->edges, G->Emax * sizeof(Edge));
    }
    G->edges[G->E].v = id1;
    G->edges[G->E].w = id2;
    G->edges[G->E].wt = wt;

    G->ladj[id1] = NEW(id2, wt, G->E, G->ladj[id1]);
    G->E++;
}

Graph GRAPHload(FILE *fin) {
    int V, i, id1, id2, wt;
    char label1[MAXC], label2[MAXC];
    Graph G;

    if(fscanf(fin, "%d", &V) != 1) { return NULL; }
    G = GRAPHinit(V);

    for (i=0; i<V; i++) {
        fscanf(fin, "%s", label1);
        STinsert(G->tab, label1, i);
    }

    while(fscanf(fin, "%s %s %d", label1, label2, &wt) == 3) {
        id1 = STsearch(G->tab, label1);
        id2 = STsearch(G->tab, label2);
        if (id1 >= 0 && id2 >= 0) {
            GRAPHinsertE(G, id1, id2, wt);
        }
    }
    return G;
}

void GRAPHstore(Graph G, FILE *fout) {
    int i;
    fprintf(fout, "%d\n", G->V);
    for (i = 0; i < G->V; i++) {
        fprintf(fout, "%s\n", STsearchByIndex(G->tab, i));
    }
    for (i = 0; i < G->E; i++) {
        fprintf(fout, "%s %s %d\n",
                STsearchByIndex(G->tab, G->edges[i].v),
                STsearchByIndex(G->tab, G->edges[i].w),
                G->edges[i].wt);
    }
}

static int dfsCycle(Graph G, int u, int *color, int *removed) {
    link t;
    color[u] = 1;
    for (t = G->ladj[u]; t != G->z; t = t->next) {
        if (removed[t->id] != 0) { continue; }
        int v = t->v;
        if (color[v] == 1) { return 1; }
        if (color[v] == 0) {
            if (dfsCycle(G, v, color, removed) != 0) { return 1; }
        }
    }
    color[u] = 2;
    return 0;
}

int isDAG(Graph G, int *removed) {
    int i;
    int *color = calloc(G->V, sizeof(int));
    for (i = 0; i < G->V; i++) {
        if (color[i] == 0) {
            if (dfsCycle(G, i, color, removed) != 0) {
                free(color);
                return 0;
            }
        }
    }
    free(color);
    return 1;
}

typedef struct {
    int *edges;
    int weight;
} EdgeSet;

static void printEdgeSet(Graph G, int *removed) {
    int first = 1;
    printf("{ ");
    for (int i = 0; i < G->E; i++) {
        if (removed[i] != 0) {
            if (first == 0) { printf(", "); }
            printf("%s->%s",
                   STsearchByIndex(G->tab, G->edges[i].v),
                   STsearchByIndex(G->tab, G->edges[i].w));
            first = 0;
        }
    }
    printf(" }");
}

void comb(Graph G, int k, int start, int depth, int *removed,
          EdgeSet **allSets, int *numSets, int *maxSets) {
    if (depth == k) {
        if (isDAG(G, removed) != 0) {
            if (*numSets >= *maxSets) {
                *maxSets = (*maxSets == 0) ? 8 : (*maxSets * 2);
                *allSets = realloc(*allSets, (*maxSets) * sizeof(EdgeSet));
            }

            (*allSets)[*numSets].edges = malloc(G->E * sizeof(int));
            (*allSets)[*numSets].weight = 0;

            for (int i = 0; i < G->E; i++) {
                (*allSets)[*numSets].edges[i] = removed[i];
                if (removed[i] != 0) {
                    (*allSets)[*numSets].weight += G->edges[i].wt;
                }
            }
            (*numSets)++;
        }
        return;
    }

    for (int i = start; i < G->E; i++) {
        removed[i] = 1;
        comb(G, k, i + 1, depth + 1, removed, allSets, numSets, maxSets);
        removed[i] = 0;
    }
}

int *costruisciDAG(Graph G, int *bestK_out, int *bestWeight_out) {
    int *removed = calloc(G->E, sizeof(int));
    int *bestRemoved = calloc(G->E, sizeof(int));
    EdgeSet *allSets = NULL;
    int numSets = 0;
    int maxSets = 0;
    int k;

    for (k = 0; k <= G->E; k++) {
        numSets = 0;
        if (allSets != NULL) {
            for (int i = 0; i < numSets; i++) {
                free(allSets[i].edges);
            }
            free(allSets);
            allSets = NULL;
            maxSets = 0;
        }

        comb(G, k, 0, 0, removed, &allSets, &numSets, &maxSets);

        if (numSets > 0) {
            printf("\nInsiemi di cardinalita minima k=%d (totale: %d):\n", k, numSets);

            int maxWeight = -1;
            int maxIdx = 0;

            for (int i = 0; i < numSets; i++) {
                printf("  Set %d (peso=%d): ", i+1, allSets[i].weight);
                printEdgeSet(G, allSets[i].edges);
                printf("\n");

                if (allSets[i].weight > maxWeight) {
                    maxWeight = allSets[i].weight;
                    maxIdx = i;
                }
            }

            printf("\nInsieme selezionato (peso massimo=%d): ", maxWeight);
            printEdgeSet(G, allSets[maxIdx].edges);
            printf("\n");

            for (int i = 0; i < G->E; i++) {
                bestRemoved[i] = allSets[maxIdx].edges[i];
            }

            *bestK_out = k;
            *bestWeight_out = maxWeight;

            for (int i = 0; i < numSets; i++) {
                free(allSets[i].edges);
            }
            free(allSets);
            free(removed);
            return bestRemoved;
        }
    }

    free(removed);
    free(allSets);
    *bestK_out = 0;
    *bestWeight_out = 0;
    return bestRemoved;
}

void tsDFS(Graph G, int u, int *visited, int *removed, int *ts, int *tsIndex) {
    link t;
    visited[u] = 1;
    for (t = G->ladj[u]; t != G->z; t = t->next) {
        if (removed[t->id] == 0 && visited[t->v] == 0) {
            tsDFS(G, t->v, visited, removed, ts, tsIndex);
        }
    }
    ts[(*tsIndex)--] = u;
}

void GRAPHlongestPath(Graph G, int *removed) {
    int *inDegree = calloc(G->V, sizeof(int));
    int i, j;
    link t;

    for (i = 0; i < G->V; i++) {
        for (t = G->ladj[i]; t != G->z; t = t->next) {
            if (removed[t->id] == 0) { inDegree[t->v]++; }
        }
    }

    printf("\nCalcolo distanze massime dai nodi sorgente:\n");

    int *ts = malloc(G->V * sizeof(int));
    int *visited = calloc(G->V, sizeof(int));
    int tsIndex = G->V - 1;

    for(i=0; i<G->V; i++) {
        if(visited[i] == 0 && inDegree[i] == 0) {
            tsDFS(G, i, visited, removed, ts, &tsIndex);
        }
    }
    for(i=0; i<G->V; i++) {
        if(visited[i] == 0) { tsDFS(G, i, visited, removed, ts, &tsIndex); }
    }

    int *dist = malloc(G->V * sizeof(int));

    for (i = 0; i < G->V; i++) {
        if (inDegree[i] == 0) {
            printf("\nSorgente: %s\n", STsearchByIndex(G->tab, i));

            for(j=0; j<G->V; j++) { dist[j] = -1; }
            dist[i] = 0;

            for (j = 0; j < G->V; j++) {
                int u = ts[j];
                if (dist[u] != -1) {
                    for (t = G->ladj[u]; t != G->z; t = t->next) {
                        if (removed[t->id] == 0) {
                            if (dist[t->v] < dist[u] + t->wt) {
                                dist[t->v] = dist[u] + t->wt;
                            }
                        }
                    }
                }
            }

            for (j = 0; j < G->V; j++) {
                if (j != i) {
                    if (dist[j] >= 0) {
                        printf("  -> %s: %d\n", STsearchByIndex(G->tab, j), dist[j]);
                    } else {
                        printf("  -> %s: non raggiungibile\n", STsearchByIndex(G->tab, j));
                    }
                }
            }
        }
    }

    free(inDegree);
    free(ts);
    free(visited);
    free(dist);
}