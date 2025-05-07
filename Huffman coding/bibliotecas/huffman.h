#include "structs.h"

unsigned long tamanho_arquivo(FILE *arquivo_entrada){
    if(!arquivo_entrada) return -1;

    fseek(arquivo_entrada, 0, SEEK_END); // Coloca o cursor no fim do arquivo
    unsigned long tam_arq = ftell(arquivo_entrada); // Pega o tamanho do arquivo
    rewind(arquivo_entrada); // Coloca no inicio dnv

    return tam_arq;
}

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

void criar_fila(LISTA *fila){
    fila->inicio = NULL;
    fila->tamanho = 0;
}

void inserir_ordenado(LISTA *fila, NOHUFF *novo){
    if(!fila->inicio || novo->frequencia <= fila->inicio->frequencia){
        novo->prox = fila->inicio;
        fila->inicio = novo;
    }else{
        NOHUFF *aux = fila->inicio;
        while(aux->prox && aux->prox->frequencia < novo->frequencia)
            aux = aux->prox;
        novo->prox = aux->prox;
        aux->prox = novo;
    }
    fila->tamanho++;
}

void preencher_fila(unsigned long *frequencia, LISTA *fila){
    NOHUFF *novo;

    for(int i = 0; i < TAM_ASCII; i++){
        if(frequencia[i] > 0){
            novo = malloc(sizeof(NOHUFF));

            novo->caracter = malloc(sizeof(unsigned char));
            *(unsigned char*) novo->caracter = i;
            novo->frequencia = frequencia[i];
            novo->direita = NULL;
            novo->esquerda = NULL;
            novo->prox = NULL;

            inserir_ordenado(fila, novo);
        }
    }
}

NOHUFF *remove_no_inicio(LISTA *fila){
    NOHUFF *aux = NULL;

    if(fila->inicio){
        aux = fila->inicio;
        fila->inicio = aux->prox;
        aux->prox = NULL;
        fila->tamanho--;
    }

    return aux;
}

NOHUFF *montar_arvore(LISTA *fila){
    NOHUFF *primeiro, *segundo, *novo;
    while(fila->tamanho > 1){
        primeiro = remove_no_inicio(fila);
        segundo = remove_no_inicio(fila);

        novo = malloc(sizeof(NOHUFF));
        novo->caracter = malloc(sizeof(unsigned char));
        *(unsigned char*)novo->caracter = '*';
        novo->frequencia = primeiro->frequencia + segundo->frequencia;
        novo->esquerda = primeiro;
        novo->direita = segundo;
        novo->prox = NULL;

        inserir_ordenado(fila, novo);
    }

    return fila->inicio;
}

unsigned int altura_arvore(NOHUFF *raiz){
    if(!raiz) return -1;

    int esq = altura_arvore(raiz->esquerda) + 1;
    int dir = altura_arvore(raiz->direita) + 1;

    return (esq > dir) ? esq : dir;
}

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

short salvar_arvore(NOHUFF *raiz, FILE *arquivo_compactado){
    if(raiz == NULL)
        return 0;
    
    int folha_escape = (*(unsigned char*)raiz->caracter == '*' || *(unsigned char*)raiz->caracter == '\\') && !raiz->esquerda && !raiz->direita;

    if(folha_escape)
        fwrite("\\", sizeof(unsigned char), 1, arquivo_compactado);
    
    fwrite(&*(unsigned char*)raiz->caracter, sizeof(unsigned char), 1, arquivo_compactado);
    int esquerda = salvar_arvore(raiz->esquerda, arquivo_compactado);
    int direita = salvar_arvore(raiz->direita, arquivo_compactado);
    
    return 1 + esquerda + direita + folha_escape;
}

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

void salvar_cabecalho(FILE *arquivo_saida, unsigned short tamanho_lixo, unsigned short tamanho_arvore){
    fseek(arquivo_saida, 0, SEEK_SET);
    tamanho_lixo <<= 13;
    short cabecalho = tamanho_lixo | tamanho_arvore;
    unsigned char buffer = cabecalho >> 8;
    fwrite(&buffer, sizeof(unsigned char), 1, arquivo_saida);
    buffer = cabecalho;
    fwrite(&buffer, sizeof(unsigned char), 1, arquivo_saida);
}