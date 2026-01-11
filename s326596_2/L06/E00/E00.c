#include <stdio.h>
#include <stdlib.h>

#define fileName "att.txt"

int leggiFile(int **val);
void solveDP(int *val, int n);
void displaySol(int *opt, int *val, int n);

int main() {
    int *val;
    int n = leggiFile(&val);
    solveDP(val, n);
    free(val);
    return 0;
}

int leggiFile(int **val) {
    int n;
    FILE *fp = fopen(fileName, "r");
    if (!fp) { return 0; }
    if (fscanf(fp, "%d", &n) != 1 || n < 0) { fclose(fp); return 0; }

    (*val) = malloc((n + 1) * sizeof(int));
    if (!(*val)) { fclose(fp); return 0; }

    (*val)[0] = 0;
    for (int i = 1; i <= n; i++) {
        if (fscanf(fp, "%d", &(*val)[i]) != 1) {  // <<< & aggiunto
            fclose(fp);
            free(*val);
            *val = NULL;
            return 0;
        }
    }
    fclose(fp);
    return n;
}

void solveDP(int *val, int n) {
    int i, *opt;

    if (n <= 0) {
        printf("maximum population covered %d\n", 0);
        return;
    }

    opt = calloc((n + 1), sizeof(int));
    if (!opt) return;

    opt[0] = 0;
    opt[1] = val[1];
    for (i = 2; i <= n; i++) {
        if (opt[i-1] > opt[i-2] + val[i])
            opt[i] = opt[i-1];
        else
            opt[i] = opt[i-2] + val[i];
    }

    printf("Dynamic programming solution \n");
    printf("maximum population covered: %d\n", opt[n]);
    displaySol(opt, val, n);
    free(opt);
}

void displaySol(int *opt, int *val, int n){
    int i, *sol;
    sol = calloc((n + 1), sizeof(int));
    if (!sol) return;
    printf("antennas in cities: ");
    i = n;
    while (i >= 1) {
        if (i == 1) {
            sol[1] = (opt[1] == val[1]) ? 1 : 0;
            break;
        }
        if (opt[i] == opt[i-1]) {
            sol[i] = 0;
            i--;
        }
        else {
            sol[i] = 1;
            sol[i-1] = 0;
            i -= 2;
        }
    }
    for (i = 1; i <= n; i++) {
        if (sol[i])
            printf("%d ", val[i]);
    }
    printf("\n");
    free(sol);
}
