#ifndef ESERCIZIO_H
#define ESERCIZIO_H

#include "elemento.h"

#define MAXE 5
#define Nseq 3

typedef struct lista_s *lista_t;
typedef struct seq_s *seq_t;
typedef struct atleta_s *atleta_t;

lista_t lista_init(void);
void lista_free(lista_t list);

atleta_t atleta_init(void);
void atleta_free(atleta_t atleta);

int leggiFile(lista_t list, FILE *fp);

void crea_esercizio(atleta_t atleta, lista_t lista, int DD, int DP);

void stampa(atleta_t atleta);

#endif