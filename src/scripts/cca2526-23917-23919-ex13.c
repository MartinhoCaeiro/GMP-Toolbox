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
    
    // Initialize mpz_t variables
    mpz_t limit;
    mpz_init(limit);

    do {
        printf("\n===== Produtos de Primos =====\n");
        printf("1 - Continuar\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &option);
        getchar();

        // Process option
        if (option == 1) {
            printf("Introduza o limite máximo: ");
            scanf("%9999s", limit_str);
            getchar();

            // Convert limit string to mpz_t
            if (mpz_set_str(limit, limit_str, 10) != 0) {
                printf("Erro: limite inválido!\n");
                continue;
            }

            // Get output filename
            printf("Introduza o nome do ficheiro de saída: ");
            scanf("%1023s", filename);
            getchar();

            FILE *fp = fopen(filename, "w");
            if (!fp) {
                printf("Erro ao abrir o ficheiro!\n");
                continue;
            }

            // Generate primes up to limit
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

            // Calculate products of primes and write to file
            mpz_t product;
            mpz_init(product);

            for (size_t i = 0; i < prime_count; i++) {
                for (size_t j = i; j < prime_count; j++) {
                    mpz_mul(product, primes[i], primes[j]);
                    fprintf(fp, "%s %s %s\n",
                        mpz_get_str(NULL, 10, product),
                        mpz_get_str(NULL, 10, primes[i]),
                        mpz_get_str(NULL, 10, primes[j]));
                }
            }

            // Free memory
            for (size_t i = 0; i < prime_count; i++) {
                mpz_clear(primes[i]);
            }
            mpz_clear(product);
            mpz_clear(prime);
            
            fclose(fp);
            printf("Ficheiro \"%s\" gerado com sucesso.\n", filename);
        }

    } while (option != 0);

    printf("Programa terminado.\n");
    mpz_clear(limit);

    return 0;
}
