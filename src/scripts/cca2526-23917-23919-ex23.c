#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#define MAX_FILENAME 256
#define AES_KEY_SIZE 32
#define AES_IV_SIZE 16

int ler_chave_publica(const char *filename, mpz_t e, mpz_t n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("Erro ao abrir chave pública"); return 0; }
    if (gmp_fscanf(fp, "%Zd,%Zd", e, n) != 2) {
        printf("Erro ao ler chave pública.\n"); fclose(fp); return 0;
    }
    fclose(fp);
    return 1;
}

int ler_chave_privada(const char *filename, mpz_t d, mpz_t n) {
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

size_t calcular_tamanho_bloco(mpz_t n) {
    size_t bits_n = mpz_sizeinbase(n, 2);
    size_t max_bytes = (bits_n - 1) / 8;
    return max_bytes;
}

int cifrar_simetrico(const char *input_file, const char *output_file, const unsigned char *key, const unsigned char *iv) {
    FILE *in = fopen(input_file, "rb");
    if (!in) { perror("Erro ao abrir ficheiro"); return 0; }
    FILE *out = fopen(output_file, "wb");
    if (!out) { perror("Erro ao abrir ficheiro"); fclose(in); return 0; }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char buffer_in[1024], buffer_out[1040];
    int len, out_len;

    while ((len = fread(buffer_in, 1, sizeof(buffer_in), in)) > 0) {
        EVP_EncryptUpdate(ctx, buffer_out, &out_len, buffer_in, len);
        fwrite(buffer_out, 1, out_len, out);
    }

    EVP_EncryptFinal_ex(ctx, buffer_out, &out_len);
    fwrite(buffer_out, 1, out_len, out);

    EVP_CIPHER_CTX_free(ctx);
    fclose(in);
    fclose(out);
    return 1;
}

int decifrar_simetrico(const char *input_file, const char *output_file, const unsigned char *key, const unsigned char *iv) {
    FILE *in = fopen(input_file, "rb");
    if (!in) { perror("Erro ao abrir ficheiro"); return 0; }
    FILE *out = fopen(output_file, "wb");
    if (!out) { perror("Erro ao abrir ficheiro"); fclose(in); return 0; }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    unsigned char buffer_in[1024], buffer_out[1040];
    int len, out_len;

    while ((len = fread(buffer_in, 1, sizeof(buffer_in), in)) > 0) {
        EVP_DecryptUpdate(ctx, buffer_out, &out_len, buffer_in, len);
        fwrite(buffer_out, 1, out_len, out);
    }

    if (EVP_DecryptFinal_ex(ctx, buffer_out, &out_len) <= 0) {
        printf("Erro na decifragem simétrica.\n");
        fclose(in);
        fclose(out);
        return 0;
    }
    fwrite(buffer_out, 1, out_len, out);

    EVP_CIPHER_CTX_free(ctx);
    fclose(in);
    fclose(out);
    return 1;
}

// Funções RSA existentes omitidas por brevidade no exemplo...

int main() {
    mpz_t e, d, n_pub, n_priv;
    mpz_inits(e, d, n_pub, n_priv, NULL);

    char opcao[10];
    char filename[MAX_FILENAME];
    char outputfile[MAX_FILENAME];

    unsigned char key[AES_KEY_SIZE];
    unsigned char iv[AES_IV_SIZE];

    while (1) {
        printf("\n==== MENU ===="
               "\n1. Cifrar ficheiro com RSA"
               "\n2. Decifrar ficheiro com RSA"
               "\n3. Cifrar ficheiro com chave simétrica (AES-256)"
               "\n4. Decifrar ficheiro com chave simétrica (AES-256)"
               "\n5. Sair"
               "\nEscolha uma opcao: ");
        scanf("%s", opcao);

        if (strcmp(opcao, "3") == 0) {
            RAND_bytes(key, AES_KEY_SIZE);
            RAND_bytes(iv, AES_IV_SIZE);
            printf("Nome do ficheiro a cifrar: "); scanf("%s", filename);
            printf("Nome de saída: "); scanf("%s", outputfile);
            cifrar_simetrico(filename, outputfile, key, iv);
            printf("Chave para decifrar (hex): ");
            for (int i = 0; i < AES_KEY_SIZE; i++) printf("%02X", key[i]);
            printf("\nIV (hex): ");
            for (int i = 0; i < AES_IV_SIZE; i++) printf("%02X", iv[i]);
            printf("\n");
        }

        else if (strcmp(opcao, "4") == 0) {
            char key_hex[65], iv_hex[33];
            printf("Introduza a chave em hex: "); scanf("%64s", key_hex);
            printf("Introduza o IV em hex: "); scanf("%32s", iv_hex);
            for (int i = 0; i < AES_KEY_SIZE; i++) sscanf(&key_hex[i*2], "%2hhx", &key[i]);
            for (int i = 0; i < AES_IV_SIZE; i++) sscanf(&iv_hex[i*2], "%2hhx", &iv[i]);
            printf("Ficheiro cifrado: "); scanf("%s", filename);
            printf("Ficheiro decifrado: "); scanf("%s", outputfile);
            decifrar_simetrico(filename, outputfile, key, iv);
        }

        else if (strcmp(opcao, "5") == 0) break;
        else printf("Opcao inválida.\n");
    }

    mpz_clears(e, d, n_pub, n_priv, NULL);
    return 0;
}
