#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>

#define MAX_PRIMES 10000
#define MAX_STR 10000
#define MAX_FILENAME 1024

int main() {
    int option;
    char limit_str[MAX_STR];
    char filename[MAX_FILENAME];
    
    mpz_t limit;
    mpz_init(limit);

    do {
        printf("\n===== Fatoração de Produtos de Primos =====\n");
        printf("1 - Continuar\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &option);
        getchar(); // limpar buffer

        if (option == 1) {
            printf("Introduza o limite máximo: ");
            scanf("%9999s", limit_str);
            getchar(); // limpar buffer

            if (mpz_set_str(limit, limit_str, 10) != 0) {
                printf("Erro: limite inválido!\n");
                continue;
            }

            printf("Introduza o nome do ficheiro de saída: ");
            scanf("%1023s", filename);
            getchar(); // limpar buffer

            FILE *fp = fopen(filename, "w");
            if (!fp) {
                printf("Erro ao abrir o ficheiro!\n");
                continue;
            }

            // Gerar primos menores que o limite
            mpz_t prime;
            mpz_init_set_ui(prime, 2);

            mpz_t primes[MAX_PRIMES];
            size_t prime_count = 0;

            while (mpz_cmp(prime, limit) < 0 && prime_count < MAX_PRIMES) {
                mpz_init_set(primes[prime_count], prime);
                prime_count++;
                mpz_nextprime(prime, prime);
            }

            printf("Primos encontrados: %zu\n", prime_count);

            // Calcular combinações dois a dois
            mpz_t product;
            mpz_init(product);

            for (size_t i = 0; i < prime_count; i++) {
                for (size_t j = i; j < prime_count; j++) { // i <= j para incluir quadrados
                    mpz_mul(product, primes[i], primes[j]);

                    // Fatoração
                    mpz_t n, factor;
                    mpz_init_set(n, product);
                    mpz_init_set_ui(factor, 2);

                    fprintf(fp, "%s = ", mpz_get_str(NULL, 10, product));

                    int first = 1;
                    while (mpz_cmp_ui(n, 1) > 0) {
                        while (mpz_divisible_p(n, factor)) {
                            if (!first) fprintf(fp, " * ");
                            fprintf(fp, "%s", mpz_get_str(NULL, 10, factor));
                            mpz_divexact(n, n, factor);
                            first = 0;
                        }
                        mpz_nextprime(factor, factor);
                    }

                    fprintf(fp, "\n");

                    mpz_clear(n);
                    mpz_clear(factor);
                }
            }

            // Liberar memória dos primos
            for (size_t i = 0; i < prime_count; i++) {
                mpz_clear(primes[i]);
            }

            mpz_clear(product);
            mpz_clear(prime);
            fclose(fp);

            printf("Fatoração gravada no ficheiro \"%s\".\n", filename);
        }

    } while (option != 0);

    printf("Programa terminado.\n");
    mpz_clear(limit);

    return 0;
}
