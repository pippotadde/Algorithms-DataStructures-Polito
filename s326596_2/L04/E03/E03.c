#include <stdio.h>
#include <stdlib.h>

#define nomeFile "hard_test_set.txt"

void leggiFile();
int costruisci(int z, int r, int t, int s, int pos, char *collana, int tot);
int puoSeguire(char prec, char succ);

int main() {
    leggiFile();
    return 0;
}

void leggiFile() {
    int n;
    int z, r, t, s;
    int l;
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) { printf("Errore apertura file"); return; }
    fscanf(fp, "%d", &n);
    for(int i = 0; i < n; i++) {
        fscanf(fp, "%d %d %d %d", &z, &r, &t, &s);
        printf("\n");
        printf("\nTEST #%d \n", i+1);
        printf("zaffiro = %d, ", z);
        printf("rubino = %d, ", r);
        printf("topazio = %d, ", t);
        printf("smeraldo = %d, ", s);
        int tot =  z+r+t+s;
        printf("TOT = %d", tot);
        printf("\n");
        char *collana = malloc((z+r+t+s+1) * sizeof(char));
        l = costruisci(z, r, t, s, 0, collana, tot);
        printf("Collana massima di lunghezza = %d", l);
    }
    return;
}

int costruisci(int z, int r, int t, int s, int pos, char *collana, int best) {
    int max = best;

    if (pos > max)
        max = pos;
    // PROVA ZAFFIRO
    if (z > 0 && (pos == 0 || puoSeguire(collana[pos-1], 'z'))) {
        collana[pos] = 'z';
        int val = costruisci(z-1, r, t, s, pos+1, collana, max);
        if (val > max) {
            max = val;
        }
    }
    // PROVA RUBINO
    if (r > 0 && (pos == 0 || puoSeguire(collana[pos-1], 'r'))) {
        collana[pos] = 'r';
        int val = costruisci(z, r-1, t, s, pos+1, collana, max);
        if (val > max) {
            max = val;
        }
    }
    // PROVA TOPAZIO
    if (t > 0 && (pos == 0 || puoSeguire(collana[pos-1], 't'))) {
        collana[pos] = 't';
        int val = costruisci(z, r, t-1, s, pos+1, collana, max);
        if (val > max) {
            max = val;
        }
    }
    // PROVA SMERALDO
    if (s > 0 && (pos == 0 || puoSeguire(collana[pos-1], 's'))) {
        collana[pos] = 's';
        int val = costruisci(z, r, t, s-1, pos+1, collana, max);
        if (val > max) {
            max = val;
        }
    }
    return max;
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