#include "stocklist.h"

#include <stdio.h>

static void pulisci_riga(void) {
    int c;

    while (1) {
        c = getchar();

        if (c == '\n') { break; }
        if (c == EOF) { break; }
    }
}

static int carica_file(StockList lista, const char *nome_file) {
    FILE *file;
    int num_titoli;
    int i_tit;

    if (lista == NULL) { return 0; }
    if (nome_file == NULL) { return 0; }

    file = fopen(nome_file, "r");
    if (file == NULL) {
        perror("fopen");
        return 0;
    }

    if (fscanf(file, "%d", &num_titoli) != 1) {
        printf("File non valido\n");
        fclose(file);
        return 0;
    }

    for (i_tit = 0; i_tit < num_titoli; i_tit++) {
        char cod[STOCK_CODE_MAX + 1];
        int n_trans;
        int ok;
        int i_trans;
        Stock *titolo;

        ok = fscanf(file, "%20s %d", cod, &n_trans);
        if (ok != 2) {
            printf("File non valido (titolo/N)\n");
            fclose(file);
            return 0;
        }

        titolo = lista_prendi(lista, cod);
        if (titolo == NULL) {
            printf("Errore inserimento titolo\n");
            fclose(file);
            return 0;
        }

        for (i_trans = 0; i_trans < n_trans; i_trans++) {
            char str_data[32];
            char str_ora[32];
            double val;
            long long qta;
            Date data;

            ok = fscanf(file, "%31s %31s %lf %lld", str_data, str_ora, &val, &qta);
            if (ok != 4) {
                printf("File non valido (transazione)\n");
                fclose(file);
                return 0;
            }

            ok = data_da_str(str_data, &data);
            if (ok == 0) {
                printf("Data non valida: %s (saltata)\n", str_data);
                continue;
            }

            albq_ins(titolo->quotes, data, val, qta);
        }
    }

    fclose(file);
    return 1;
}

static void stampa_menu(void) {
    printf("\n--- ES3 Titoli azionari ---\n");
    printf("1) Carica file transazioni\n");
    printf("2) Elenca titoli\n");
    printf("3) Seleziona titolo\n");
    printf("4) Quotazione in data\n");
    printf("5) Min/Max in intervallo date\n");
    printf("6) Min/Max su tutto il periodo\n");
    printf("7) Bilancia BST se ratio > S\n");
    printf("0) Esci\n");
    printf("> ");
}

static void chiedi_codice(char *buf) {
    printf("Codice titolo: ");
    scanf("%20s", buf);
}

static int chiedi_data(const char *testo, Date *out) {
    char s[32];
    int ok;

    if (testo == NULL) { return 0; }
    if (out == NULL) { return 0; }

    printf("%s (YYYY/MM/DD): ", testo);
    if (scanf("%31s", s) != 1) { return 0; }

    ok = data_da_str(s, out);
    if (ok == 0) { return 0; }

    return 1;
}

static void stampa_minmax(const QuoteDay *mn, const QuoteDay *mx) {
    printf("MIN: ");
    qg_stampa(mn, stdout);
    printf("\n");

    printf("MAX: ");
    qg_stampa(mx, stdout);
    printf("\n");
}

int main(void) {
    StockList lista;
    Stock *scelto;
    int scelta;

    lista = lista_init();
    scelto = NULL;

    while (1) {
        stampa_menu();

        if (scanf("%d", &scelta) != 1) { break; }
        if (scelta == 0) { break; }

        switch (scelta) {
            case 1: {
                char nome[256];
                int ok;

                printf("Nome file: ");
                scanf("%255s", nome);

                ok = carica_file(lista, nome);
                if (ok != 0) {
                    printf("Caricamento ok. Titoli: %d\n", lista_size(lista));
                } else {
                    printf("Caricamento fallito.\n");
                }
            } break;

            case 2: {
                lista_stampa(lista, stdout);
            } break;

            case 3: {
                char cod[STOCK_CODE_MAX + 1];

                chiedi_codice(cod);

                scelto = lista_cerca(lista, cod);
                if (scelto == NULL) {
                    printf("Titolo '%s' non trovato.\n", cod);
                } else {
                    printf("Selezionato: %s (giorni=%d)\n", scelto->code, albq_size(scelto->quotes));
                }
            } break;

            case 4: {
                Date data;
                QuoteDay q;
                int ok;

                if (scelto == NULL) {
                    printf("Seleziona prima un titolo (opzione 3).\n");
                    break;
                }

                ok = chiedi_data("Data", &data);
                if (ok == 0) {
                    printf("Data non valida.\n");
                    break;
                }

                ok = albq_cerca(scelto->quotes, data, &q);
                if (ok == 0) {
                    printf("Nessuna quotazione in quella data.\n");
                } else {
                    printf("Quotazione: ");
                    qg_stampa(&q, stdout);
                    printf("\n");
                }
            } break;

            case 5: {
                Date d1;
                Date d2;
                QuoteDay mn;
                QuoteDay mx;
                int ok1;
                int ok2;
                int ok3;

                if (scelto == NULL) {
                    printf("Seleziona prima un titolo (opzione 3).\n");
                    break;
                }

                ok1 = chiedi_data("Data inizio", &d1);
                if (ok1 == 0) {
                    printf("Data non valida.\n");
                    break;
                }

                ok2 = chiedi_data("Data fine", &d2);
                if (ok2 == 0) {
                    printf("Data non valida.\n");
                    break;
                }

                ok3 = albq_minmaxint(scelto->quotes, d1, d2, &mn, &mx);
                if (ok3 == 0) {
                    printf("Nessuna quotazione nell'intervallo.\n");
                } else {
                    stampa_minmax(&mn, &mx);
                }
            } break;

            case 6: {
                QuoteDay mn;
                QuoteDay mx;
                int ok;

                if (scelto == NULL) {
                    printf("Seleziona prima un titolo (opzione 3).\n");
                    break;
                }

                ok = albq_minmaxtot(scelto->quotes, &mn, &mx);
                if (ok == 0) {
                    printf("Nessuna quotazione registrata.\n");
                } else {
                    stampa_minmax(&mn, &mx);
                }
            } break;

            case 7: {
                double soglia;
                int pmin;
                int pmax;
                double ratio;
                int ok;
                if (scelto == NULL) {
                    printf("Seleziona prima un titolo (opzione 3).\n");
                    break;
                }
                printf("Soglia S (es. 2.0): ");
                ok = scanf("%lf", &soglia);
                if (ok != 1) {
                    printf("Input non valido.\n");
                    pulisci_riga();
                    break;
                }
                pmin = albq_profmin(scelto->quotes);
                pmax = albq_profmax(scelto->quotes);

                if (pmin == 0) {
                    printf("BST vuoto.\n");
                    break;
                }

                ratio = (double)pmax / (double)pmin;
                printf("profMin=%d  profMax=%d  ratio=%.3f\n", pmin, pmax, ratio);

                ok = albq_bilancia(scelto->quotes, soglia);
                if (ok != 0) {
                    printf("Bilanciato. Ora profMin=%d profMax=%d\n",
                           albq_profmin(scelto->quotes),
                           albq_profmax(scelto->quotes));
                } else {
                    printf("Nessun bilanciamento (ratio <= S).\n");
                }
            } break;
            default: {
                printf("Comando non riconosciuto.\n");
            } break;
        }
    }
    lista_free(lista);
    return 0;
}
