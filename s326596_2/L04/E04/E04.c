#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nomeFile "test_set.txt"

void leggiFile();
int costruisci(int nz, int vz, int nr, int vr, int nt, int vt, int ns, int vs, int pos, char *collana, int rip, int prezzo, int *best, char *bestcollana, int usedZ, int usedS);
int puoSeguire(char prec, char succ);
int maxRipe(int rip, int pos, char gemma, char *collana);

int main() {
    leggiFile();
    return 0;
}

void leggiFile() {
    int n, rip;
    int nz, nr, nt, ns;
    int vz, vr, vt, vs;
    FILE *fp = fopen(nomeFile, "r");
    if (fp == NULL) { printf("Errore apertura file"); return; }
    fscanf(fp, "%d", &n);
    for(int i = 0; i < n; i++) {
        fscanf(fp, "%d %d %d %d %d %d %d %d %d", &nz, &nr, &nt, &ns, &vz, &vr, &vt, &vs, &rip);
        printf("\n");
        printf("\n TEST #%d \n", i+1);
        printf("zaffiro = %d [%d], ", nz, vz);
        printf("rubino = %d [%d], ", nr, vr);
        printf("topazio = %d [%d], ", nt, vt);
        printf("smeraldo = %d [%d], ", ns, vs);
        int tot =  nz+nr+nt+ns;
        printf("TOT = %d {max_rip = %d}", tot, rip);
        printf("\n");
        char *collana = malloc((nz+nr+nt+ns+1) * sizeof(char));
        char *bestcollana = malloc(tot+1);
        int best = 0;
        costruisci(nz, vz, nr, vr, nt, vt, ns, vs,0, collana, rip,0, &best, bestcollana,0, 0);
        printf("Soluzione ottima di valore %d ", best);
        int l = strlen(bestcollana);
        printf("usando %d gemma/e\n", l);
        printf("Composizione collana: %s", bestcollana);
    }
    return;
}

int costruisci(int nz, int vz, int nr, int vr, int nt, int vt, int ns, int vs, int pos, char *collana, int rip, int prezzo, int *best, char *bestcollana, int usedZ, int usedS) {
    if (prezzo > *best) {
        *best = prezzo;
        memcpy(bestcollana, collana, pos);
        bestcollana[pos] = '\0';
    }
    if ((pos==0) || ((nz > 0) && ((usedZ + 1)<=usedS) && (maxRipe(rip, pos, 'z', collana)) && (puoSeguire(collana[pos-1], 'z')))) {
        collana[pos] = 'z';
        costruisci(nz-1, vz, nr, vr, nt, vt, ns, vs, pos+1, collana, rip, prezzo+vr, best, bestcollana, usedZ+1, usedS);
    }
    if ((pos==0) || ((nr > 0) && (maxRipe(rip, pos, 'r', collana)) && (puoSeguire(collana[pos-1], 'r')))) {
        collana[pos] = 'r';
        costruisci(nz, vz, nr-1, vr, nt, vt, ns, vs, pos+1, collana, rip, prezzo+vr, best, bestcollana, usedZ, usedS);
    }
    if ((pos==0) || ((nt > 0) && (maxRipe(rip, pos, 't', collana)) && (puoSeguire(collana[pos-1], 't')))) {
        collana[pos] = 't';
        costruisci(nz, vz, nr, vr, nt-1, vt, ns, vs, pos+1, collana, rip, prezzo+vt, best, bestcollana, usedZ, usedS);
    }
    if ((pos==0) || ((ns > 0) && (maxRipe(rip, pos, 's', collana)) && (puoSeguire(collana[pos-1], 's')))) {
        collana[pos] = 's';
        costruisci(nz, vz, nr, vr, nt, vt, ns-1, vs, pos+1, collana, rip, prezzo+vs, best, bestcollana, usedZ, usedS+1);
    }
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

int maxRipe(int rip, int pos, char gemma, char *collana) {
    int count = 0;
    for (int i = pos - 1; i >= 0 && collana[i] == gemma; i--) {
        count++;
        if (count >= rip) {
            return 0;
        }
    }
    return 1;
}
