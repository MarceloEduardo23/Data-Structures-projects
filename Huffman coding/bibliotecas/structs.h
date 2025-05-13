/**
 * @file structs.h
 * @brief Definições de estruturas e constantes utilizadas no algoritmo de compressão Huffman.
 */

#include <stdio.h>      /**< Para operações com arquivos (fopen, fread, etc.) */
#include <stdlib.h>     /**< Para alocação de memória (malloc, free) */
#include <ctype.h>      /**< Para verificar caracteres imprimíveis (isprint) */
#include <windows.h>    /**< Para suporte a acentuação no Windows (SetConsoleOutputCP) */
#include <string.h>     /**< Para operações com strings (strcat, strcmp, etc.) */

/** 
 * @def MAX_LEITURA
 * @brief Tamanho máximo para leitura de dados via entrada padrão.
 */
#define MAX_LEITURA 100

/** 
 * @def TAM_ASCII
 * @brief Tamanho total da tabela ASCII padrão.
 */
#define TAM_ASCII 256

/**
 * @struct NOHUFF
 * @brief Estrutura que representa um nó da árvore de Huffman.
 *
 * Cada nó armazena:
 * - Um ponteiro genérico para o caractere ou valor.
 * - A frequência de ocorrência desse caractere.
 * - Ponteiros para o próximo nó (caso em lista).
 * - Ponteiros para os nós esquerdo e direito na árvore de Huffman.
 */
typedef struct nohuff {
    void *caracter;                       
    int frequencia;                       
    struct nohuff *prox;               
    struct nohuff *esquerda, *direita;
} NOHUFF;

/**
 * @struct LISTA
 * @brief Estrutura que representa uma lista encadeada ordenada de nós de Huffman.
 *
 * A lista é usada como estrutura auxiliar para construir a árvore de Huffman.
 * Ela mantém os nós em ordem crescente de frequência (do menor para o maior).
 *
 * Campos:
 * - NOHUFF* inicio: ponteiro para o primeiro nó da lista.
 * - int tamanho: número de elementos atualmente na lista.
 *
 * Essa lista permite:
 * - Inserção ordenada de nós.
 * - Remoção dos dois nós com menor frequência (para combinação na árvore).
 */
typedef struct{
    NOHUFF *inicio; 
    int tamanho;
}LISTA;
