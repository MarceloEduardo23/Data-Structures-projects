#include "structs.h"

/**
 * @brief Retorna o tamanho em bytes de um arquivo.
 * 
 * @param arquivo_entrada Ponteiro para o arquivo aberto.
 * @return unsigned long Tamanho do arquivo em bytes ou -1 se inválido.
 */
unsigned long tamanho_arquivo(FILE *arquivo_entrada){
    if(!arquivo_entrada) return -1;

    fseek(arquivo_entrada, 0, SEEK_END);
    unsigned long tam_arq = ftell(arquivo_entrada);
    rewind(arquivo_entrada);
    
    return tam_arq;
}

/**
 * @brief Conta a frequência de cada byte no arquivo.
 * 
 * @param arquivo_entrada Ponteiro para o arquivo.
 * @param tam_arq Tamanho do arquivo.
 * @return unsigned long* Vetor de frequência com 256 posições.
 */
unsigned long *contar_frequencia(FILE *arquivo_entrada, unsigned long tam_arq){
    unsigned long *frequencia = calloc(TAM_ASCII, sizeof(unsigned long));
    unsigned char byte;

    for(int i = 0; i < tam_arq; i++){
        fread(&byte, sizeof(unsigned char), 1, arquivo_entrada);
        frequencia[byte]++;
    }

    rewind(arquivo_entrada);
    return frequencia;
}

/**
 * @brief Cria uma heap mínima.
 * 
 * @param capacidade Número máximo de elementos.
 * @return HEAP* Ponteiro para a heap criada.
 */
HEAP *criar_heap(int capacidade) {
    HEAP *heap = malloc(sizeof(HEAP));
    heap->dados = malloc(capacidade * sizeof(NOHUFF*));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

/**
 * @brief Troca dois ponteiros de nós da árvore.
 * 
 * @param a Ponteiro para o primeiro nó.
 * @param b Ponteiro para o segundo nó.
 */
void trocar_no(NOHUFF **a, NOHUFF **b) {
    NOHUFF *temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Reorganiza a heap para cima a partir de um índice.
 * 
 * @param heap Ponteiro para a heap.
 * @param indice Índice do elemento a reposicionar.
 */
void heapify_up(HEAP *heap, int indice) {
    int pai = (indice - 1) / 2;
    if (indice && heap->dados[indice]->frequencia < heap->dados[pai]->frequencia) {
        trocar_no(&heap->dados[indice], &heap->dados[pai]);
        heapify_up(heap, pai);
    }
}

/**
 * @brief Reorganiza a heap para baixo a partir de um índice.
 * 
 * @param heap Ponteiro para a heap.
 * @param indice Índice inicial.
 */
void heapify_down(HEAP *heap, int indice) {
    int menor = indice;
    int esq = 2 * indice + 1;
    int dir = 2 * indice + 2;

    if (esq < heap->tamanho && heap->dados[esq]->frequencia < heap->dados[menor]->frequencia)
        menor = esq;
    if (dir < heap->tamanho && heap->dados[dir]->frequencia < heap->dados[menor]->frequencia)
        menor = dir;

    if (menor != indice) {
        trocar_no(&heap->dados[indice], &heap->dados[menor]);
        heapify_down(heap, menor);
    }
}

/**
 * @brief Insere um nó na heap.
 * 
 * @param heap Ponteiro para a heap.
 * @param no Ponteiro para o nó.
 */
void inserir_heap(HEAP *heap, NOHUFF *no) {
    heap->dados[heap->tamanho] = no;
    heapify_up(heap, heap->tamanho);
    heap->tamanho++;
}

/**
 * @brief Remove e retorna o menor elemento da heap.
 * 
 * @param heap Ponteiro para a heap.
 * @return NOHUFF* Ponteiro para o nó removido.
 */
NOHUFF *remover_minimo(HEAP *heap) {
    if (heap->tamanho == 0) return NULL;
    NOHUFF *minimo = heap->dados[0];
    heap->dados[0] = heap->dados[--heap->tamanho];
    heapify_down(heap, 0);
    return minimo;
}

/**
 * @brief Preenche a heap com nós a partir das frequências.
 * 
 * @param frequencia Vetor de frequência.
 * @param heap Ponteiro para a heap.
 */
void preencher_heap(unsigned long *frequencia, HEAP *heap) {
    NOHUFF *novo;

    for (int i = 0; i < TAM_ASCII; i++) {
        if (frequencia[i] > 0) {
            novo = malloc(sizeof(NOHUFF));
            novo->caracter = malloc(sizeof(unsigned char));
            *(unsigned char*) novo->caracter = i;
            novo->frequencia = frequencia[i];
            novo->direita = NULL;
            novo->esquerda = NULL;
            novo->prox = NULL;

            inserir_heap(heap, novo);
        }
    }
}

/**
 * @brief Monta a árvore de Huffman a partir da heap.
 * 
 * @param heap Ponteiro para a heap.
 * @return NOHUFF* Ponteiro para a raiz da árvore.
 */
NOHUFF *montar_arvore_heap(HEAP *heap) {
    NOHUFF *primeiro, *segundo, *novo;

    while (heap->tamanho > 1) {
        primeiro = remover_minimo(heap);
        segundo = remover_minimo(heap);

        novo = malloc(sizeof(NOHUFF));
        novo->caracter = malloc(sizeof(unsigned char));
        *(unsigned char*) novo->caracter = '*';
        novo->frequencia = primeiro->frequencia + segundo->frequencia;
        novo->esquerda = primeiro;
        novo->direita = segundo;
        novo->prox = NULL;

        inserir_heap(heap, novo);
    }

    return remover_minimo(heap);
}

/**
 * @brief Calcula a altura da árvore de Huffman.
 * 
 * @param raiz Ponteiro para a raiz da árvore.
 * @return unsigned int Altura da árvore.
 */
unsigned int altura_arvore(NOHUFF *raiz){
    if(!raiz) return -1;

    int esq = altura_arvore(raiz->esquerda) + 1;
    int dir = altura_arvore(raiz->direita) + 1;

    return (esq > dir) ? esq : dir;
}

/**
 * @brief Gera o dicionário de códigos binários para cada caractere.
 * 
 * @param dicionario Vetor de strings com os códigos.
 * @param raiz Ponteiro para a árvore.
 * @param caminho Caminho atual (string de 0s e 1s).
 * @param tam_max Tamanho máximo das strings do caminho.
 */
void gerar_dicionario(unsigned char **dicionario, NOHUFF *raiz, char *caminho, unsigned int tam_max){
    char esquerda[tam_max], direita[tam_max];

    if(!raiz->esquerda && !raiz->direita)
        strcpy(dicionario[*(unsigned char*)raiz->caracter], caminho);
    else{
        strcpy(esquerda, caminho);
        strcpy(direita, caminho);

        strcat(esquerda, "0");
        strcat(direita, "1");

        gerar_dicionario(dicionario, raiz->esquerda, esquerda, tam_max);
        gerar_dicionario(dicionario, raiz->direita, direita, tam_max);
    }
}

/**
 * @brief Salva a árvore de Huffman serializada em pré-ordem no arquivo.
 * 
 * @param raiz Ponteiro para a árvore.
 * @param arquivo_saida Arquivo de saída.
 * @return short Quantidade de caracteres salvos.
 */
short salvar_arvore(NOHUFF *raiz, FILE *arquivo_saida){
    if(!raiz) return 0;
    
    int folha_escape = (*(unsigned char*)raiz->caracter == '*' || *(unsigned char*)raiz->caracter == '\\') && !raiz->esquerda && !raiz->direita;

    if(folha_escape)
        fwrite("\\", sizeof(unsigned char), 1, arquivo_saida);
    
    fwrite(&*(unsigned char*)raiz->caracter, sizeof(unsigned char), 1, arquivo_saida);
    int esquerda = salvar_arvore(raiz->esquerda, arquivo_saida);
    int direita = salvar_arvore(raiz->direita, arquivo_saida);
    
    return 1 + esquerda + direita + folha_escape;
}

/**
 * @brief Salva os dados comprimidos no arquivo.
 * 
 * @param arquivo_entrada Ponteiro para o arquivo original.
 * @param arquivo_saida Ponteiro para o arquivo de saída.
 * @param dicionario Vetor de dicionário de Huffman.
 * @param tam_arq Tamanho do arquivo original.
 * @param tam_arvore Tamanho da arvore binaria.
 */
short salvar_dados(FILE *arquivo_entrada, FILE *arquivo_saida, unsigned char **dicionario, int tam_arquivo, int tam_arvore){
    int bit_atual = 0;
    int tamanho_lixo = 0;
    unsigned char escrever_buffer = 0;
    unsigned char ler_buffer;

    fseek(arquivo_saida, 0, SEEK_END);

    for (int i = 0; i < tam_arquivo; i++){
        fread(&ler_buffer, sizeof(unsigned char), 1, arquivo_entrada);
        unsigned char *codigo = dicionario[ler_buffer];

        for(int j = 0; codigo[j] != '\0'; j++){
            escrever_buffer <<= 1;
            if(codigo[j] == '1')
                escrever_buffer |= 1;
            
            bit_atual++;

            if(bit_atual == 8){
                fwrite(&escrever_buffer, sizeof(unsigned char), 1, arquivo_saida);
                escrever_buffer = 0;
                bit_atual = 0;
            }
        }
    }

    if(bit_atual > 0){
        escrever_buffer <<= 8 - bit_atual;
        fwrite(&escrever_buffer, sizeof(unsigned char), 1, arquivo_saida);
        tamanho_lixo = 8 - bit_atual;
    }

    fclose(arquivo_entrada);
    rewind(arquivo_saida);
    return tamanho_lixo;
}

/**
 * @brief Substitui os dois caracteres iniciais por 3 bits para o lixo e 13 bits para o tamanho da arvore resultando em .
 *  bytes
 * @param arquivo_saida Nome do arquivo de saida.
 * @param tam_lixo Tamanho do lixo no final do arquivo.
 * @param tam_arvore Tamanho da arvore binaria.
 */
void salvar_cabecalho(FILE *arquivo_saida, unsigned short tam_lixo, unsigned short tam_arvore){
    fseek(arquivo_saida, 0, SEEK_SET);
    tam_lixo <<= 13;
    short cabecalho = tam_lixo | tam_arvore;
    unsigned char buffer = cabecalho >> 8;
    fwrite(&buffer, sizeof(unsigned char), 1, arquivo_saida);
    buffer = cabecalho;
    fwrite(&buffer, sizeof(unsigned char), 1, arquivo_saida);
}

/**
 * @brief Le o cabecalho do arquivo compactado para descobrir o tamanho do lixo e arvore.
 * 
 * @param arquivo_entrada Nome do arquivo de entrada.
 * @param tam_lixo Ponteiro para guardar o tamanho do lixo no final do arquivo.
 * @param tam_arvore Ponteiro para guardar o tamanho da arvore binaria.
 */
void ler_cabecalho(FILE *arquivo_entrada, unsigned short *tam_lixo, unsigned short *tam_arvore){
    unsigned char buffer;
    fread(&buffer, sizeof(char), 1, arquivo_entrada);

    unsigned short cabecalho = buffer << 8;

    fread(&buffer, sizeof(char), 1, arquivo_entrada);

    cabecalho |= buffer;

    *tam_lixo = cabecalho >> 13;
    *tam_arvore = cabecalho & 0x1FFF; // USADO COMO MASCARA (0X1FFFF)

    printf("\n\tTamanho Lixo: %d\n", *tam_lixo);
    printf("\n\tTamanho Arvore: %d\n", *tam_arvore);
}

/**
 * @brief Funcao usada para criar um novo no para arvore.
 * 
 * @param caractere Caracter a ser adicionado na arvore.
 * @param esquerda Ponteiro para o no esquerdo.
 * @param direita Ponteiro para o no direito.
 */
NOHUFF*criar_arvore(unsigned char caractere, NOHUFF *esquerda, NOHUFF *direita){
    NOHUFF *novo = malloc(sizeof(NOHUFF));
    novo->caracter = malloc(sizeof(unsigned char));
    *(unsigned char*) novo->caracter = caractere;
    novo->esquerda = esquerda;
    novo->direita = direita;

    return novo;
}

/**
 * @brief Funcao usada para remontar arvore.
 * 
 * @param arquivo_entrada Nome do arquivo entrada.
 * @param tam_arvore Tamanho da arvore.
 */
NOHUFF *remontar_arvore(FILE *arquivo_entrada, unsigned short *tam_arvore){
    unsigned char buffer;
    fread(&buffer, sizeof(unsigned char), 1, arquivo_entrada);

    int e_folha = 0;
    if(*tam_arvore == 0)
        return NULL;
    (*tam_arvore)--;
    if(buffer == '\\'){
        (*tam_arvore)--;
        fread(&buffer, sizeof(unsigned char), 1, arquivo_entrada);
        e_folha = 1;
    }
    if(buffer != '*'){
        e_folha = 1;
    }

    if(e_folha){
        return criar_arvore(buffer, NULL, NULL);
    }
    NOHUFF *esquerda = remontar_arvore(arquivo_entrada, tam_arvore);
    NOHUFF *direita = remontar_arvore(arquivo_entrada, tam_arvore);
    return criar_arvore('*', esquerda, direita);
}

/**
 * @brief Funcao principal para decodificar o arquivo.
 * 
 * @param arquivo_entrada Nome do arquivo entrada.
 * @param arquivo_saida Nome do arquivo saida.
 * @param tam_arquivo Tamanho do arquivo compactado.
 * @param tam_lixo Tamanho do lixo no ultimo byte do arquivo.
 * @param tam_arvore Tamanho da arvore.
 */
void decodificar(FILE *arquivo_entrada, FILE *arquivo_saida, unsigned long tam_arquivo, unsigned short tam_lixo, unsigned short tam_arvore){
    tam_arquivo -= tam_arvore + 2;
    tam_arquivo <<= 3;
    tam_arquivo -= tam_lixo;

    NOHUFF *raiz = remontar_arvore(arquivo_entrada, &tam_arvore);
    NOHUFF *aux = raiz;
    unsigned char buffer;
    int bit_atual = 0;

    while(tam_arquivo--){
        if(bit_atual == 0){
            fread(&buffer, sizeof(unsigned char), 1, arquivo_entrada);
            bit_atual = 8;
        }

        if(buffer & (1 << --bit_atual)){
            aux = aux->direita;
        }else{
            aux = aux->esquerda;
        }

        if(aux->esquerda == NULL && aux->direita == NULL){
            fwrite(&*(unsigned char*)aux->caracter, sizeof(unsigned char), 1, arquivo_saida);
            aux = raiz;
        }
    }
}
