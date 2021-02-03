/*
 * info607, TP2 : générateurs pseudo aléatoires simples
 * Jules Geyer
 * Kevin Traini
 * L3 groupe 3
 */

#include <stdbool.h>
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

    return *g == 1 ? mod(*x, m) : 0;
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
    if (*m == 0) { // Calcul du module
        if (nb >= 6) {
            // Calcul des y -> quantité = nbX >= 2 ? nbX -1 : 0
            int64_t y0 = random[1] - random[0];
            int64_t y1 = random[2] - random[1];
            int64_t y2 = random[3] - random[2];
            int64_t y3 = random[4] - random[3];

            // Calcul des z -> quantité = nbY >= 3 ? nbY -2 : 0
            int64_t z1 = (y2 * y0) - (y1 * y1);
            int64_t z2 = (y3 * y1) - (y2 * y2);

            // pgcd
            int64_t pgcd = gcd(z1, z2);
            pgcd = pgcd < 0 ? -pgcd : pgcd;
            *m = pgcd;
        } else
            return -1; // On n'a pas assez de nombres
    }

    if (nb >= 3) {
        int64_t invert = invert_mod(mod(random[1] - random[0], *m), *m);
        if (invert == 0) {
            *a = mod(random[2] - random[1], *m) / mod(random[1] - random[0], *m);
        } else {
            *a = mod(random[2] - random[1], *m) * invert_mod(mod(random[1] - random[0], *m), *m);
        }
        *c = random[1] - (*a * random[0]);

        *a = mod(*a, *m);
        *c = mod(*c, *m);

        int64_t x = random[0];
        for (int i = 1; i < nb; ++i) {
            x = mod(*a * x + *c, *m);

            if (x != random[i])
                return -1; // Les valeurs trouvées ne sont pas correctes
        }

        return 1;
    }

    return -1; // On n'a pas assez de nombres
}

/*
 * diagonalise une matrice booléenne de taille n x n+1
 */
int gauss(word *M, int nb_lignes) {
    for (int i = 0; i < nb_lignes; ++i) {
        if (BIT(nb_lignes - i, M[i]) != 1) {
            int j = i;
            while (j < nb_lignes && BIT(nb_lignes - i, M[j]) == 0) // Cherche la ligne ou la colonne i vaut 1
                j++;

            if (j == nb_lignes)
                return -1; // Si on n'a pas trouvé de ligne avec un 1 en colonne i

            M[i] ^= M[j];
        }

        for (int j = 0; j < nb_lignes; j++)
            if (j != i && BIT(nb_lignes - i, M[j]))
                M[j] ^= M[i];
    }

    return 1;
}

/*
 * vérifie si les taps génèrent bien la bonne suite
 * renvoie 1 si oui, -1 sinon
 */
bool verif_LSFR(int nb, int *random, word *taps, int taille) {
    int n = 0;
    for (int i = 0; i < taille; ++i) {
        n += random[i] << (taille - i - 1);
    }

    for (int i = taille; i < nb; i++) {
        int b = 0;
        for (int j = 0; j < taille; j++) {
            b ^= BIT(j, *taps) ? BIT(j, n) : 0;
        }
        n <<= 1;
        n += b;
    }

    bool isGood = true;
    int i = 0;
    while (isGood && i < nb) {
        if (BIT(nb - i - 1, n) != random[i])
            isGood = false;

        i++;
    }

    return isGood;
}

/*
 * craque un générateur linéaire "fibonacci" en cherchant les "taps" qui
 * permettent de regénérer la suite.
 * Le tableau 'random' contient les 'nb' premiers bits générés par le
 * générateur.
 * La fonction renvoie 1 si les taps permettent de regénérer la suite, et -1
 * sinon.
 */
int LFSR_crack(int nb, int *random, word *taps) {
    bool is0 = true;
    int i = 0;
    while (is0 && i < nb) {
        if (random[i] != 0)
            is0 = false;

        i++;
    }
    if (is0) return -1;

    int taille = 1;
    int isVerif = false;
    while (taille < nb && !isVerif) {
        word M[taille];
        for (int i = 0; i < taille; i++) {
            M[i] = 0;
            for (int j = 0; j < taille; j++) {
                M[i] += random[i + j] << (taille - j);
            }
            M[i] += random[i + taille];
        }

        if (gauss(M, taille) == 1) {
            *taps = 0;
            for (int i = 0; i < taille; i++) {
                *taps += (M[i] & 1) << (taille - i - 1);
            }

            isVerif = verif_LSFR(nb, random, taps, taille);
        }

        taille++;
    }

    return isVerif ? 1 : -1;
}
