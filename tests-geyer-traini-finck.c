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
int perform_tests(int argc, char **argv) {
    printf("TESTS\n");
    printf("-----\n\n");

    if (argc >= 1) {
        if (strcmp(argv[0], "1") == 0) {
            printf("inverse modulo (question 1)\n");
            int64_t a, b, m;
            m = 0x7fffffff;
            a = 47268;
            b = invert_mod(a, m);
            printf("L'inverse de %" PRId64 " modulo %ld est %ld\n", a, m, b);
            printf("Vérification: %" PRId64 " * %" PRId64 " = %" PRId64
                   " (mod %" PRId64 ")\n",
                   a, b, mod(a * b, m), m);
        } else if (strcmp(argv[0], "4") == 0) {
            word M[5] = {
                    0x3e,           // 111110 en hexa
                    0x1d,           // 011101 en hexa
                    0x34,           // 110100 en hexa
                    0xe,            // 001110 en hexa
                    0x1e            // 011110 en hexa
            };
            print_M(M, 5);
            if (gauss(M, 5)) {
                print_M(M, 5);
            } else {
                printf("Erreur !");
            }
        }
    }

    return 0;
}
