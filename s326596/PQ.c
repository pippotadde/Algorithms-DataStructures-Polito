#include <stdlib.h>
#include "PQ.h"

struct pq {Item *q; int N; int head; int tail;};

PQ PQinit(int maxN) {
    PQ q = malloc(sizeof *q);
    q->q = malloc((maxN + 1) * sizeof(Item));
    q->N = maxN + 1;
    q->head = q->N;
    q->tail = 0;
    return q;
}

int PQempty(PQ q) {
    return q->head % q->N == q->tail;
}

void PQinsert(PQ q, Item val) {
    q->q[q->tail++] = val;
    q->tail = q->tail % q->N;
}

Item PQget(PQ q) {
    q->head = q->head % q->N;
    return q->q[q->head++];
}

void PQfree(PQ q) {
    free(q->q);
    free(q);
}