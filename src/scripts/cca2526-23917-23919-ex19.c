#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>

#define MAX_FILENAME 256

// Read public key (e,n)
int read_public_key(const char *filename, mpz_t e, mpz_t n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Erro ao abrir chave pública"); return 0; }
    if (gmp_fscanf(fp, "%Zd,%Zd", e, n) != 2) {
        printf("Erro ao ler chave pública.\n"); fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

// Read private key (d,n)
int read_private_key(const char *filename, mpz_t d, mpz_t n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Erro ao abrir chave privada"); return 0; }
    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp); 
    if (gmp_fscanf(fp, "%Zd,%Zd", d, n) != 2) {
        printf("Erro ao ler chave privada.\n"); fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

// Calculate maximum number of bytes per block based on n
size_t calculate_block_size(mpz_t n) {
    size_t bits_n = mpz_sizeinbase(n, 2);
    size_t max_bytes = (bits_n - 1) / 8; 
    return max_bytes;
}

// Encrypt file using variable block size
int encrypt_file(const char *input_file, const char *output_file, mpz_t e, mpz_t n) {
    FILE *in = fopen(input_file, "rb");
    if (!in) { perror("Erro ao abrir ficheiro"); return 0; }
    FILE *out = fopen(output_file, "w");
    if (!out) { perror("Erro ao abrir ficheiro"); fclose(in); return 0; }

    size_t block_size = calculate_block_size(n);
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

// Decrypt file using variable block size
int decrypt_file(const char *input_file, const char *output_file, mpz_t d, mpz_t n) {
    FILE *in = fopen(input_file, "r");
    if (!in) { perror("Erro ao abrir ficheiro"); return 0; }
    FILE *out = fopen(output_file, "wb");
    if (!out) { perror("Erro ao abrir ficheiro"); fclose(in); return 0; }

    size_t block_size = calculate_block_size(n);
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

int main() {
    // Initialize mpz_t variables
    mpz_t e, d, n_pub, n_priv;
    mpz_inits(e, d, n_pub, n_priv, NULL);

    char option[10];
    char filename[MAX_FILENAME];
    char outputfile[MAX_FILENAME];

    while (1) {
        printf("\n==== MENU ====\n");
        printf("1. Cifrar ficheiro com chave publica\n");
        printf("2. Decifrar ficheiro com chave privada\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%s", option);

        if (strcmp(option, "1") == 0) {
            if (!read_public_key("public.myasc", e, n_pub)) continue;
            printf("Nome do ficheiro a cifrar: ");
            scanf("%s", filename);
            printf("Nome do ficheiro de saída cifrado: ");
            scanf("%s", outputfile);
            if (cifrar_ficheiro(filename, outputfile, e, n_pub))
                printf("Ficheiro cifrado com sucesso!\n");
        }
        else if (strcmp(option, "2") == 0) {
            if (!ler_chave_privada("secret.myasc", d, n_priv)) continue;
            printf("Nome do ficheiro a decifrar: ");
            scanf("%s", filename);
            printf("Nome do ficheiro de saída decifrado: ");
            scanf("%s", outputfile);
            if (decrypt_file(filename, outputfile, d, n_priv))
                printf("Ficheiro decifrado com sucesso!\n");
        }
        else if (strcmp(option, "3") == 0) {
            break;
        }
        else {
            printf("Opção inválida.\n");
        }
    }

    mpz_clears(e, d, n_pub, n_priv, NULL);
    return 0;
}
