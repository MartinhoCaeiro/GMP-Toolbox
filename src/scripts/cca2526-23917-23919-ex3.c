#include <stdio.h>
#include <gmp.h>

int main() {
    mpz_t num;
    char str[10000];
    int result;

    // Initialize mpz_t variables
    mpz_init(num);

    // Read user input
    printf("Introduza um número inteiro: ");
    scanf("%9999s", str);

    // Convert strings to mpz_t
    mpz_set_str(num, str, 10);

    // Check primality (25 iterations -> high accuracy)
    result = mpz_probab_prime_p(num, 25);

    // Interpret result
    if (result == 0) {
        printf("O número NÃO é primo.\n");
    } else if (result == 1) {
        printf("O número é PROVAVELMENTE primo.\n");
    } else if (result == 2) {
        printf("O número é DEFINITIVAMENTE primo.\n");
    }

    // Free memory
    mpz_clear(num);

    return 0;
}
