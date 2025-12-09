#include <stdio.h>
#include <gmp.h>

int main() {
    int option;
    char limit_str[10000];
    mpz_t limit, prime;

    // Initialize mpz_t variables
    mpz_init(limit);
    mpz_init(prime);

    // Menu loop
    do {
        printf("\n===== Listagem de Números Primos =====\n");
        printf("\n1 - Continuar\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &option);

        if (option == 1) {
            printf("Introduza o limite máximo: ");
            scanf("%9999s", limit_str);

            // Convert string to mpz_t
            mpz_set_str(limit, limit_str, 10);

            // Start from the first prime: 2
            mpz_set_ui(prime, 2);
            printf("\nPrimos inferiores a ");
            mpz_out_str(stdout, 10, limit);
            printf(":\n");

            // While prime < limit → show and move to the next
            while (mpz_cmp(prime, limit) < 0) {
                mpz_out_str(stdout, 10, prime);
                printf("\n");

                // prime = next prime
                mpz_nextprime(prime, prime);
            }
        }
    } while (option != 0);

    printf("Programa terminado.\n");

    // Free memory
    mpz_clear(limit);
    mpz_clear(prime);

    return 0;
}