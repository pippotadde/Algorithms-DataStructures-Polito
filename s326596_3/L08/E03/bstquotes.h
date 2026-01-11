#ifndef BSTQUOTES_H
#define BSTQUOTES_H

#include "quote.h"

typedef struct bstquotes_s *BSTQuotes;

BSTQuotes albq_init(void);
void albq_free(BSTQuotes bst);

int albq_vuoto(BSTQuotes bst);
int albq_size(BSTQuotes bst);

void albq_ins(BSTQuotes bst, Date d, double value, long long qty);
int albq_cerca(BSTQuotes bst, Date d, QuoteDay *out);

int albq_minmaxint(BSTQuotes bst, Date d1, Date d2, QuoteDay *outMin, QuoteDay *outMax);
int albq_minmaxtot(BSTQuotes bst, QuoteDay *outMin, QuoteDay *outMax);

int albq_profmin(BSTQuotes bst);
int albq_profmax(BSTQuotes bst);

int albq_bilancia(BSTQuotes bst, double S);

#endif
