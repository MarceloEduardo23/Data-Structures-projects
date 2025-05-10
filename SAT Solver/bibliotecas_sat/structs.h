#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <stdbool.h> 

#define MAX 100

/**
 * @brief Enumeração para os estados de valoração das variáveis.
 * 
 * Define os três possíveis estados para uma variável: TRUE, FALSE e UNDEF (indefinido).
 */
typedef enum {
    FALSE = 0, /**< Valor lógico FALSO */
    TRUE = 1,  /**< Valor lógico VERDADEIRO */
    UNDEF = -1 /**< Valor lógico INDEFINIDO */
} ESTADO;

/**
 * @brief Estrutura para nó da árvore binária de decisão.
 * 
 * Cada nó representa uma variável com seu estado (TRUE, FALSE ou UNDEF)
 * e ponteiros para os filhos da esquerda (representando TRUE) e direita (representando FALSE).
 */
typedef struct arvore {
    int var;           /**< A variável representada por este nó da árvore */
    ESTADO estado;     /**< O estado (TRUE, FALSE ou UNDEF) da variável */
    struct arvore *direita; /**< Ponteiro para o filho à direita (valor FALSO) */
    struct arvore *esquerda; /**< Ponteiro para o filho à esquerda (valor VERDADEIRO) */
} ARVORE;

/**
 * @brief Estrutura para nó da lista encadeada de literais.
 * 
 * Armazena um literal (que pode ser uma variável positiva ou negativa) como um inteiro,
 * além de um ponteiro para o próximo nó da lista.
 */
typedef struct no {
    int item; /**< O literal, representado como um inteiro */
    struct no *prox; /**< Ponteiro para o próximo nó na lista */
} NO;

/**
 * @brief Estrutura para cláusula da fórmula CNF.
 * 
 * Uma cláusula é uma disjunção (OR) de literais e é representada como uma lista encadeada
 * de literais. Esta estrutura aponta para o primeiro literal da lista.
 */
typedef struct clausula {
    NO *literais; /**< Ponteiro para a lista de literais da cláusula */
    struct clausula *prox; /**< Ponteiro para a próxima cláusula na fórmula */
} CLAUSULA;

/**
 * @brief Estrutura para representação da fórmula CNF completa.
 * 
 * A fórmula CNF é composta por uma conjunção (AND) de cláusulas. Esta estrutura armazena
 * o número de variáveis, o número de cláusulas e um ponteiro para a primeira cláusula da fórmula.
 */
typedef struct formula {
    int num_var; /**< Número de variáveis na fórmula CNF */
    int num_sentencas; /**< Número de cláusulas (sentenças) na fórmula CNF */
    CLAUSULA *inicio; /**< Ponteiro para a primeira cláusula da fórmula */
} FORMULA;
