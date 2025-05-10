/**
 * @file satsolver.c
 * @brief Implementação do solver SAT que lê uma fórmula CNF de um arquivo, resolve e imprime a atribuição das variáveis.
 *
 * O programa lê uma fórmula SAT (em formato CNF) de um arquivo, cria uma árvore binária de decisão, tenta resolver
 * a fórmula e imprime o resultado (SAT ou UNSAT) com a atribuição das variáveis.
 */

#include "bibliotecas_sat/satsolver.h"

/**
 * @brief Função principal do programa.
 *
 * Esta função executa o algoritmo de resolução SAT. Ela solicita o nome do arquivo CNF, carrega a fórmula do arquivo,
 * cria uma árvore binária de decisão para representar a fórmula e, em seguida, tenta resolver o problema SAT. Se a
 * fórmula for satisfatível (SAT), imprime a atribuição das variáveis. Caso contrário, imprime UNSAT.
 *
 * @return int Retorna 0 se a execução for bem-sucedida, ou 1 se houver erro na leitura do arquivo.
 */
int main(){
    char nome_arquivo[100];

    // Solicita o nome do arquivo CNF
    printf("Digite o nome do arquivo CNF: ");
    scanf("%s", nome_arquivo);

    // Tenta abrir o arquivo
    FILE *arquivo = fopen(nome_arquivo, "r");
    if(!arquivo){
        printf("ERRO: NAO FOI POSSIVEL ABRIR O ARQUIVO '%s'.\n", nome_arquivo);
        return 1; // Retorna erro se não conseguir abrir o arquivo
    }

    // Lê a fórmula do arquivo
    FORMULA formula = ler_formula(arquivo);
    fclose(arquivo); // Fecha o arquivo após leitura

    // Cria a árvore binária de decisão com base no número de variáveis
    ARVORE *raiz = criar_arvore_binaria(NULL, 1, formula.num_var);

    // Tenta resolver a fórmula usando o algoritmo SAT
    if(sat(raiz, formula)){
        printf("\nSAT\n"); // Caso a fórmula seja satisfatível
        imprimir_atribuicao(raiz, formula.num_var); // Imprime a atribuição das variáveis
    } else {
        printf("\nUNSAT\n"); // Caso a fórmula seja insatisfatível
    }

    // Libera os recursos utilizados
    liberar_formula(formula);
    liberar_arvore(raiz);

    return 0; // Retorna sucesso
}
