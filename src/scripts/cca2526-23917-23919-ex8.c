#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

// Function to show help message
void show_help(const char *name) {
	printf("Descrição:\n");
	printf(" Lista todos os números primos inferiores ao limite máximo fornecido.\n");
	printf(" O limite máximo deve ser um número inteiro positivo (em base 10).\n\n");
}

int main(int argc, char *argv[]) {
	// Verify number of arguments
	if (argc != 2) {
		printf("Erro: número de argumentos inválido!\n\n");
		show_help(argv[0]);
		return 1;
	}

	// Explicit help option
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		show_help(argv[0]);
		return 0;
	}

	// Initialize mpz_t variables
	mpz_t limit, prime;
	mpz_init(limit);
	mpz_init(prime);

	// Validate that the argument is a valid number
	if (mpz_set_str(limit, argv[1], 10) != 0) {
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

	printf("Listando números primos inferiores a ");
	mpz_out_str(stdout, 10, limit);
	printf(":\n\n");

	// Start from the first prime: 2
	mpz_set_ui(prime, 2);

	// While prime < limit → show and move to the next
	while (mpz_cmp(prime, limit) < 0) {
		mpz_out_str(stdout, 10, prime);
		printf("\n");

		// prime = next prime
		mpz_nextprime(prime, prime);
	}

	// Free memory
	mpz_clear(limit);
	mpz_clear(prime);

	return 0;
}