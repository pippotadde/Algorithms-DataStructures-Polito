#include <stdio.h>
#include <stdlib.h>

#define fileName "att.txt"

typedef struct {
    int si;
    int fi;
    int di;
} att;

int leggiFile(att **V);
void attSel(int N, att *V);
int cmpAtt(const void *a, const void *b);

int main() {
    att *V;
    int N = leggiFile(&V);
    if (N <= 0) {return 1;}
    attSel(N, V);
    free(V);
    return 0;
}

int leggiFile(att **V) {
    int n;
    FILE *fp = fopen(fileName, "r");
    if (!fp) {printf("Errore apertura file\n"); return 0;}
    fscanf(fp, "%d", &n);
    *V = malloc(n * sizeof(att));
    if (!*V) {printf("Errore malloc\n"); fclose(fp); return 0;}
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d %d", &(*V)[i].si, &(*V)[i].fi);
        (*V)[i].di = (*V)[i].fi - (*V)[i].si;
    }
    fclose(fp);
    return n;
}

int cmpAtt(const void *a, const void *b) {
    att *x = (att *)a;
    att *y = (att *)b;
    if (x->fi != y->fi)
        return x->fi - y->fi;
    return x->si - y->si;
}

void attSel(int N, att *V) {
    int i, j;
    int *dp;
    int *prev;
    int best = 0;
    int last = -1;

    qsort(V, N, sizeof(att), cmpAtt);

    dp = malloc(N * sizeof(int));
    prev = malloc(N * sizeof(int));
    if (!dp || !prev) {
        printf("Errore malloc\n");
        free(dp);
        free(prev);
        return;
    }

    for (i = 0; i < N; i++) {
        dp[i] = V[i].di;
        prev[i] = -1;
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < i; j++) {
            if (V[j].fi <= V[i].si) {
                if (dp[j] + V[i].di > dp[i]) {
                    dp[i] = dp[j] + V[i].di;
                    prev[i] = j;
                }
            }
        }
    }

    for (i = 0; i < N; i++) {
        if (dp[i] > best) {
            best = dp[i];
            last = i;
        }
    }

    int *lista = malloc(N * sizeof(int));
    int k = 0;
    i = last;
    while (i != -1) {
        lista[k++] = i;
        i = prev[i];
    }

    printf("Dynamic programming solution\n");
    printf("Durata totale massima delle attivita compatibili: %d\n", best);
    printf("Attivita selezionate (si, fi):\n");
    for (i = k - 1; i >= 0; i--) {
        int idx = lista[i];
        printf("(%d, %d) ", V[idx].si, V[idx].fi);
    }
    printf("\n");

    free(lista);
    free(dp);
    free(prev);
}
