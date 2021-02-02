/*
 * info607, TP2 : générateurs pseudo aléatoires simples
 * Jules Geyer
 * Kevin Traini
 * L3 groupe 3
 */

#include "tp2.h"

/*
 * calcule l'inverse de a modulo m
 * (renvoie 0 si a n'est pas premier avec m ou si le résultat est incorrect)
 */
int64_t invert_mod(int64_t a, int64_t m) {
    int64_t *g = malloc(sizeof(int64_t));
    int64_t *x = malloc(sizeof(int64_t));
    int64_t *y = malloc(sizeof(int64_t));
    gcd_bezout(g, x, y, a, m);

    return *g == 1 ? *x : 0;
}

/*
 * craque un générateur congruentiel en cherchant le module, puis les nombres
 * a et c à partir de nombre générés
 *
 * Le tableau 'random' contient les 'nb' premiers nombres générés.
 * La fonction renvoie 1 si le générateur trouvé génère bien la liste donnée
 * et -1 sinon.
 *
 * Les équations sont :
 * | (ax1 + c) = x2
 * | (ax2 + c) = x3
 * V
 * | c = x2 - ax1
 * | ax2 + x2 - ax1 = x3
 *  a(x2 - x1) + x2 = x3
 *       a(x2 - x1) = x3 - x2
 *                a = (x3 - x2) / (x2 - x1)
 * V
 * | c = x2 - ax1
 * | a = (x3 - x2) / (x2 - x1)
 */
int LCG_crack(int nb, int64_t *random, int64_t *a, int64_t *c, int64_t *m) {
    if (nb >= 3) {
        *a = mod(random[2] - random[1], *m) * invert_mod(mod(random[1] - random[0], *m), *m);
        *c = random[1] - (*a * random[0]);

        *a = mod(*a, *m);
        *c = mod(*c, *m);

        return 1;
    }

    return -1;
}

/*
 * diagonalise une matrice booléenne de taille n x n+1
 */
int gauss(word *M, int nb_lignes) { return -1; }

/*
 * craque un générateur linéaire "fibonacci" en cherchant les "taps" qui
 * permettent de regénérer la suite.
 * Le tableau 'random' contient les 'nb' premiers bits générés par le
 * générateur.
 * La fonction renvoie 1 si les taps permettent de regénérer la suite, et -1
 * sinon.
 */
int LFSR_crack(int nb, int *random, word *taps) { return -1; }
