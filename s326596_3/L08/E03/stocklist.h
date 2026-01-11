#ifndef STOCKLIST_H
#define STOCKLIST_H

#include "bstquotes.h"

#define STOCK_CODE_MAX 20

typedef struct {
    char code[STOCK_CODE_MAX + 1];
    BSTQuotes quotes;
} Stock;

typedef struct stocklist_s *StockList;

StockList lista_init(void);
void lista_free(StockList sl);

Stock *lista_cerca(StockList sl, const char *code);
Stock *lista_prendi(StockList sl, const char *code);

int lista_size(StockList sl);
void lista_stampa(StockList sl, FILE *fp);

#endif
