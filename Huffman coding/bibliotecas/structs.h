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
    void *caracter;               /**< Ponteiro para o caractere (ou valor associado). */
    int frequencia;               /**< Frequência de ocorrência do caractere. */
    struct nohuff *prox;          /**< Ponteiro para o próximo nó (utilizado em listas). */
    struct nohuff *esquerda;      /**< Ponteiro para o filho esquerdo na árvore de Huffman. */
    struct nohuff *direita;       /**< Ponteiro para o filho direito na árvore de Huffman. */
} NOHUFF;

/**
 * @struct HEAP
 * @brief Estrutura que representa uma heap mínima para montagem da árvore de Huffman.
 *
 * A heap é implementada como um vetor de ponteiros para nós de Huffman, controlado pelos campos:
 * - tamanho: quantidade atual de elementos.
 * - capacidade: capacidade máxima permitida.
 */
typedef struct {
    NOHUFF **dados;      /**< Vetor de ponteiros para nós de Huffman. */
    int tamanho;         /**< Quantidade atual de elementos no heap. */
    int capacidade;      /**< Capacidade máxima do heap. */
} HEAP; 
