#include <stdio.h>
#include <gmp.h>

int main() {
    int option;
    char limit_str[10000];
    char filename[1024];
    FILE *fp;
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

            // Get output filename and limit
            printf("Introduza o nome do ficheiro de saída: ");
            scanf("%1023s", filename);

            fp = fopen(filename, "w");
            if (!fp) {
                printf("Erro ao abrir o ficheiro!\n");
                continue;
            }

            printf("Introduza o limite máximo: ");
            scanf("%9999s", limit_str);

            // Convert string to mpz_t
            mpz_set_str(limit, limit_str, 10);

            // Start from the first prime: 2
            mpz_set_ui(prime, 2);

            // Header in file
            fprintf(fp, "Primos inferiores a ");
            mpz_out_str(fp, 10, limit);
            fprintf(fp, ":\n");

            // While prime < limit → print and move to the next 
            while (mpz_cmp(prime, limit) < 0) {
                mpz_out_str(fp, 10, prime);
                fprintf(fp, "\n");

                // prime = next prime
                mpz_nextprime(prime, prime);
            }

            fclose(fp);
            printf("Listagem gravada no ficheiro \"%s\".\n", filename);
        }

    } while (option != 0);

    printf("Programa terminado.\n");

    // Free memory
    mpz_clear(limit);
    mpz_clear(prime);

    return 0;
}
