#include <stdio.h>
#include <gmp.h>

int main() {
    char n_str[10000], k_str[10000];
    mpz_t n, k, result;

    // Initialize mpz_t variables
    mpz_init(n);
    mpz_init(k);
    mpz_init(result);

    // Read values as strings
    printf("Introduza o número total de elementos (n): ");
    scanf("%9999s", n_str);

    printf("Introduza o número de elementos por grupo (k): ");
    scanf("%9999s", k_str);

    // Convert to mpz_t
    mpz_set_str(n, n_str, 10);
    mpz_set_str(k, k_str, 10);

    // Calculate combinations C(n,k) = n choose k
    mpz_bin_uiui(result,
                 mpz_get_ui(n),
                 mpz_get_ui(k));

    // Show result
    printf("\nNúmero de combinações C(n, k):\n");
    mpz_out_str(stdout, 10, result);
    printf("\n");

    // Free memory
    mpz_clear(n);
    mpz_clear(k);
    mpz_clear(result);

    return 0;
}
