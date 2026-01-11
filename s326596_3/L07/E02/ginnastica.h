#ifndef GINNASTICA_H
#define GINNASTICA_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAXS 100
#define MAXE 5
#define Nseq 3

typedef struct elemento_s *elemento_t;
typedef struct lista_s *lista_t;
typedef struct seq_s *seq_t;
typedef struct atleta_s *atleta_t;

elemento_t elemento_init(void);
void elemento_free(elemento_t elemento);

lista_t lista_init(void);
void lista_free(lista_t list);

seq_t seq_init(void);
void seq_free(seq_t seq);

atleta_t atleta_init(void);
void atleta_free(atleta_t atleta);

int leggiFile(lista_t list, FILE *fp);

elemento_t lista_get(lista_t l, int i);

int checkcondizioni(seq_t seq);
int seq_DD(seq_t seq, int DD);


int atleta_DP(atleta_t atleta, int PD);
int check_consecutivi(atleta_t atleta);
int check_avanti_indietro(atleta_t atleta);
int check_almeno_un_acrobatico(seq_t seq);

void calcola_punteggio_atleta(atleta_t atleta);

void atleta_copia(atleta_t best, atleta_t atleta);

int crea_esercizio(atleta_t atleta, lista_t lista, int DD, int DP);
void solve_ricorsiva(atleta_t atleta, atleta_t best_atleta, lista_t lista, int DD, int DP, int k_seq);

void stampa(atleta_t atleta);

#endif