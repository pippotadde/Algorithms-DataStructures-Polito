#include <stdio.h>
#include <stdlib.h>

int majority(int *a, int N);
int* vettore(int *N);
int conta(int n, int *a, int N);

int main(int argc, char **argv) {
    int *a;
    int N;
    int risultato;
    a = vettore(&N);
    risultato = majority(a, N);
    if (risultato != -1)
        printf("Elemento maggioritario: %d\n", risultato);
    else
        printf("Nessun elemento maggioritario.\n");
    free(a);
    return 0;
}

int* vettore(int *N) {
    printf("Inserire N (numero di elementi vet): ");
    scanf("%d", N);

    int *a = malloc((*N) * sizeof(int));
    if (a == NULL) {
        printf("Errore di allocazione\n");
        exit(1);
    }

    for (int i = 0; i < *N; i++) {
        printf("Inserire %d numero: ", i + 1);
        scanf("%d", &a[i]);
    }
    return a;
}

int majority(int *a, int N) {
    if (N == 1) {
        return a[0];
    } else {
        int mid = N/2;
        int left_major = majority(a, mid);
        int right_major = majority(a + mid, N - mid);
        if (left_major == right_major) {
            return left_major;
        } else {
            if (left_major == -1 && right_major == -1) {
                return -1;
            } else if (left_major == -1) {
                return right_major;
            }else if (right_major == -1) {
                return left_major;
            }

            int left_count = conta(left_major, a, N);
            int right_count = conta(right_major, a, N);
            if (left_count > N / 2) {
                return left_major;
            } else if (right_count > N / 2) {
                return right_major;
            } else {
                return -1;
            }
        }
    }
}

int conta(int n, int *a, int N) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (n == a[i]) {
            count++;
        }
    }
    return count;
}