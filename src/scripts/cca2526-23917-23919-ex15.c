#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

int main() {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    printf("===== Geração de Chaves RSA com medição de tempo =====\n");
    int bits;
    printf("Introduza o tamanho das chaves em bits: ");
    scanf("%d", &bits);

    // Initialize mpz_t variables
    mpz_t p, q, n, phi, e, d, gcd;
    mpz_inits(p, q, n, phi, e, d, gcd, NULL);

    clock_t start, end;
    double elapsed;

    // Prime generation
    start = clock();

    mpz_urandomb(p, state, bits / 2);
    mpz_nextprime(p, p);

    mpz_urandomb(q, state, bits / 2);
    mpz_nextprime(q, q);

    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTempo para gerar primos: %.6f segundos\n", elapsed);

    // Calculate n and phi
    start = clock();

    mpz_mul(n, p, q);

    mpz_t p1, q1;
    mpz_inits(p1, q1, NULL);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo para calcular n e phi: %.6f segundos\n", elapsed);

    // Choose e and calculate d
    start = clock();

    mpz_set_ui(e, 65537);
    mpz_gcd(gcd, e, phi);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        printf("65537 não é primo relativo a phi(n), escolha outro e.\n");
        return 1;
    }

    if (mpz_invert(d, e, phi) == 0) {
        printf("Erro: não foi possível calcular o inverso modular.\n");
        return 1;
    }

    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo para calcular d: %.6f segundos\n", elapsed);

    // Show keys
    printf("\nChave pública (n, e):\n");
    gmp_printf("n = %Zd\n", n);
    gmp_printf("e = %Zd\n", e);

    printf("\nChave privada (n, d):\n");
    gmp_printf("n = %Zd\n", n);
    gmp_printf("d = %Zd\n", d);

    // Free memory
    mpz_clears(p, q, n, phi, e, d, gcd, p1, q1, NULL);
    gmp_randclear(state);

    return 0;
}
