#include <stdio.h>
#include <stdlib.h>

#define fileName "hard_test_set.txt"

#define MAX 30

void leggiFile();
int costruisci(int z, int r, int t, int s, char last);
int puoSeguire(char prec, char succ);
int Idx(char last);
int memo[MAX+1][MAX+1][MAX+1][MAX+1][5];

int main() {
    leggiFile();
    return 0;
}

void leggiFile() {
    int n, z, r, t, s, max;
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) { printf("Errore apertura file"); return; }

    fscanf(fp, "%d", &n);
    for(int i = 0; i < n; i++) {
        fscanf(fp, "%d %d %d %d", &z, &r, &t, &s);

        printf("\nTEST #%d \n", i+1);
        printf("zaffiro = %d, ", z);
        printf("rubino  = %d, ", r);
        printf("topazio = %d, ", t);
        printf("smeraldo= %d, ", s);
        printf("TOT = %d\n", z+r+t+s);

        for (int Z = 0; Z <= z; Z++)
            for (int R = 0; R <= r; R++)
                for (int T = 0; T <= t; T++)
                    for (int S = 0; S <= s; S++)
                        for (int L = 0; L < 5; L++)
                            memo[Z][R][T][S][L] = -1;

        max = costruisci(z, r, t, s, 0);   // 0 = nessuna gemma precedente

        printf("Collana massima di lunghezza = %d\n", max);
    }
    fclose(fp);
    return;
}

int Idx(char last) {
    switch (last) {
        case 'z': return 1;
        case 'r': return 2;
        case 't': return 3;
        case 's': return 4;
        default:  return 0;   // nessuna precedente
    }
}

int costruisci(int z, int r, int t, int s, char last) {
    int idx = Idx(last);

    if (memo[z][r][t][s][idx] != -1)
        return memo[z][r][t][s][idx];

    int best = 0;

    if (z > 0 && (last == 0 || puoSeguire(last, 'z'))) {
        int val = 1 + costruisci(z-1, r, t, s, 'z');
        if (val > best) best = val;
    }

    if (r > 0 && (last == 0 || puoSeguire(last, 'r'))) {
        int val = 1 + costruisci(z, r-1, t, s, 'r');
        if (val > best) best = val;
    }

    if (t > 0 && (last == 0 || puoSeguire(last, 't'))) {
        int val = 1 + costruisci(z, r, t-1, s, 't');
        if (val > best) best = val;
    }

    if (s > 0 && (last == 0 || puoSeguire(last, 's'))) {
        int val = 1 + costruisci(z, r, t, s-1, 's');
        if (val > best) best = val;
    }

    memo[z][r][t][s][idx] = best;
    return best;
}

int puoSeguire(char prec, char succ) {
    switch (prec) {
        case 'z': return (succ == 'z' || succ == 'r');
        case 'r': return (succ == 's' || succ == 't');
        case 't': return (succ == 'z' || succ == 'r');
        case 's': return (succ == 's' || succ == 't');
    }
    return 0;
}
