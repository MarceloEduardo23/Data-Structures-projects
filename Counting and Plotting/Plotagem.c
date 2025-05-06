#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10000

// ======================== FILA DE PRIORIDADE SEM HEAP (LISTA ORDENADA) ==========================
typedef struct No {
    int valor;
    struct No* prox;
} No;

typedef struct {
    No* inicio;
    int comparacoes;
} FilaLista;

FilaLista* criarFilaLista() {
    FilaLista* f = malloc(sizeof(FilaLista));
    f->inicio = NULL;
    f->comparacoes = 0;
    return f;
}

void inserirLista(FilaLista* f, int valor) {
    No* novo = malloc(sizeof(No));
    novo->valor = valor;
    novo->prox = NULL;

    No** p = &f->inicio;
    while (*p && (*p)->valor > valor) {
        f->comparacoes++;
        p = &(*p)->prox;
    }
    f->comparacoes++;
    novo->prox = *p;
    *p = novo;
}

int removerLista(FilaLista* f) {
    if (!f->inicio) return -1;
    int val = f->inicio->valor;
    No* temp = f->inicio;
    f->inicio = f->inicio->prox;
    free(temp);
    return val;
}

// ======================== FILA DE PRIORIDADE COM HEAP ==========================
typedef struct {
    int* dados;
    int tamanho;
    int comparacoes;
} Heap;

Heap* criarHeap(int capacidade) {
    Heap* h = malloc(sizeof(Heap));
    h->dados = malloc(sizeof(int) * capacidade);
    h->tamanho = 0;
    h->comparacoes = 0;
    return h;
}

void trocar(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void subir(Heap* h, int idx) {
    while (idx > 0) {
        int pai = (idx - 1) / 2;
        h->comparacoes++;
        if (h->dados[idx] <= h->dados[pai]) break;
        trocar(&h->dados[idx], &h->dados[pai]);
        idx = pai;
    }
}

void descer(Heap* h, int idx) {
    while (2 * idx + 1 < h->tamanho) {
        int filho = 2 * idx + 1;
        if (filho + 1 < h->tamanho && h->dados[filho + 1] > h->dados[filho]) {
            filho++;
        }
        h->comparacoes++;
        if (h->dados[idx] >= h->dados[filho]) break;
        trocar(&h->dados[idx], &h->dados[filho]);
        idx = filho;
    }
}

void inserirHeap(Heap* h, int valor) {
    h->dados[h->tamanho++] = valor;
    subir(h, h->tamanho - 1);
}

int removerHeap(Heap* h) {
    if (h->tamanho == 0) return -1;
    int raiz = h->dados[0];
    h->dados[0] = h->dados[--h->tamanho];
    descer(h, 0);
    return raiz;
}

// ======================== MAIN ==========================

int main() {
    srand(time(NULL));
    FilaLista* filaLista = criarFilaLista();
    Heap* heap = criarHeap(MAX);

    FILE* f_insercao = fopen("dados_insercao.txt", "w");
    FILE* f_remocao = fopen("dados_remocao.txt", "w");
    if (!f_insercao || !f_remocao) {
        printf("Erro ao abrir os arquivos!\n");
        return 1;
    }

    fprintf(f_insercao, "Valor,Comparacoes_Fila,Comparacoes_Heap\n");
    fprintf(f_remocao, "Valor,Comparacoes_Fila,Comparacoes_Heap\n");

    // Inserir 500 elementos
    for (int i = 0; i < 500; i++) {
        int val = rand() % 10000;
        inserirLista(filaLista, val);
        inserirHeap(heap, val);
        fprintf(f_insercao, "%d,%d,%d\n", val, filaLista->comparacoes, heap->comparacoes);
    }

    // Remover 200 elementos
    for (int i = 0; i < 200; i++) {
        int val_lista = removerLista(filaLista);
        int val_heap = removerHeap(heap);
        fprintf(f_remocao, "%d,%d,%d\n", val_lista, filaLista->comparacoes, heap->comparacoes);
    }

    fclose(f_insercao);
    fclose(f_remocao);

    printf("Arquivos dados_insercao.txt e dados_remocao.txt gerados!\n");
    return 0;
}
