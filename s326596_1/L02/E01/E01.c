#include <stdio.h>

int gcd(int a, int b);

int main(int argc, char **argv) {
    int  a, b, risultato;
    do {
        printf("Digitare numero a (diverso da 0): ");
        scanf("%d", &a);
    } while (a == 0);
    do {
        printf("Digitare numero b (diverso da 0): ");
        scanf("%d", &b);
    } while (b == 0);

    risultato = gcd(a, b);

    printf("Il massimo comune divisore tra %d e %d: %d", a, b, risultato);
    return 0;
}

int gcd(int a, int b) {
    int c;
    if (b > a) {
        c = a;
        a = b;
        b = c;
    }

    if (a == b) {
        return a;
    } else if ((a%2 == 0) && (b%2 == 0)) {
        a = a/2;
        b = b/2;
        return 2*gcd(a, b);
    } else if ((a%2 == 1) && (b%2 == 0)) {
        b = b/2;
        return gcd(a, b);
    } else if ((a%2 == 0) && (b%2 == 1)) {
        a = a/2;
        return gcd(a, b);
    } else if ((a%2 == 1) && (b%2 == 1)) {
        a = (a - b)/2;
        return gcd(a, b);
    }
}
