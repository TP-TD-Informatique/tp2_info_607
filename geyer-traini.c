/*
 * info607, TP2 : générateurs pseudo aléatoires simples
 * Jules Geyer
 * Kevin Traini
 */

#include "tp2.h"

/*
 * calcule l'inverse de a modulo m
 * (renvoie 0 si a n'est pas premier avec m ou si le résultat est incorrect)
 */
int64_t invert_mod(int64_t a, int64_t m) { return 0; }

/*
 * craque un générateur congruentiel en cherchant le module, puis les nombres
 * a et c à partir de nombre générés
 *
 * Le tableau 'random' contient les 'nb' premiers nombres générés.
 * La fonction renvoie 1 si le générateur trouvé génère bien la liste donnée
 * et -1 sinon.
 */
int LCG_crack(int nb, int64_t* random, int64_t* a, int64_t* c, int64_t* m)
{
    return -1;
}

/*
 * diagonalise une matrice booléenne de taille n x n+1
 */
int gauss(word* M, int nb_lignes) { return -1; }

/*
 * craque un générateur linéaire "fibonacci" en cherchant les "taps" qui
 * permettent de regénérer la suite.
 * Le tableau 'random' contient les 'nb' premiers bits générés par le
 * générateur.
 * La fonction renvoie 1 si les taps permettent de regénérer la suite, et -1
 * sinon.
 */
int LFSR_crack(int nb, int* random, word* taps) { return -1; }
