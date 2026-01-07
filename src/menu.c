#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int number;
    const char *name;
    const char *executable;
} Script;

int main() {
    // Array of available scripts with display names
    Script scripts[] = {
        {1, "[Ex 1] Multiplicador de Números Inteiros", "./scripts/cca2526-23917-23919-ex1"},
        {2, "[Ex 2] Calculadora GCD", "./scripts/cca2526-23917-23919-ex2"},
        {3, "[Ex 3] Determinador de Números Primos", "./scripts/cca2526-23917-23919-ex3"},
        {4, "[Ex 4] Calculadora de Exponenciação", "./scripts/cca2526-23917-23919-ex4"},
        {5, "[Ex 5] Calculadora de Tempo de Exponenciação", "./scripts/cca2526-23917-23919-ex5"},
        {6, "[Ex 6] Comparador de Tempo de Exponenciação", "./scripts/cca2526-23917-23919-ex6"},
        {7, "[Ex 7] Números Primos Inferiores a Limite Máximo (Ciclo)", "./scripts/cca2526-23917-23919-ex7"},
        {8, "[Ex 8] Números Primos Inferiores a Limite Máximo", "./scripts/cca2526-23917-23919-ex8"},
        {9, "[Ex 9] Números Primos Inferiores a Limite Máximo (Ciclo + Ficheiro de Texto)", "./scripts/cca2526-23917-23919-ex9"},
        {10, "[Ex 10] Números Primos Inferiores a Limite Máximo (Ficheiro de Texto)", "./scripts/cca2526-23917-23919-ex10"},
        {11, "[Ex 11] Contador de Combinações", "./scripts/cca2526-23917-23919-ex11"},
        {12, "[Ex 12] Fatorização de Números Primos", "./scripts/cca2526-23917-23919-ex12"},
        {13, "[Ex 13] Fatorização de Números Primos (Ficheiro de Texto)", "./scripts/cca2526-23917-23919-ex13"},
        {14, "[Ex 14] Gerador de Chaves RSA", "./scripts/cca2526-23917-23919-ex14"},
        {15, "[Ex 15] Gerador de Chaves RSA (Com Determinantes)", "./scripts/cca2526-23917-23919-ex15"},
        {16, "[Ex 16] Gerador de Chaves RSA (Com Melhorias)", "./scripts/cca2526-23917-23919-ex16"},
        {17, "[Ex 17] Gráfico de Tempos RSA", "./scripts/cca2526-23917-23919-ex17"},
        {18, "[Ex 18] Gerador de Chaves RSA (Ficheiro de Texto)", "./scripts/cca2526-23917-23919-ex18"},
        {19, "[Ex 19] (De)cifrador de ficheiros", "./scripts/cca2526-23917-23919-ex19"},
        {20, "[Ex 20] Analise Critica do (De)cifrador de ficheiros [Não Disponivel!]", ""},
        {21, "[Ex 21] (De)cifrador de ficheiros (Tempos de Execução)", "./scripts/cca2526-23917-23919-ex21"},
        {22, "[Ex 22] (De)cifrador de ficheiros (Com Chaves de Dimensão K)", "./scripts/cca2526-23917-23919-ex22"},
        {23, "[Ex 23] (De)cifrador de ficheiros (Chaves Simétricas)", "./scripts/cca2526-23917-23919-ex23"},
        {24, "[Ex 24] Gerador de Números Pseudo-Aleatórios", "./scripts/cca2526-23917-23919-ex24"},
        {25, "[Ex 25] (De)cifrador com Cifra RC4", "./scripts/cca2526-23917-23919-ex25"},
    };

    int numScripts = sizeof(scripts) / sizeof(scripts[0]);
    int choice;

    while (1) {
        printf("\n");
        printf("╔══════════════════════════════════════╗\n");
        printf("║          GMP TOOLBOX MENU            ║\n");
        printf("╚══════════════════════════════════════╝\n");
        printf("\nScripts Disponiveis:\n\n");

        // Display menu options
        for (int i = 0; i < numScripts; i++) {
            printf("  %2d. %s\n", scripts[i].number, scripts[i].name);
        }

        printf("  0.  Sair\n\n");
        printf("Escolha um script: ");

        if (scanf("%d", &choice) != 1) {
            // Clear input buffer if non-integer input
            while (getchar() != '\n');
            printf("\nEntrada inválida! Por favor, insira um número.\n");
            continue;
        }

        // Check if user wants to exit
        if (choice == 0) {
            printf("\nAdeus!\n\n");
            break;
        }

        // Find and execute the selected script
        int found = 0;
        for (int i = 0; i < numScripts; i++) {
            if (scripts[i].number == choice) {
                printf("\n A correr: %s\n", scripts[i].name);
                printf("════════════════════════════════════════\n\n");
                
                int result = system(scripts[i].executable);
                
                printf("\n════════════════════════════════════════\n");
                if (result == 0) {
                    printf(" %s concluído com sucesso!\n", scripts[i].name);
                } else {
                    printf(" %s saiu com o código: %d\n", scripts[i].name, result);
                }
                
                found = 1;
                break;
            }
        }

        if (!found && choice > 0) {
            printf("\n Escolha inválida! Por favor, selecione uma opção válida.\n");
        }

        if (!found && choice == 20) {
            printf("\n Não Disponível! Por favor, selecione outra opção válida!\n\n");
        }
    }

    return 0;
}
