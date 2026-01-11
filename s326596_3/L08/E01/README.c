/*
 *                              LAB 7 E01
 *
 * Le soluzioni proposte sono greedy e non garantiscono l’ottimo globale
 * Le mie idee per risoluzione:
 * La soluzione finale addotata è la 1.
 *
 * 1) Funzione di appetibilità: A(e) = (val(e)/diff(e)) + bonus(e)
 *
 * Logica:
 * - costruire le 3 diagonali in modo incrementale, facendo ciclicamente un tentativo
 *   di aggiunta su ogni diagonale (round-robin) finché non è più possibile inserire
 *   elementi
 * - ad ogni passo, per la diagonale k si sceglie l’elemento ammissibile con
 *    A(e) massima rispettando vincoli (ingresso/uscita, precedenza, DD e DP)
 * - per garantire i vincoli globali, da DP si “riserva” a inizio algoritmo un
 *   budget minimo dove si sottraggono i minimi degli elementi e poi si restituiscono quando il vincolo è soddisfatto
 * - bonus(e) favorisce elementi che coprono vincoli mancanti:
 *   avanti/indietro, bonus 1.5 nella diag 3, coppia consecutiva
 *
 *
 * 2) Funzione apetibilità: A(e) = val(e) / diff(e)
 *
 * Logica:
 * - ordinare la lista degli elementi in ordine decrescente rispetto alla funzione
 *   di appetibilità;
 * - aggiungere alle diagonali il maggior numero possibile di elementi, rispettando
 *   i vincoli di difficoltà (DD e DP) e i vincoli di successione;
 * - costruire le diagonali una alla volta;
 * - partire dalla diagonale 3 per cercare di massimizzare il punteggio sfruttando
 *   il moltiplicatore 1.5;
 * - nella diagonale 2 forzare la presenza di due elementi consecutivi se il vincolo
 *   non è stato ancora soddisfatto;
 * - nelle altre diagonali controllare la presenza di elementi avanti e indietro
 *   e, se necessario, vincolare la scelta per coprire tali requisiti.
 *
 *
 * 3) Funzione apetibilità: A(e) = (val(e) + bonus(e)) / diff(e)
 *
 *  Dove il bonus dipende da:
 * - inserimento di elementi acrobatici se la diagonale non ne contiene
 * - copertura di acrobatici avanti e indietro
 * - creazione di coppie di acrobatici consecutivi
 * - nell’ultima diagonale, chiusura con diff >= 8 per ottenere il bonus 1.5
 *
 * Logica:
 * - l’idea è estendere la funzione di appetibilità base introducendo un bonus
 *   che tenga conto dei vincoli globali del problema;
 * - il bonus viene assegnato agli elementi che aiutano a soddisfare vincoli
 *   non ancora coperti nel programma o nella diagonale corrente;
 * - in questo modo non sceglie solo gli elementi più convenienti
 *   in termini di valore/difficoltà, ma privilegia anche quelli utili a
 *   rendere la soluzione valida.
 *-  la funzione di appetibilità varia dinamicamente in base allo stato
 *   corrente della soluzione (vincoli già soddisfatti o mancanti);
 */