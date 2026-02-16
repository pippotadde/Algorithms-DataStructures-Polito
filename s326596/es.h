#ifndef ES_H
#define ES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ST.h"
#include "PQ.h"

typedef struct date *Date;
struct date {int dd; int mm; int aa;};
typedef struct key *link;
typedef struct graph *Graph;

int KEYcmp(link k1, link k2);
int relationshipK(Graph G, char *nome1, Date d1, char *nome2, Date d2, int k);
void largestLineageK(Graph G, int k);


Graph graph_init(int n);
void graph_free(Graph G);
link key_init(void);
void key_free(link K);
Date date_init(void);
void date_free(Date D);

//funzioni aggiunte
Date readDate(void);
link readKey(void);
void GRAPHinsertE(Graph G, int id1, int id2);
Graph GRAPHload(FILE *fp);
void GRAPHprint(Graph G);

#endif