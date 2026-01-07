#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

int main() {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    printf("===== Geração de Chaves RSA =====\n");
    int bits;
    printf("Introduza o tamanho das chaves em bits: ");
    scanf("%d", &bits);

    // Initialize mpz_t variables
    mpz_t p, q, n, phi, e, d, gcd;
    mpz_inits(p, q, n, phi, e, d, gcd, NULL);

    // Generate two large primes p and q
    mpz_urandomb(p, state, bits / 2);
    mpz_nextprime(p, p);

    mpz_urandomb(q, state, bits / 2);
    mpz_nextprime(q, q);

    // Calculate n = p * q
    mpz_mul(n, p, q);

    // Calculate phi = (p-1)*(q-1)
    mpz_t p1, q1;
    mpz_inits(p1, q1, NULL);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // Choose e
    mpz_set_ui(e, 65537);

    // Ensure gcd(e, phi) = 1
    mpz_gcd(gcd, e, phi);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        printf("65537 não é primo relativo a phi(n), escolha outro e.\n");
        return 1;
    }

    // Calculate d = e^-1 mod phi
    if (mpz_invert(d, e, phi) == 0) {
        printf("Erro: não foi possível calcular o inverso modular.\n");
        return 1;
    }

    // Show public and private keys
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
