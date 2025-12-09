#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main() {
    int option;
    unsigned int num;

    // Menu loop
    do {
        printf("\n===== Comparador de Tempos de Exponenciação =====\n");
        printf("\n1 - Continuar\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &option);

        if (option == 1) {

            printf("Introduza o valor de n: ");
            scanf("%u", &num);

            // Normal exponential
            clock_t start = clock();

            unsigned long long r = 1;
            for (unsigned int i = 0; i < num; i++)
                r *= 2;

            clock_t end = clock();
            double normal_time = (double)(end - start) / CLOCKS_PER_SEC;

            printf("\nTempo usando exponenciação normal: %.6f s\n", normal_time);


            // GMP exponential
            mpz_t result;
            mpz_init(result);

            start = clock();
            mpz_ui_pow_ui(result, 2, num);
            end = clock();

            double gmp_time = (double)(end - start) / CLOCKS_PER_SEC;

            printf("Tempo usando exponenciação GMP: %.6f s\n", gmp_time);

            mpz_clear(result);
        }

    } while (option != 0);

    printf("Programa encerrado.\n");
    return 0;
}
