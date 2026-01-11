#include "quote.h"

#include <stdio.h>

QuoteDay qg_crea(Date d) {
    QuoteDay dato;

    dato.date = d;
    dato.sumVN = 0.0;
    dato.sumN = 0;
    return dato;
}

void qg_aggi(QuoteDay *q, double value, long long quantity) {
    if (q == NULL) { return; }
    if (quantity <= 0) { return; }

    q->sumVN += value * (double)quantity;
    q->sumN += quantity;
}

double qg_media(const QuoteDay *q) {
    if (q == NULL) { return 0.0; }
    if (q->sumN == 0) { return 0.0; }

    return q->sumVN / (double)q->sumN;
}

void qg_stampa(const QuoteDay *q, FILE *fp) {
    if (q == NULL) { return; }
    if (fp == NULL) { fp = stdout; }

    data_stampa(q->date, fp);
    fprintf(fp, "  Q=%.6f (sumN=%lld)", qg_media(q), q->sumN);
}
