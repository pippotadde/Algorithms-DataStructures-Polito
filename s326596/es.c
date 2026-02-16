#include "es.h"

struct key {int id; char *nome; Date nascita; Date morte; link next;};
struct graph {int V; int E; link *ladj; ST tab; link z;};

int KEYcmp(link k1, link k2) {
    if ( strcmp(k1->nome, k2->nome) == 0) {
        if (k1->nascita->aa != k2->nascita->aa) {
            return k1->nascita->aa - k2->nascita->aa;
        }
        if (k1->nascita->mm != k2->nascita->mm) {
            return k1->nascita->mm - k2->nascita->mm;
        }
        return k1->nascita->dd - k2->nascita->dd;
    }
    return strcmp(k1->nome, k2->nome);
}

int relationshipK(Graph G, char *nome1, Date d1, char *nome2, Date d2, int k) {
    int id1 = STgetindexByNameDate(G->tab, nome1, d1->dd, d1->mm, d1->aa);
    int id2 = STgetindexByNameDate(G->tab, nome2, d2->dd, d2->mm, d2->aa);
    int *vist = calloc(G->V, sizeof(int));
    int *dist = calloc(G->V, sizeof(int));
    int *coda = malloc(G->V * sizeof(int));
    int head = 0, tail = 0;
    coda[tail++] = id1;
    vist[id1] = 1;
    dist[id1] = 0;
    while (head < tail) {
        int v = coda[head++];
        if (dist[v] >= k) {
            continue;
        }
        for (link t = G->ladj[v]; t != G->z; t = t->next) {
            if (vist[t->id] == 0) {
                vist[t->id] = 1;
                dist[t->id] = dist[v] + 1;
                coda[tail++] = t->id;
                if (t->id == id2) {
                    free(vist);
                    free(dist);
                    free(coda);
                    return 1;
                }
            }
        }
    }

    head = 0;
    tail = 0;
    coda[tail++] = id2;
    int *vist2 = calloc(G->V, sizeof(int));
    int *dist2 = calloc(G->V, sizeof(int));
    vist2[id2] = 1;
    dist2[id2] = 0;
    while (head < tail) {
        int v = coda[head++];
        if (dist2[v] >= k) {
            continue;
        }
        for (link t = G->ladj[v]; t != G->z; t = t->next) {
            int nuova_dist = dist2[v] + 1;
            if (t->id == id1) {
                if (nuova_dist <= k) {
                    free(vist);
                    free(dist);
                    free(coda);
                    free(vist2);
                    free(dist2);
                    return 1;
                }
            }
            // Caso 3: antenato comune
            if (vist[t->id] == 1 && (dist[t->id] + nuova_dist) <= k) {
                free(vist);
                free(dist);
                free(coda);
                free(vist2);
                free(dist2);
                return 1;
            }
            if (vist2[t->id] == 0) {
                vist2[t->id] = 1;
                dist2[t->id] = nuova_dist;
                coda[tail++] = t->id;
            }
        }
    }
    free(vist);
    free(dist);
    free(coda);
    free(vist2);
    free(dist2);
    return 0;
}

static int bfs(Graph G, int id, int k) {
    int countD = 0;
    int *vis = calloc(G->V, sizeof(int));
    int *dist = malloc(G->V * sizeof(int));
    for (int i = 0; i < G->V; i++) {
        dist[i] = -1;
    }
    PQ q = PQinit(G->V);
    dist[id] = 0;
    vis[id] = 1;
    PQinsert(q, id);
    while(PQempty(q) != 1) {
        int v = PQget(q);
        if (dist[v] == k) { continue; }
        for(link t = G->ladj[v]; t != G->z; t=t->next) {
            if (vis[t->id] != 1) {
                vis[t->id] = 1;
                dist[t->id] = dist[v] + 1;
                if (dist[t->id] <= k) {
                    countD++;
                    PQinsert(q, t->id);
                }
            }
        }
    }
    free(vis);
    free(dist);
    PQfree(q);
    return countD;
}

void largestLineageK(Graph G, int k) {
    int bestDiscendenti = 0;
    int bestID = 0;

    for(int id=0; id<G->V; id++) {
        int n_discendenti = bfs(G, id, k);
        if (n_discendenti > bestDiscendenti) {
            bestDiscendenti = n_discendenti;
            bestID = id;
        }
    }
    char *nome = STsearchByIndex(G->tab, bestID);
    printf("%s\n", nome);
    return;
}

Date date_init(void) {
    Date D = malloc(sizeof(*D));
    if (D == NULL) return NULL;

    D->dd = 0;
    D->mm = 0;
    D->aa = 0;
    return D;
}

void date_free(Date D) {
    if (D == NULL) return;
    free(D);
}

link key_init(void) {
    link K = malloc(sizeof(*K));
    if (K == NULL) return NULL;
    K->id = -1;
    K->nome = NULL;
    K->nascita = date_init();
    K->morte   = date_init();
    K->next = NULL;
    return K;
}

void key_free(link K) {
    if (K == NULL) { return; }
    if (K->nome != NULL) {
        free(K->nome);
    }
    date_free(K->nascita);
    date_free(K->morte);
    free(K);
}

Graph graph_init(int n) {
    int i;
    Graph G = malloc(sizeof(*G));
    if (G == NULL) return NULL;
    G->V = n;
    G->E = 0;
    G->ladj = malloc(n * sizeof(link));
    if (G->ladj == NULL) {
        free(G);
        return NULL;
    }
    G->z = key_init();
    if (G->z == NULL) {
        free(G->ladj);
        free(G);
        return NULL;
    }
    G->z->id = -1;
    if (G->z->nome) {
        free(G->z->nome);
        G->z->nome = NULL;
    }
    G->z->next = G->z;
    for (i = 0; i < n; i++) {
        G->ladj[i] = G->z;
    }
    G->tab = STinit(n);
    return G;
}

void graph_free(Graph G) {
    int i;
    link x, t;
    if (G == NULL) { return; }
    if (G->ladj != NULL) {
        for (i = 0; i < G->V; i++) {
            x = G->ladj[i];
            while (x != NULL && x != G->z) {
                t = x->next;
                key_free(x);
                x = t;
            }
        }
        free(G->ladj);
    }
    STfree(G->tab);
    key_free(G->z);
    free(G);
}

Date readDate() {
    int dd, mm, aa;
    if (scanf("%d %d %d", &dd, &mm, &aa) != 3) {
        return NULL;
    }
    Date d = date_init();
    if (d == NULL) {
        return NULL;
    }
    d->dd = dd; d->mm = mm; d->aa = aa;
    return d;
}

link readKey(void) {
    char nome[65];

    link K = key_init();
    if (K == NULL) return NULL;

    printf("\nInserisci nome: ");
    scanf(" %64[^\n]", nome);
    printf("\nInserire data nascita (dd mm aa): ");
    Date d_nascita = readDate();
    printf("\nInserire data morte (dd mm aa): ");
    Date d_morte = readDate();

    K->nome = strdup(nome);
    K->nascita = d_nascita;
    K->morte = d_morte;
    K->next = NULL;
    return K;
}

static link NEWadj(int id, link next) {
    link x = malloc(sizeof(*x));
    if (x == NULL) return NULL;
    x->id = id;
    x->nome = NULL;
    x->nascita = NULL;
    x->morte = NULL;
    x->next = next;
    return x;
}

void GRAPHinsertE(Graph G, int id1, int id2) {
    if (G == NULL) return;
    if (id1 < 0 || id1 >= G->V) { return; }
    if (id2 < 0 || id2 >= G->V) { return; }
    G->ladj[id1] = NEWadj(id2, G->ladj[id1]);
    G->E++;
}

Graph GRAPHload(FILE *fp) {
    int V, E;
    char cognome[31], nome[31];
    int dd, mm, aa, dd2, mm2, aa2;

    if (fscanf(fp, "%d", &V) != 1) {
        return NULL;
    }

    Graph G = graph_init(V);
    if (G == NULL) {
        return NULL;
    }

    for (int i = 0; i < V; i++) {
        if (fscanf(fp, "%30s %30s %d/%d/%d %d/%d/%d",
                   cognome, nome, &dd, &mm, &aa, &dd2, &mm2, &aa2) != 8) {
            graph_free(G);
            return NULL;
        }

        char nomeCompleto[65];
        sprintf(nomeCompleto, "%s %s", cognome, nome);

        char key[100];
        sprintf(key, "%s|%02d/%02d/%04d", nomeCompleto, dd, mm, aa);

        STinsert(G->tab, key, i);
    }

    if (fscanf(fp, "%d", &E) != 1) {
        graph_free(G);
        return NULL;
    }

    for (int i = 0; i < E; i++) {
        char cog1[31], nom1[31], cog2[31], nom2[31];
        int d1, m1, a1, d2, m2, a2;

        if (fscanf(fp, "%30s %30s %d/%d/%d %30s %30s %d/%d/%d",
                   cog1, nom1, &d1, &m1, &a1, cog2, nom2, &d2, &m2, &a2) != 10) {
            graph_free(G);
            return NULL;
        }

        char nomeC1[65], nomeC2[65];
        sprintf(nomeC1, "%s %s", cog1, nom1);
        sprintf(nomeC2, "%s %s", cog2, nom2);

        int id1 = STgetindexByNameDate(G->tab, nomeC1, d1, m1, a1);
        int id2 = STgetindexByNameDate(G->tab, nomeC2, d2, m2, a2);

        if (id1 != -1 && id2 != -1) {
            GRAPHinsertE(G, id1, id2);
        }
    }
    return G;
}

void GRAPHprint(Graph G) {
    if (G == NULL) {
        printf("Grafo vuoto\n");
        return;
    }

    printf("\n GRAFO GENEALOGICO \n");
    printf("Numero persone (V): %d\n", G->V);
    printf("Numero relazioni (E): %d\n\n", G->E);

    printf("LISTA PERSONE:\n");
    for (int i = 0; i < G->V; i++) {
        char *info = STsearchByIndex(G->tab, i);
        if (info != NULL) {
            printf("  ID %d: %s\n", i, info);
        }
    }

    printf("\nRELAZIONI GENITORE -> FIGLIO:\n");
    for (int i = 0; i < G->V; i++) {
        char *genitore = STsearchByIndex(G->tab, i);
        if (genitore == NULL) continue;

        int ha_figli = 0;
        for (link t = G->ladj[i]; t != G->z; t = t->next) {
            if (!ha_figli) {
                printf("  %s ->\n", genitore);
                ha_figli = 1;
            }
            char *figlio = STsearchByIndex(G->tab, t->id);
            if (figlio != NULL) {
                printf("      %s\n", figlio);
            }
        }
    }
}