#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>

#define MAX_FILENAME 256

// Ler chave pública (e,n)
int ler_chave_publica(const char *filename, mpz_t e, mpz_t n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Erro ao abrir chave pública"); return 0; }
    if (gmp_fscanf(fp, "%Zd,%Zd", e, n) != 2) {
        printf("Erro ao ler chave pública.\n"); fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

// Ler chave privada (d,n)
int ler_chave_privada(const char *filename, mpz_t d, mpz_t n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Erro ao abrir chave privada"); return 0; }
    char buffer[1024];
    fgets(buffer, sizeof(buffer), fp); // pular primeira linha
    if (gmp_fscanf(fp, "%Zd,%Zd", d, n) != 2) {
        printf("Erro ao ler chave privada.\n"); fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

// Calcula número máximo de bytes por bloco com base em n
size_t calcular_tamanho_bloco(mpz_t n) {
    size_t bits_n = mpz_sizeinbase(n, 2);
    size_t max_bytes = (bits_n - 1) / 8; // -1 para garantir m < n
    return max_bytes;
}

// Cifra ficheiro usando blocos de tamanho variável
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
        gmp_fprintf(out, "%zu %Zd\n", read_bytes, c); // salva tamanho do bloco + cifra
    }

    free(buffer);
    mpz_clears(m, c, NULL);
    fclose(in);
    fclose(out);
    return 1;
}

// Decifra ficheiro usando blocos de tamanho variável
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

    while (fscanf(in, "%zu", &real_size) == 1) { // lê tamanho real do bloco
        if (gmp_fscanf(in, "%Zd", c) != 1) break;
        mpz_powm(m, c, d, n);
        for (int i = real_size - 1; i >= 0; i--) {
            bytes[i] = mpz_fdiv_ui(m, 256);
            mpz_fdiv_q_2exp(m, m, 8);
        }
        fwrite(bytes, 1, real_size, out); // escreve apenas os bytes válidos
    }

    free(bytes);
    mpz_clears(m, c, NULL);
    fclose(in);
    fclose(out);
    return 1;
}

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
        printf("3. Sair\n");
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
            break;
        }
        else {
            printf("Opcao inválida.\n");
        }
    }

    mpz_clears(e, d, n_pub, n_priv, NULL);
    return 0;
}
