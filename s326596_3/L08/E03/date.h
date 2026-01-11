#ifndef DATE_H
#define DATE_H

#include <stdio.h>

typedef struct {
    int y, m, d;
} Date;

typedef struct {
    int h, m;
} Time;

int data_cmp(Date a, Date b);
int data_da_str(const char *s, Date *out);
int ora_da_str(const char *s, Time *out);

void data_stampa(Date d, FILE *fp);

#endif
