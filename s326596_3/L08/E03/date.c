#include "date.h"

#include <stdio.h>

int data_cmp(Date a, Date b) {
    if (a.y != b.y) {
        if (a.y < b.y) { return -1; }
        return 1;
    }

    if (a.m != b.m) {
        if (a.m < b.m) { return -1; }
        return 1;
    }

    if (a.d != b.d) {
        if (a.d < b.d) { return -1; }
        return 1;
    }

    return 0;
}

int data_da_str(const char *s, Date *out) {
    int anno;
    int mese;
    int giorno;
    int ok;

    if (s == NULL) { return 0; }
    if (out == NULL) { return 0; }

    ok = sscanf(s, "%d/%d/%d", &anno, &mese, &giorno);
    if (ok != 3) { return 0; }

    out->y = anno;
    out->m = mese;
    out->d = giorno;

    return 1;
}

int ora_da_str(const char *s, Time *out) {
    int ora;
    int min;
    int ok;

    if (s == NULL) { return 0; }
    if (out == NULL) { return 0; }

    ok = sscanf(s, "%d:%d", &ora, &min);
    if (ok != 2) { return 0; }

    out->h = ora;
    out->m = min;

    return 1;
}

void data_stampa(Date d, FILE *fp) {
    if (fp == NULL) { fp = stdout; }
    fprintf(fp, "%04d/%02d/%02d", d.y, d.m, d.d);
}
