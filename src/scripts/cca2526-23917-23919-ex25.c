#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

/* ------------ RC4 (byte oriented) ------------ */

void rc4_init(unsigned char *S, const unsigned char *key, int keylen) {
    int i, j = 0;
    unsigned char temp;

    for (i = 0; i < 256; i++)
        S[i] = i;

    for (i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keylen]) & 0xFF;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
}

void rc4_crypt(unsigned char *data, int len, const unsigned char *key, int keylen) {
    unsigned char S[256];
    rc4_init(S, key, keylen);

    int i = 0, j = 0, k, t;
    unsigned char temp;

    for (k = 0; k < len; k++) {
        i = (i + 1) & 0xFF;
        j = (j + S[i]) & 0xFF;

        temp = S[i];
        S[i] = S[j];
        S[j] = temp;

        t = (S[i] + S[j]) & 0xFF;
        data[k] ^= S[t];
    }
}

/* --------- LER FICHEIROS --------- */

int ler_ficheiro_texto(const char *nome, unsigned char **buffer) {
    FILE *f = fopen(nome, "r");
    if (!f) return -1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    *buffer = (unsigned char *) malloc(size + 1);
    if (!*buffer) { fclose(f); return -1; }

    fread(*buffer, 1, size, f);
    (*buffer)[size] = '\0';

    fclose(f);
    return (int)size;
}

int ler_ficheiro_binario(const char *nome, unsigned char **buffer) {
    FILE *f = fopen(nome, "rb");
    if (!f) return -1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    *buffer = (unsigned char *) malloc(size);
    if (!*buffer) { fclose(f); return -1; }

    fread(*buffer, 1, size, f);
    fclose(f);
    return (int)size;
}

void escrever_ficheiro_binario(const char *nome, unsigned char *data, int len) {
    FILE *f = fopen(nome, "wb");
    fwrite(data, 1, len, f);
    fclose(f);
}

/* --------- LER CHAVE EM GMP --------- */
int carregar_chave_gmp(const char *ficheiro, unsigned char **key) {
    mpz_t k;
    mpz_init(k);

    FILE *f = fopen(ficheiro, "r");
    if (!f) return -1;

    if (mpz_inp_str(k, f, 10) == 0) { fclose(f); mpz_clear(k); return -1; }
    fclose(f);

    /* Converter inteiro GMP em bytes */
    size_t len = (mpz_sizeinbase(k, 2) + 7) / 8;
    unsigned char *buf = (unsigned char *) malloc(len);
    mpz_export(buf, &len, 1, 1, 1, 0, k);  // big endian

    *key = buf;
    mpz_clear(k);
    return (int)len;
}

/* ===================== MAIN ===================== */

int main() {
    unsigned char *key = NULL;
    int keylen;
    char ficheiro_chave[100];

    printf("Nome do ficheiro com a chave RC4 (GMP inteiro): ");
    scanf("%99s", ficheiro_chave);

    keylen = carregar_chave_gmp(ficheiro_chave, &key);
    if (keylen <= 0) {
        printf("Erro ao carregar chave.\n");
        return 1;
    }

    int opcao;
    do {
        printf("\n--- RC4 (com chave GMP) ---\n");
        printf("1 - Cifrar\n");
        printf("2 - Decifrar\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1 || opcao == 2) {
            int tipo;
            unsigned char *entrada = NULL;
            int tamanho = 0;

            printf("\nEntrada:\n");
            printf("1 - Inline\n");
            printf("2 - Ficheiro de texto\n");
            printf("3 - Ficheiro binario\n");
            printf("Opcao: ");
            scanf("%d", &tipo);

            if (tipo == 1) {
                char temp[4096];
                printf("Introduza a mensagem/criptograma: ");
                getchar();
                fgets(temp, sizeof(temp), stdin);

                tamanho = strlen(temp);
                if (temp[tamanho - 1] == '\n') tamanho--;

                entrada = (unsigned char *) malloc(tamanho);
                memcpy(entrada, temp, tamanho);

            } else if (tipo == 2) {
                char nome[100];
                printf("Nome do ficheiro: ");
                scanf("%99s", nome);
                tamanho = ler_ficheiro_texto(nome, &entrada);
                if (tamanho < 0) { printf("Erro ao ler ficheiro.\n"); continue; }

            } else if (tipo == 3) {
                char nome[100];
                printf("Nome do ficheiro binario: ");
                scanf("%99s", nome);
                tamanho = ler_ficheiro_binario(nome, &entrada);
                if (tamanho < 0) { printf("Erro ao ler ficheiro.\n"); continue; }
            }

            /* RC4 é simétrico */
            rc4_crypt(entrada, tamanho, key, keylen);

            if (tipo == 1) {
                printf("\nResultado em HEX (%d bytes):\n", tamanho);
                for (int i = 0; i < tamanho; i++)
                    printf("%02X ", entrada[i]);
                printf("\n");

            } else {
                char out[100];
                printf("Nome do ficheiro de saida: ");
                scanf("%99s", out);
                escrever_ficheiro_binario(out, entrada, tamanho);
                printf("Guardado em %s.\n", out);
            }

            free(entrada);
        }

    } while (opcao != 0);

    free(key);
    return 0;
}
