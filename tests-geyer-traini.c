/*
 * info607, TP2 : générateurs pseudo aléatoires simples
 * Jules Geyer
 * Kevin Traini
 * tests
 */

#include "tp2.h"

/*
 * fonction de test, exécutée par l'argument '-T' de la ligne de commande
 */
int perform_tests(int argc, char** argv)
{
    printf("TESTS\n");
    printf("-----\n\n");

    printf("inverse modulo (question 1)\n");
    int64_t a, b, m;
    m = 0x7fffffff;
    a = 47268;
    b = invert_mod(a, m);
    printf("L'inverse de %" PRId64 " modulo %ld est %ld\n", a, m, b);
    printf("Vérification: %" PRId64 " * %" PRId64 " = %" PRId64
           " (mod %" PRId64 ")\n",
        a, b, mod(a * b, m), m);

    return 0;
}
