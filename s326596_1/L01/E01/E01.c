#include <stdio.h>
#include <ctype.h>

#define MAX_TESTO 1000
#define MAX_PATTERN 100
#define nomeFile "testo.txt"

int leggiFile(char *testo);
char *cercaRegexp(char *src, char *regexp);

int main(void) {
    char testo[MAX_TESTO];
    char pattern[MAX_PATTERN];

    if (!leggiFile(testo)) {
        return 1;
    }

    printf("Inserisci il pattern da cercare: ");
    scanf("%99s", pattern);

    char *risultato = cercaRegexp(testo, pattern);

    if (risultato != NULL) {
        printf("Trovata espressione: ");
        for (int i = 0; risultato[i] != ' ' && risultato[i] != '\0'; i++) {
            putchar(risultato[i]);
        }
        putchar('\n');
    } else {
        printf("Nessuna corrispondenza trovata.\n");
    }
}

int leggiFile(char *testo) {
    char c;
    int i = 0;

    FILE *f = fopen(nomeFile, "r");
    if (f == NULL) {
        printf("Errore nell'apertura del file.\n");
        return 0;
    }

    while ((c = fgetc(f)) != EOF && i < MAX_TESTO - 1) {
        testo[i] = c;
        i++;
    }
    testo[i] = '\0';
    fclose(f);
    return 1;
}

char *cercaRegexp(char *testo, char *parola) {
    int count = 0;

    for(int i = 0; testo[i] != '\0'; i++) {
        int k = 0;
        for(int j = 0; parola[j] != '\0'; j++) {
            int negato = 0;
            int trovato = 0;
            char t = testo[i + k];   // carattere del testo
            char p = parola[j];      // carattere della parola
            k++;

            switch (p) {
                case '.':
                    count ++;
                    break;
                case '[':
                    while (parola[j] != ']' && parola[j] != '\0') {
                        if (parola[j+1] == '^') {
                            negato = 1;
                            j++;
                        } else if (t == parola[j] && trovato != 1) {
                            trovato = 1;
                            count++;
                        }
                        j++;
                    }
                    break;
                case '\\':
                    if (isupper(t) && parola[j+1] == 'A') {
                        count++;
                        j++;
                    } else if (islower(t) && parola[j+1] == 'a') {
                        count++;
                        j++;
                    } else{
                        count = 0;
                    }
                    break;
                default:
                    if (p == t)
                        count++;
                    else
                        count = 0;
                    break;
            }

            if (count > 0) {
                if (((negato == 1) && (testo[i+j+1] == ' ')) || (negato == 0))
                    return &testo[i];
                else {
                    k = 0;
                    break;
                }
            } else if (count == 0) {
                k = 0;
                break;
            }
        }
    }
    return NULL;
}

