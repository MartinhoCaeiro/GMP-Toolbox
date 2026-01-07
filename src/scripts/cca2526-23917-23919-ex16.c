#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

// Function to generate a random mpz_t number with the given number of bits
// using /dev/urandom as a cryptographically secure source
void random_mpz(mpz_t rop, int bits) {
    int bytes = (bits + 7) / 8;
    unsigned char *buf = malloc(bytes);
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir /dev/urandom");
        exit(1);
    }
    if (read(fd, buf, bytes) != bytes) {
        perror("Erro ao ler /dev/urandom");
        close(fd);
        exit(1);
    }
    close(fd);

    // Import bytes into mpz_t
    mpz_import(rop, bytes, 1, 1, 0, 0, buf);
    mpz_setbit(rop, bits - 1);
    free(buf);
}

int main() {
    printf("===== Geração de Chaves RSA com medição de tempo e armazenamento =====\n");
    int bits;
    printf("Introduza o tamanho das chaves em bits: ");
    scanf("%d", &bits);

    // Initialize mpz_t variables
    mpz_t p, q, n, phi, e, d, gcd, p1, q1;
    mpz_inits(p, q, n, phi, e, d, gcd, p1, q1, NULL);

    clock_t start, end;
    double elapsed;

    // Prime generation
    start = clock();

    do {
        random_mpz(p, bits/2);
        mpz_nextprime(p, p);

        random_mpz(q, bits/2);
        mpz_nextprime(q, q);
    } while (mpz_cmp(p, q) == 0);

    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo para gerar primos: %.6f segundos\n", elapsed);

    // Calculate n and phi
    start = clock();

    mpz_mul(n, p, q);
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
        fprintf(stderr, "65537 não é primo relativo a phi(n)\n");
        exit(1);
    }

    if (mpz_invert(d, e, phi) == 0) {
        fprintf(stderr, "Erro ao calcular o inverso modular d\n");
        exit(1);
    }

    end = clock();
    elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tempo para calcular d: %.6f segundos\n", elapsed);

    // Save keys to files
    FILE *pub = fopen("public.key", "w");
    FILE *priv = fopen("private.key", "w");

    if (!pub || !priv) {
        perror("Erro ao criar ficheiros de chave");
        exit(1);
    }

    gmp_fprintf(pub, "%Zd\n%Zd\n", n, e);
    gmp_fprintf(priv, "%Zd\n%Zd\n", n, d);

    fclose(pub);
    fclose(priv);

    printf("Chaves geradas e gravadas em 'public.key' e 'private.key'.\n");

    // Free memory
    mpz_clears(p, q, n, phi, e, d, gcd, p1, q1, NULL);

    return 0;
}
