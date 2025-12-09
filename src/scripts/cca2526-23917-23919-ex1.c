#include <stdio.h>
#include <gmp.h>

int main() {
    mpz_t num1, num2, result;
    char str1[10000], str2[10000];

    // Initialize mpz_t variables
    mpz_init(num1);
    mpz_init(num2);
    mpz_init(result);

    // Read user input
    printf("Introduza o primeiro número inteiro: ");
    scanf("%9999s", str1);

    printf("Introduza o segundo número inteiro: ");
    scanf("%9999s", str2);

    // Convert strings to mpz_t
    mpz_set_str(num1, str1, 10);
    mpz_set_str(num2, str2, 10);

    // Multiply
    mpz_mul(result, num1, num2);

    // Show result
    printf("\nResultado da multiplicação:\n");
    mpz_out_str(stdout, 10, result);
    printf("\n");

    // Free memory
    mpz_clear(num1);
    mpz_clear(num2);
    mpz_clear(result);

    return 0;
}
