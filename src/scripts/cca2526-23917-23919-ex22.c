#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME 256

// -----------------------------------------------------------
//  GERAR PRIMO ENTRE 2^(k-1) E 2^k - 1
// -----------------------------------------------------------
void gerar_primo_k_bits(mpz_t primo, gmp_randstate_t estado, unsigned int k) {
    mpz_t min, max, range;
    mpz_inits(min, max, range, NULL);

    // min = 2^(k-1)
    mpz_ui_pow_ui(min, 2, k - 1);

    // max = 2^k - 1
    mpz_ui_pow_ui(max, 2, k);
    mpz_sub_ui(max, max, 1);

    // range = max - min
    mpz_sub(range, max, min);

    do {
        mpz_urandomm(primo, estado, range);
        mpz_add(primo, primo, min);
        mpz_nextprime(primo, primo);
    } while (mpz_cmp(primo, max) > 0);

    mpz_clears(min, max, range, NULL);
}

// -----------------------------------------------------------
//  GERAR CHAVES RSA
// -----------------------------------------------------------
void gerar_chave_RSA(unsigned int k) {
    gmp_randstate_t estado;
    gmp_randinit_default(estado);
    gmp_randseed_ui(estado, time(NULL));

    mpz_t p, q, n, phi, e, d, p1, q1;
    mpz_inits(p, q, n, phi, e, d, p1, q1, NULL);

    printf("A gerar primos %u-bit...\n", k);

    gerar_primo_k_bits(p, estado, k);
    gerar_primo_k_bits(q, estado, k);

    // n = p * q
    mpz_mul(n, p, q);

    // φ = (p-1)(q-1)
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(phi, p1, q1);

    // e = 65537 (valor recomendado)
    mpz_set_ui(e, 65537);

    // d = e^-1 mod φ
    mpz_invert(d, e, phi);

    // Guardar chave pública
    FILE* pub = fopen("public.myasc", "w");
    gmp_fprintf(pub, "%Zd,%Zd", e, n);
    fclose(pub);

    // Guardar chave privada
    FILE* priv = fopen("secret.myasc", "w");
    fprintf(priv, "Chave Privada:\n");
    gmp_fprintf(priv, "%Zd,%Zd", d, n);
    fclose(priv);

    printf("Chaves RSA %u-bit geradas com sucesso!\n", k);

    mpz_clears(p, q, n, phi, e, d, p1, q1, NULL);
    gmp_randclear(estado);
}

// -----------------------------------------------------------
//  LER CHAVES
// -----------------------------------------------------------
int ler_chave_publica(const char *filename, mpz_t e, mpz_t n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Erro ao abrir chave pública"); return 0; }
    if (gmp_fscanf(fp, "%Zd,%Zd", e, n) != 2) {
        printf("Erro ao ler chave pública.\n");
        fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

int ler_chave_privada(const char *filename, mpz_t d, mpz_t n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Erro ao abrir chave privada"); return 0; }
    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp); // ignora "Chave Privada:"
    if (gmp_fscanf(fp, "%Zd,%Zd", d, n) != 2) {
        printf("Erro ao ler chave privada.\n");
        fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

// -----------------------------------------------------------
//  CIFRAR
// -----------------------------------------------------------
size_t calcular_tamanho_bloco(mpz_t n) {
    size_t bits_n = mpz_sizeinbase(n, 2);
    return (bits_n - 1) / 8;
}

int cifrar_ficheiro(const char *input_file, const char *output_file, mpz_t e, mpz_t n) {
    FILE *in = fopen(input_file, "rb");
    if (!in) { perror("Erro ao abrir ficheiro"); return 0; }
    FILE *out = fopen(output_file, "w");
    if (!out) { perror("Erro ao abrir ficheiro"); fclose(in); return 0; }

    size_t block_size = calcular_tamanho_bloco(n);
    unsigned char *buffer = malloc(block_size);
    if (!buffer) { perror("Memória insuficiente"); fclose(in); fclose(out); return 0; }

    mpz_t m, c;
    mpz_inits(m, c, NULL);

    size_t read_bytes;
    while ((read_bytes = fread(buffer, 1, block_size, in)) > 0) {
        mpz_set_ui(m, 0);
        for (size_t i = 0; i < read_bytes; i++) {
            mpz_mul_2exp(m, m, 8);
            mpz_add_ui(m, m, buffer[i]);
        }
        mpz_powm(c, m, e, n);
        gmp_fprintf(out, "%zu %Zd\n", read_bytes, c);
    }

    free(buffer);
    mpz_clears(m, c, NULL);
    fclose(in);
    fclose(out);
    return 1;
}

// -----------------------------------------------------------
//  DECIFRAR
// -----------------------------------------------------------
int decifrar_ficheiro(const char *input_file, const char *output_file, mpz_t d, mpz_t n) {
    FILE *in = fopen(input_file, "r");
    if (!in) { perror("Erro ao abrir ficheiro"); return 0; }
    FILE *out = fopen(output_file, "wb");
    if (!out) { perror("Erro ao abrir ficheiro"); fclose(in); return 0; }

    size_t block_size = calcular_tamanho_bloco(n);
    unsigned char *bytes = malloc(block_size);
    if (!bytes) { perror("Memória insuficiente"); fclose(in); fclose(out); return 0; }

    mpz_t m, c;
    mpz_inits(m, c, NULL);
    size_t real_size;

    while (fscanf(in, "%zu", &real_size) == 1) {
        if (gmp_fscanf(in, "%Zd", c) != 1) break;
        mpz_powm(m, c, d, n);

        for (int i = real_size - 1; i >= 0; i--) {
            bytes[i] = mpz_fdiv_ui(m, 256);
            mpz_fdiv_q_2exp(m, m, 8);
        }
        fwrite(bytes, 1, real_size, out);
    }

    free(bytes);
    mpz_clears(m, c, NULL);
    fclose(in);
    fclose(out);
    return 1;
}

// -----------------------------------------------------------
//  MAIN
// -----------------------------------------------------------
int main() {
    mpz_t e, d, n_pub, n_priv;
    mpz_inits(e, d, n_pub, n_priv, NULL);

    char opcao[10];
    char filename[MAX_FILENAME];
    char outputfile[MAX_FILENAME];

    while (1) {
        printf("\n==== MENU ====\n");
        printf("1. Cifrar ficheiro com chave publica\n");
        printf("2. Decifrar ficheiro com chave privada\n");
        printf("3. Gerar novas chaves RSA\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%s", opcao);

        if (strcmp(opcao, "1") == 0) {
            if (!ler_chave_publica("public.myasc", e, n_pub)) continue;
            printf("Nome do ficheiro a cifrar: ");
            scanf("%s", filename);
            printf("Nome do ficheiro de saída cifrado: ");
            scanf("%s", outputfile);
            if (cifrar_ficheiro(filename, outputfile, e, n_pub))
                printf("Ficheiro cifrado com sucesso!\n");
        }
        else if (strcmp(opcao, "2") == 0) {
            if (!ler_chave_privada("secret.myasc", d, n_priv)) continue;
            printf("Nome do ficheiro a decifrar: ");
            scanf("%s", filename);
            printf("Nome do ficheiro de saída decifrado: ");
            scanf("%s", outputfile);
            if (decifrar_ficheiro(filename, outputfile, d, n_priv))
                printf("Ficheiro decifrado com sucesso!\n");
        }
        else if (strcmp(opcao, "3") == 0) {
            unsigned int k;
            printf("Introduza o tamanho da chave (k bits): ");
            scanf("%u", &k);
            gerar_chave_RSA(k);
        }
        else if (strcmp(opcao, "4") == 0) {
            break;
        }
        else {
            printf("Opcao inválida.\n");
        }
    }

    mpz_clears(e, d, n_pub, n_priv, NULL);
    return 0;
}
