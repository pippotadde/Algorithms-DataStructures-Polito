#include "bstquotes.h"

#include <stdlib.h>

typedef struct nodo_s {
    QuoteDay q;
    struct nodo_s *sx;
    struct nodo_s *dx;
} Nodo;

struct bstquotes_s {
    Nodo *rad;
    int n;
};

static Nodo *nodo_nuovo(Date d) {
    Nodo *x;

    x = (Nodo *)malloc(sizeof(*x));
    if (x == NULL) { exit(1); }

    x->q = qg_crea(d);
    x->sx = NULL;
    x->dx = NULL;

    return x;
}

static void nodo_free(Nodo *h) {
    if (h == NULL) { return; }
    nodo_free(h->sx);
    nodo_free(h->dx);
    free(h);
}

BSTQuotes albq_init(void) {
    BSTQuotes bst;

    bst = (BSTQuotes)malloc(sizeof(*bst));
    if (bst == NULL) { exit(1); }

    bst->rad = NULL;
    bst->n = 0;

    return bst;
}

void albq_free(BSTQuotes bst) {
    if (bst == NULL) { return; }
    nodo_free(bst->rad);
    free(bst);
}

int albq_vuoto(BSTQuotes bst) {
    if (bst == NULL) { return 1; }
    if (bst->rad == NULL) { return 1; }
    return 0;
}

int albq_size(BSTQuotes bst) {
    if (bst == NULL) { return 0; }
    return bst->n;
}

static Nodo *ins_o_agg(Nodo *h, Date d, double value, long long qty, int *ins) {
    int cmp;

    if (h == NULL) {
        Nodo *x;

        x = nodo_nuovo(d);
        qg_aggi(&x->q, value, qty);

        if (ins != NULL) { *ins = 1; }
        return x;
    }

    cmp = data_cmp(d, h->q.date);

    if (cmp < 0) {
        h->sx = ins_o_agg(h->sx, d, value, qty, ins);
    } else {
        if (cmp > 0) {
            h->dx = ins_o_agg(h->dx, d, value, qty, ins);
        } else {
            qg_aggi(&h->q, value, qty);
        }
    }

    return h;
}

void albq_ins(BSTQuotes bst, Date d, double value, long long qty) {
    int ins;

    if (bst == NULL) { return; }

    ins = 0;
    bst->rad = ins_o_agg(bst->rad, d, value, qty, &ins);

    if (ins != 0) { bst->n = bst->n + 1; }
}

static Nodo *cerca_nodo(Nodo *h, Date d) {
    while (h != NULL) {
        int cmp;

        cmp = data_cmp(d, h->q.date);

        if (cmp == 0) { return h; }

        if (cmp < 0) {
            h = h->sx;
        } else {
            h = h->dx;
        }
    }

    return NULL;
}

int albq_cerca(BSTQuotes bst, Date d, QuoteDay *out) {
    Nodo *x;

    if (bst == NULL) { return 0; }

    x = cerca_nodo(bst->rad, d);
    if (x == NULL) { return 0; }

    if (out != NULL) { *out = x->q; }
    return 1;
}

static void minmax_aggi(const QuoteDay *q, int *pres, QuoteDay *mn, QuoteDay *mx) {
    double v;
    double v1;
    double v2;

    if (q == NULL) { return; }
    if (q->sumN == 0) { return; }

    v = qg_media(q);

    if (*pres == 0) {
        *mn = *q;
        *mx = *q;
        *pres = 1;
        return;
    }

    v1 = qg_media(mn);
    v2 = qg_media(mx);

    if (v < v1) { *mn = *q; }
    if (v > v2) { *mx = *q; }
}

static void minmax_tot_rec(Nodo *h, int *pres, QuoteDay *mn, QuoteDay *mx) {
    if (h == NULL) { return; }

    minmax_tot_rec(h->sx, pres, mn, mx);
    minmax_aggi(&h->q, pres, mn, mx);
    minmax_tot_rec(h->dx, pres, mn, mx);
}

int albq_minmaxtot(BSTQuotes bst, QuoteDay *outMin, QuoteDay *outMax) {
    int pres;
    QuoteDay mn;
    QuoteDay mx;

    if (bst == NULL) { return 0; }
    if (bst->rad == NULL) { return 0; }

    pres = 0;
    minmax_tot_rec(bst->rad, &pres, &mn, &mx);

    if (pres == 0) { return 0; }

    if (outMin != NULL) { *outMin = mn; }
    if (outMax != NULL) { *outMax = mx; }

    return 1;
}

static void minmax_int_rec(Nodo *h, Date d1, Date d2, int *pres, QuoteDay *mn, QuoteDay *mx) {
    int c1;
    int c2;

    if (h == NULL) { return; }

    c1 = data_cmp(h->q.date, d1);
    if (c1 < 0) {
        minmax_int_rec(h->dx, d1, d2, pres, mn, mx);
        return;
    }

    c2 = data_cmp(h->q.date, d2);
    if (c2 > 0) {
        minmax_int_rec(h->sx, d1, d2, pres, mn, mx);
        return;
    }

    minmax_int_rec(h->sx, d1, d2, pres, mn, mx);
    minmax_aggi(&h->q, pres, mn, mx);
    minmax_int_rec(h->dx, d1, d2, pres, mn, mx);
}

int albq_minmaxint(BSTQuotes bst, Date d1, Date d2, QuoteDay *outMin, QuoteDay *outMax) {
    int pres;
    QuoteDay mn;
    QuoteDay mx;

    if (bst == NULL) { return 0; }
    if (bst->rad == NULL) { return 0; }

    if (data_cmp(d1, d2) > 0) {
        Date tmp;

        tmp = d1;
        d1 = d2;
        d2 = tmp;
    }

    pres = 0;
    minmax_int_rec(bst->rad, d1, d2, &pres, &mn, &mx);

    if (pres == 0) { return 0; }

    if (outMin != NULL) { *outMin = mn; }
    if (outMax != NULL) { *outMax = mx; }

    return 1;
}

static int prof_max_rec(Nodo *h) {
    int a;
    int b;

    if (h == NULL) { return 0; }

    a = prof_max_rec(h->sx);
    b = prof_max_rec(h->dx);

    if (a > b) { return 1 + a; }
    return 1 + b;
}

static int prof_min_rec(Nodo *h) {
    int a;
    int b;

    if (h == NULL) { return 0; }
    if (h->sx == NULL && h->dx == NULL) { return 1; }

    if (h->sx == NULL) { return 1 + prof_min_rec(h->dx); }
    if (h->dx == NULL) { return 1 + prof_min_rec(h->sx); }

    a = prof_min_rec(h->sx);
    b = prof_min_rec(h->dx);

    if (a < b) { return 1 + a; }
    return 1 + b;
}

int albq_profmin(BSTQuotes bst) {
    if (bst == NULL) { return 0; }
    return prof_min_rec(bst->rad);
}

int albq_profmax(BSTQuotes bst) {
    if (bst == NULL) { return 0; }
    return prof_max_rec(bst->rad);
}

static void inorder_met(Nodo *h, QuoteDay *vett, int *i) {
    if (h == NULL) { return; }

    inorder_met(h->sx, vett, i);

    vett[*i] = h->q;
    *i = *i + 1;

    inorder_met(h->dx, vett, i);
}

static Nodo *costruisci(QuoteDay *vett, int lo, int hi) {
    int m;
    Nodo *h;

    if (lo > hi) { return NULL; }

    m = lo + (hi - lo) / 2;

    h = (Nodo *)malloc(sizeof(*h));
    if (h == NULL) { exit(1); }

    h->q = vett[m];
    h->sx = costruisci(vett, lo, m - 1);
    h->dx = costruisci(vett, m + 1, hi);

    return h;
}

static void sostituisci(BSTQuotes bst, Nodo *nuova) {
    nodo_free(bst->rad);
    bst->rad = nuova;
}

int albq_bilancia(BSTQuotes bst, double S) {
    int pmin;
    int pmax;
    double ratio;
    int n;
    QuoteDay *vett;
    int i;
    Nodo *nuova;

    if (bst == NULL) { return 0; }
    if (bst->rad == NULL) { return 0; }

    pmin = albq_profmin(bst);
    pmax = albq_profmax(bst);

    if (pmin == 0) { return 0; }

    ratio = (double)pmax / (double)pmin;
    if (ratio <= S) { return 0; }

    n = bst->n;

    vett = (QuoteDay *)malloc((size_t)n * sizeof(*vett));
    if (vett == NULL) { exit(1); }

    i = 0;
    inorder_met(bst->rad, vett, &i);

    nuova = costruisci(vett, 0, n - 1);
    sostituisci(bst, nuova);

    free(vett);
    return 1;
}
