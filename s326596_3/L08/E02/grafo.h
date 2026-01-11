#ifndef GRAFO_H
#define GRAFO_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAXC 31

typedef struct node_s *node_t;
typedef struct graph_s *graph_t;

graph_t GRAPHinit(void);
void  GRAPHfree(graph_t g);

graph_t GRAPHload(FILE *fp);

void GRAPHorder(graph_t G);

int GRAPHgetIndex2(graph_t G, char *name);
int GRAPHcheck3Matrix(graph_t G, char *n1, char *n2, char *n3);
int GRAPHcheck3List(graph_t G, char *n1, char *n2, char *n3);

node_t newNode(int indice, int flusso, node_t next);
void GRAPHconvertToList(graph_t G);

void GRAPHprintMatrix(graph_t G);
void GRAPHprintList(graph_t G);

#endif