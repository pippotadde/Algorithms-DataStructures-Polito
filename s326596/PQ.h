#ifndef QUEUE_H
#define QUEUE_H

typedef int Item;
typedef struct pq *PQ;

PQ PQinit(int maxN);
int   PQempty(PQ q);
void  PQinsert(PQ q, Item val);
Item  PQget(PQ q);
void  PQfree(PQ q);

#endif