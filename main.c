#include "tp2.h"

void print_usage(char* cmd)
{
    printf("  Usage:\n");
    printf("  \n");
    printf("  As a generator:\n");
    printf("    %s -g LCG <a> <c> <m>             "
           "generate random numbers using an LCG\n",
        cmd);
    printf("    %s -g LFSR <t1> ... <tn>          "
           "generate random numbers using an LFSR\n",
        cmd);

    printf("  In all cases, the option -s <seed> allows to choose the seed, in decimal (default: 1)\n");
    printf("  and the option -n <nb> allows to choose how many numbers will be generated (default: 10)\n");
    printf("  \n");

    printf("  To crack a generator:\n");
    printf("    %s -c LCG <r1> <r2> ...           "
           "attempt to recover the LCG from the sequence of integers r1,r2,...\n",
        cmd);
    printf("    %s -m <m> -c LCG <r1> <r2> ...    "
           "attempt to recover the LCG from the sequence of integers r1,r2,... with modulus m\n",
        cmd);
    printf(
        "    %s -c LFSR <r1r2...>              attempt to recover the LFSR from the sequence of bits r1,r2,...\n", cmd);
    printf("  \n");
    printf("  Internal tests:\n");
    printf("    %s -T <ARG>                       execute the 'test' function\n", cmd);
    printf("  \n");
}

#define NONE 0
#define LCG 1
#define LFSR 2
#define GALOIS 3
#define GENERATE 3
#define CRACK 4
#define TESTS 5

int VERBOSE = 0;
int main(int argc, char** argv)
{

    int opt;

    if (argc == 1) {
        print_usage(argv[0]);
        exit(-1);
    }

    int nb_randoms = 10;
    int type_prng = NONE;
    int seed = 1;
    int action = GENERATE;
    int64_t modulus = 0;
    while ((opt = getopt(argc, argv, "hn:g:s:c:m:vT")) != -1) {
        switch (opt) {
        case 'h':
            print_usage(argv[0]);
            exit(0);
        case 'T':
            type_prng = TESTS;
            break;
        case 's':
            seed = strtoul(optarg, NULL, 10);
            break;
        case 'g':
            if (0 == strcmp("LFSR", optarg)) {
                type_prng = LFSR;
                break;
            }
            if (0 == strcmp("LCG", optarg)) {
                type_prng = LCG;
                break;
            }
            /* if (0 == strcmp("galois", optarg)) { */
            /*     type_prng = GALOIS; */
            /*     break; */
            /* } */
            /* printf("allowed types for prng: LFSR, galois and LCG\n"); */
            printf("allowed types for prng: LFSR and LCG\n");
            exit(0);
            break;
        case 'c':
            if (0 == strcmp("LFSR", optarg)) {
                type_prng = LFSR;
                action = CRACK;
                break;
            }
            if (0 == strcmp("LCG", optarg)) {
                type_prng = LCG;
                action = CRACK;
                break;
            }
            printf("allowed types for prng: LFSR and LCG\n");
            exit(0);
            break;
        case 'm':
            modulus = strtoul(optarg, NULL, 10);
            break;
        case 'n':
            nb_randoms = atoi(optarg);
            break;
        case 'v':
            VERBOSE++;
            break;
        default: /* '?' */
            printf("%s -h   to get a help message\n", argv[0]);
            exit(-1);
        }
    }

    if (type_prng == TESTS) {
        argc -= optind;
        argv += optind;
        return perform_tests(argc, argv);
    } else if (type_prng == LCG && action == GENERATE) {
        if (argc - optind != 3) {
            print_usage(argv[0]);
            exit(-1);
        }
        int64_t a, c, m;
        a = strtoul(argv[optind], NULL, 10);
        c = strtoul(argv[optind + 1], NULL, 10);
        m = strtoul(argv[optind + 2], NULL, 10);
        LCG_init(a, c, m);
        LCG_seed(seed);
        LCG_show();
        for (int i = 0; i < nb_randoms; i++) {
            printf("%" PRId64, LCG_random());
        }
        printf("\n");
        return 0;
    } else if (type_prng == LFSR && action == GENERATE) {
        word taps = 0;
        for (int i = optind; i < argc; i++) {
            taps |= 1 << (atoi(argv[i]) - 1);
        }
        LFSR_init(taps);
        if (seed == 0) {
            printf("The seed of a linear feedback shift register generator "
                   "cannot be 0!\n");
            exit(1);
        }
        LFSR_seed(seed);
        LFSR_show();
        for (int i = 0; i < nb_randoms; i++) {
            printf("%d", LFSR_random_bit());
        }
        printf("\n");
        /*
        } else if (type_prng == GALOIS && action == GENERATE) {
            uint64_t taps = 0;
            for (int i=optind;i<argc;i++) {
                taps |= 1<<(atoi(argv[i])-1);
            }
            galois_init(taps);
            if (seed == 0) {
                printf("The seed of a galois linear feedback shift register
        generator cannot be 0!\n"); exit(1);
            }
            galois_seed(seed);
            galois_show();
            for (int i=0;i<nb_randoms;i++) {
                printf("%d",galois_random_bit());
            }
            printf("\n");
        */
    } else if (type_prng == LCG && action == CRACK) {
        int64_t* table = malloc((argc - optind) * sizeof(int64_t));
        for (int i = optind; i < argc; i++) {
            table[i - optind] = strtoul(argv[i], NULL, 10);
        }
        int64_t a, c;
        a = 0;
        c = 0;
        if (1 == LCG_crack(argc - optind, table, &a, &c, &modulus)) {
            printf("crack successfull:\n");
            LCG_init(a, c, modulus);
            LCG_show();
            return 0;
        } else {
            printf("crack unsuccessfull..\n");
            return 1;
        }
    } else if (type_prng == LFSR && action == CRACK) {
        char* random = argv[optind];
        int* table = malloc(sizeof(int) * strlen(random));
        word taps = 0;
        for (unsigned i = 0; i < strlen(random); i++) {
            if (random[i] == '0') {
                table[i] = 0;
            } else if (random[i] == '1') {
                table[i] = 1;
            } else {
                printf("The sequence contains more than just bits...\n");
                exit(-1);
            }
        }
        if (1 == LFSR_crack(strlen(random), table, &taps)) {
            printf("crack successfull:\n");
            LFSR_init(taps);
            LFSR_show();
            return 0;
        } else {
            printf("crack unsuccessfull..\n");
            return 1;
        }
    }

    return 0;
}
// vim: textwidth=120
