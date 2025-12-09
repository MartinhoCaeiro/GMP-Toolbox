#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

// Function to show help message
void show_help(const char *name) {
    printf("Descrição:\n");
    printf("  Lista todos os números primos inferiores ao limite máximo fornecido.\n");
    printf("  O limite máximo deve ser um número inteiro positivo.\n");
    printf("Uso:\n");
    printf("  %s <limite> <ficheiro_saida>\n\n", name);
}

int main(int argc, char *argv[]) {
    // Verify number of arguments
    if (argc != 3) {
        printf("Erro: número de argumentos inválido!\n\n");
        show_help(argv[0]);
        return 1;
    }

    // Explicit help option
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        show_help(argv[0]);
        return 0;
    }

    const char *limit_str = argv[1];
    const char *filename = argv[2];

    // Initialize mpz_t variables
    mpz_t limit, prime;
    mpz_init(limit);
    mpz_init(prime);

    // Validate that the first argument is a valid number
    if (mpz_set_str(limit, limit_str, 10) != 0) {
        printf("Erro: o parâmetro fornecido não é um número válido!\n\n");
        show_help(argv[0]);
        mpz_clear(limit);
        mpz_clear(prime);
        return 1;
    }

    // Validate that limit is >= 2
    if (mpz_cmp_ui(limit, 2) < 0) {
        printf("Erro: o limite deve ser >= 2.\n");
        mpz_clear(limit);
        mpz_clear(prime);
        return 1;
    }

    // Open file for writing
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Erro: não foi possível criar ou abrir o ficheiro \"%s\"!\n", filename);
        mpz_clear(limit);
        mpz_clear(prime);
        return 1;
    }

    // Header in file
    fprintf(fp, "Primos inferiores a ");
    mpz_out_str(fp, 10, limit);
    fprintf(fp, ":\n\n");

    // Start from the first prime: 2
    mpz_set_ui(prime, 2);

    // While prime < limit → write to file
    while (mpz_cmp(prime, limit) < 0) {
        mpz_out_str(fp, 10, prime);
        fprintf(fp, "\n");

        // prime = next prime
        mpz_nextprime(prime, prime);
    }

    fclose(fp);
    printf("Listagem gravada no ficheiro \"%s\".\n", filename);

    // Free memory
    mpz_clear(limit);
    mpz_clear(prime);

    return 0;
}
