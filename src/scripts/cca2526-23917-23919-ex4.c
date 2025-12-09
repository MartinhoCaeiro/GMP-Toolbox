#include <stdio.h>
#include <gmp.h>
#include <time.h> 

int main() {
    mpz_t base, exponent, module, result;
    char sbase[10000], sexp[10000], smod[10000];
    int option;

    // Initialize mpz_t variables
    mpz_init(base);
    mpz_init(exponent);
    mpz_init(module);
    mpz_init(result);

    // Menu loop
    do {
        printf("\n===== Exponenciação =====\n");
        printf("1 - Exponenciação normal\n");
        printf("2 - Exponenciação modular\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &option);

        if (option == 1 || option == 2) {
            printf("\nDigite a base: ");
            scanf("%9999s", sbase);

            printf("Digite o expoente: ");
            scanf("%9999s", sexp);

            mpz_set_str(base, sbase, 10);
            mpz_set_str(exponent, sexp, 10);

            if (option == 1) {
                mpz_pow_ui(result, base, mpz_get_ui(exponent));
            }

            if (option == 2) {
                printf("Digite o módulo: ");
                scanf("%9999s", smod);
                mpz_set_str(module, smod, 10);
                mpz_powm(result, base, exponent, module);
            }

            // Display result
            printf("\nResultado:\n");
            mpz_out_str(stdout, 10, result);
            printf("\n");
        }

    } while (option != 0);

    // Free memory
    mpz_clear(base);
    mpz_clear(exponent);
    mpz_clear(module);
    mpz_clear(result);

    printf("\nPrograma encerrado.\n");
    return 0;
}
