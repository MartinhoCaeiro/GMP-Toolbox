#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

// Initialize RC4 state array
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

// RC4 encrypt/decrypt data (symmetric cipher)
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

// Read text file into buffer
int read_text_file(const char *nome, unsigned char **buffer) {
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

// Read binary file into buffer
int read_binary_file(const char *nome, unsigned char **buffer) {
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

// Write data to binary file
void write_binary_file(const char *nome, unsigned char *data, int len) {
    FILE *f = fopen(nome, "wb");
    fwrite(data, 1, len, f);
    fclose(f);
}

// Convert HEX string to bytes
int hex_string_to_bytes(const char *hex_str, unsigned char **bytes) {
    int len = strlen(hex_str);
    
    // Remove spaces and count hex digits
    int hex_len = 0;
    for (int i = 0; i < len; i++) {
        if (hex_str[i] != ' ' && hex_str[i] != '\n')
            hex_len++;
    }
    
    if (hex_len % 2 != 0) return -1;  // Invalid hex string
    
    int byte_len = hex_len / 2;
    *bytes = (unsigned char *) malloc(byte_len);
    if (!*bytes) return -1;
    
    int idx = 0;
    for (int i = 0; i < len; i++) {
        if (hex_str[i] == ' ' || hex_str[i] == '\n') continue;
        
        unsigned char high = hex_str[i];
        unsigned char low = hex_str[++i];
        
        if (high >= '0' && high <= '9') high -= '0';
        else if (high >= 'A' && high <= 'F') high -= 'A' - 10;
        else if (high >= 'a' && high <= 'f') high -= 'a' - 10;
        else return -1;
        
        if (low >= '0' && low <= '9') low -= '0';
        else if (low >= 'A' && low <= 'F') low -= 'A' - 10;
        else if (low >= 'a' && low <= 'f') low -= 'a' - 10;
        else return -1;
        
        (*bytes)[idx++] = (high << 4) | low;
    }
    
    return byte_len;
}

// Load RC4 key from GMP file
int load_key_gmp(const char *ficheiro, unsigned char **key) {
    mpz_t k;
    mpz_init(k);

    FILE *f = fopen(ficheiro, "r");
    if (!f) return -1;

    if (mpz_inp_str(k, f, 10) == 0) { fclose(f); mpz_clear(k); return -1; }
    fclose(f);

    // Convert GMP integer to bytes
    size_t len = (mpz_sizeinbase(k, 2) + 7) / 8;
    unsigned char *buf = (unsigned char *) malloc(len);
    mpz_export(buf, &len, 1, 1, 1, 0, k);  

    *key = buf;
    mpz_clear(k);
    return (int)len;
}

int main() {
    unsigned char *key = NULL;
    int keylen;
    char key_filename[100];

    printf("Nome do ficheiro com a chave RC4 (GMP inteiro): ");
    scanf("%99s", key_filename);

    keylen = load_key_gmp(key_filename, &key);
    if (keylen <= 0) {
        printf("Erro ao carregar chave.\n");
        return 1;
    }

    int option;
    do {
        printf("\n===== RC4 (com chave GMP) =====\n");
        printf("\n1 - Cifrar");
        printf("\n2 - Decifrar");
        printf("\n0 - Sair");
        printf("\nEscolha uma opção: ");
        scanf("%d", &option);

        if (option == 1 || option == 2) {
            int type;
            unsigned char *entry = NULL;
            int size = 0;

            printf("\nEntrada:");
            printf("\n1 - Inline");
            printf("\n2 - Ficheiro de texto");
            printf("\n3 - Ficheiro binario");
            printf("\nEscolha uma opção: ");
            scanf("%d", &type);

            if (type == 1) {
                char temp[4096];
                
                if (option == 1) {
                    // Encrypt mode
                    printf("Introduza a mensagem: ");
                    getchar();
                    fgets(temp, sizeof(temp), stdin);

                    size = strlen(temp);
                    if (temp[size - 1] == '\n') size--;

                    entry = (unsigned char *) malloc(size);
                    memcpy(entry, temp, size);
                } else {
                    // Decrypt mode - input should be HEX
                    printf("Introduza o criptograma em HEX (ex: 1A 2B 3C): ");
                    getchar();
                    fgets(temp, sizeof(temp), stdin);

                    size = hex_string_to_bytes(temp, &entry);
                    if (size <= 0) {
                        printf("Erro: formato HEX inválido.\n");
                        continue;
                    }
                }

            } else if (type == 2) {
                char nome[100];
                printf("Nome do ficheiro: ");
                scanf("%99s", nome);
                size = read_text_file(nome, &entry);
                if (size < 0) { printf("Erro ao ler ficheiro.\n"); continue; }

            } else if (type == 3) {
                char nome[100];
                printf("Nome do ficheiro binario: ");
                scanf("%99s", nome);
                size = read_binary_file(nome, &entry);
                if (size < 0) { printf("Erro ao ler ficheiro.\n"); continue; }
            }

            // RC4 is symmetric
            rc4_crypt(entry, size, key, keylen);

            // Display or save result
            printf("\nResultado:\n");
            
            if (type == 1) {
                // Inline mode - only print to screen, NEVER save files
                if (option == 1) {
                    // Encrypt mode - show HEX
                    printf("HEX (%d bytes):\n", size);
                    for (int i = 0; i < size; i++)
                        printf("%02X ", entry[i]);
                    printf("\n");
                } else {
                    // Decrypt mode - show both HEX and text
                    printf("HEX (%d bytes):\n", size);
                    for (int i = 0; i < size; i++)
                        printf("%02X ", entry[i]);
                    printf("\n");
                    
                    printf("Texto:\n");
                    for (int i = 0; i < size; i++)
                        printf("%c", entry[i]);
                    printf("\n");
                }
            } else if (type == 2 || type == 3) {
                // File mode (text or binary) - save to file
                char out[100];
                printf("Nome do ficheiro de saida: ");
                scanf("%99s", out);
                write_binary_file(out, entry, size);
                printf("Guardado em %s.\n", out);
            }

            free(entry);
        }

    } while (option != 0);

    free(key);
    return 0;
}
