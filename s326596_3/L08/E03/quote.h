#ifndef QUOTE_H
#define QUOTE_H

#include "date.h"

#include <stdio.h>

typedef struct {
    Date date;
    double sumVN;
    long long sumN;
} QuoteDay;

QuoteDay qg_crea(Date d);
void qg_aggi(QuoteDay *q, double value, long long quantity);
double qg_media(const QuoteDay *q);

void qg_stampa(const QuoteDay *q, FILE *fp);

#endif
