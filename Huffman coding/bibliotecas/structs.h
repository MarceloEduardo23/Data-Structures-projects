#include <stdio.h> // Para operações com arquivos (fopen, fread, etc.)
#include <stdlib.h> // Para alocação de memória (malloc, free)
#include <ctype.h> // Para verificar caracteres imprimíveis (isprint)
#include <windows.h> // Para suporte a acentuação no Windows (SetConsoleOutputCP)
#include <string.h>

#define MAX_LEITURA 100 // TAMANHO MAXIMO PARA LEITURA DE DADOS
#define TAM_ASCII 256 // TAMANHO TOTAL DA TABELA ASCII

typedef struct nohuff{
    void *caracter;
    int frequencia;
    struct nohuff *prox;
    struct nohuff *esquerda, *direita;
}NOHUFF;

// Estrutura para uma lista encadeada (usada em etapas futuras)
typedef struct{
    NOHUFF *inicio; // Ponteiro para o primeiro nó
    int tamanho; // Quantidade de nós na lista
}LISTA;