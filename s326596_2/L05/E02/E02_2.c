#include <stdio.h>
#include <stdlib.h>

#define File1 "tiles.txt"
#define File2 "board.txt"

typedef struct {
    char tubo1;
    int  val1;
    char tubo2;
    int  val2;
    int usato;
    int fissa;
} tessera;

int  leggiFile1(tessera **tess);
void leggiFile2(int *nr, int *nc, tessera *tess, tessera **board);
void rotazione(tessera *board, int pos);
void game();
int  punti(tessera *board, int nc, int nr);

int main() {
    tessera *tess;
    int N = leggiFile1(&tess);

    int nr, nc;
    tessera *board;
    leggiFile2(&nr, &nc, tess, &board);

    int pmax = 0;
    game();
    printf("Il punteggio massimo ottenibile: %d\n", pmax);

    free(tess);
    free(board);
    return 0;
}

int leggiFile1(tessera **tess) {
    int n;
    FILE *fp = fopen(File1, "r");
    if (!fp) { printf("Errore apertura\n"); exit(1); }
    fscanf(fp, "%d", &n);
    *tess = malloc(n * sizeof(tessera));
    for (int i = 0; i < n; i++) {
        fscanf(fp, " %c %d %c %d",
               &(*tess)[i].tubo1, &(*tess)[i].val1,
               &(*tess)[i].tubo2, &(*tess)[i].val2);
        (*tess)[i].usato = 0;
        (*tess)[i].fissa = 0;
    }
    fclose(fp);
    return n;
}

void leggiFile2(int *nr, int *nc, tessera *tess, tessera **board) {
    int idx, rot;
    FILE *fp = fopen(File2, "r");
    if (!fp) { printf("Errore apertura file\n"); exit(1); }
    fscanf(fp, "%d %d", nr, nc);
    *board = malloc((*nr) * (*nc) * sizeof(tessera));
    for (int i = 0; i < (*nr) * (*nc); i++) {
        fscanf(fp, "%d/%d", &idx, &rot);
        if (idx != -1) {
            (*board)[i] = tess[idx];
            (*board)[i].fissa = 1;
            tess[idx].usato = 1;
            if (rot == 1) { rotazione(*board, i); }
        }
        else {
            (*board)[i].tubo1 = ' ';
            (*board)[i].tubo2 = ' ';
            (*board)[i].val1 = 0;
            (*board)[i].val2 = 0;
            (*board)[i].fissa = 0;
        }
    }
    fclose(fp);
}

void rotazione(tessera *board, int pos) {
    char tc = board[pos].tubo1;
    int  tv = board[pos].val1;
    board[pos].tubo1 = board[pos].tubo2;
    board[pos].val1  = board[pos].val2;
    board[pos].tubo2 = tc;
    board[pos].val2  = tv;
}

void game();

int punti(tessera *board, int nc, int nr){
    int P = 0;
    for(int r = 0; r < nr; r++) {
        char tipo = board[r*nc].tubo1;
        int somma = 0;
        for(int c = 0; c < nc; c++) {
            if(board[r*nc + c].tubo1 == tipo)
                somma += board[r*nc + c].val1;
            else {
                somma = 0;
                break;
            }
        }
        P += somma;
    }
    for(int c = 0; c < nc; c++) {
        char tipo = board[c].tubo2;
        int somma = 0;
        for(int r = 0; r < nr; r++) {
            if(board[r*nc + c].tubo2 == tipo)
                somma += board[r*nc + c].val2;
            else {
                somma = 0;
                break;
            }
        }
        P += somma;
    }
    return P;
}