#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

mpz_t a, c, m, seed;

// LCG function
void lcg(mpz_t result) {
    mpz_mul(result, a, seed);   
    mpz_add(result, result, c);  
    mpz_mod(result, result, m);  
    mpz_set(seed, result);      
}

int main() {
    FILE *fconfig, *fout;
    char nomeConfig[100], nomeSaida[100];
    int n, option;

    mpz_init(a);
    mpz_init(c);
    mpz_init(m);
    mpz_init(seed);
    printf("Deve ter o formato: Multiplicador (a), Incremento (c), Módulo (m), Semente (seed) (cada valor em uma linha)\n");
    printf("Nome do ficheiro de configuração: ");
    scanf("%99s", nomeConfig);

    fconfig = fopen(nomeConfig, "r");
    if (!fconfig) {
        printf("Erro ao abrir o ficheiro.\n");
        return 1;
    }

    // Read values from file (a, c, m, seed)
    if (mpz_inp_str(a, fconfig, 10) == 0 ||
        mpz_inp_str(c, fconfig, 10) == 0 ||
        mpz_inp_str(m, fconfig, 10) == 0 ||
        mpz_inp_str(seed, fconfig, 10) == 0) 
    {
        printf("Erro ao ler valores do ficheiro.\n");
        fclose(fconfig);
        return 1;
    }
    fclose(fconfig);

    do {
        printf("\n=====Gerador LCG (GMP) =====\n");
        printf("\n1 - Gerar valores pseudo-aleatorios");
        printf("\n0 - Sair");
        printf("\nEscolha uma opção: ");
        scanf("%d", &option);

        if (option == 1) {
            printf("Quantos valores deseja gerar? ");
            scanf("%d", &n);

            printf("Nome do ficheiro de saida: ");
            scanf("%99s", nomeSaida);

            fout = fopen(nomeSaida, "w");
            if (!fout) {
                printf("Erro ao criar o ficheiro de saida.\n");
                continue;
            }

            mpz_t next;
            mpz_init(next);

            for (int i = 0; i < n; i++) {
                lcg(next);
                mpz_out_str(fout, 10, next);
                fprintf(fout, "\n");
            }

            mpz_clear(next);
            fclose(fout);

            printf("Foram gerados %d valores para '%s'.\n", n, nomeSaida);
        }

    } while (option != 0);

    mpz_clear(a);
    mpz_clear(c);
    mpz_clear(m);
    mpz_clear(seed);

    printf("Programa terminado.\n");
    return 0;
}
