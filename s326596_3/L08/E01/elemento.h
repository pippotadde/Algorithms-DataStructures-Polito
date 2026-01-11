#ifndef ELEMENTO_H
#define ELEMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXS 100

typedef struct elemento_s *elemento_t;

struct elemento_s {
    char nome[MAXS];
    int tipologia;
    int dir_ingresso;
    int dir_uscita;
    int precedenza;
    int finale;
    float valore;
    int difficolta;
};

elemento_t elemento_init(void);
void elemento_free(elemento_t e);

int leggiElemento(elemento_t e, FILE *fp);

#endif