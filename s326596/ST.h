#ifndef ST_H
#define ST_H

typedef struct symboltable *ST;

ST    STinit(int maxN);
void  STfree(ST st);
void  STinsert(ST st, char *str, int i);
int   STgetindex(ST st, char *str);
char *STsearchByIndex(ST st, int i);
int STgetindexByNameDate(ST st, char *nome, int dd, int mm, int aa);

#endif
