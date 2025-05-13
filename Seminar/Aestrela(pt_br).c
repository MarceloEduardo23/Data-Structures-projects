#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Tamanho do mapa (grade)
#define LARGURA 5
#define ALTURA 5
#define MAX_ABERTOS 1000 // Tamanho máximo da lista de nós a serem explorados

// Estrutura que representa cada posição no mapa
typedef struct No {
    int x, y;         // Coordenadas (coluna x, linha y)
    int g, h, f;      // g: custo desde o início até aqui
                      // h: estimativa até o objetivo
                      // f: custo total estimado (g + h)
    struct No* pai;   // Aponta para o nó anterior (para reconstruir o caminho)
    int esta_aberto;  // 1 se está na lista de abertos
    int esta_fechado; // 1 se já foi explorado (lista fechada)
} No;

// Função que estima a distância até o objetivo (heurística)
// Aqui usamos a "distância de Manhattan", ideal para mapas em grade
int calcular_heuristica(No* atual, No* objetivo) {
    return abs(atual->x - objetivo->x) + abs(atual->y - objetivo->y);
}

// Função que define o custo real de se mover de um ponto a outro
// Neste exemplo, todo movimento custa 1
int calcular_distancia(No* a, No* b) {
    return 1;
}

// Imprime o caminho encontrado, do objetivo até o início
void mostrar_caminho(No* atual) {
    printf("Caminho encontrado:\n");
    while (atual != NULL) {
        printf("(%d, %d) <- ", atual->x, atual->y);
        atual = atual->pai;
    }
    printf("início\n");
}

// Algoritmo principal A* (A-estrela)
void A_Estrela(No* inicio, No* objetivo, No mapa[ALTURA][LARGURA]) {
    No* abertos[MAX_ABERTOS]; // Lista de nós a serem explorados
    int total_abertos = 0;

    // Inicializando o nó de início
    inicio->g = 0;
    inicio->h = calcular_heuristica(inicio, objetivo);
    inicio->f = inicio->g + inicio->h;
    inicio->pai = NULL;
    inicio->esta_aberto = 1;
    abertos[total_abertos++] = inicio;

    // Enquanto ainda há nós a serem explorados
    while (total_abertos > 0) {
        // Escolher o nó com menor f (mais promissor)
        int melhor_indice = 0;
        for (int i = 1; i < total_abertos; i++) {
            if (abertos[i]->f < abertos[melhor_indice]->f)
                melhor_indice = i;
        }

        No* atual = abertos[melhor_indice];

        // Se chegamos ao destino, mostramos o caminho e encerramos
        if (atual->x == objetivo->x && atual->y == objetivo->y) {
            mostrar_caminho(atual);
            return;
        }

        // Remover da lista de abertos
        for (int i = melhor_indice; i < total_abertos - 1; i++)
            abertos[i] = abertos[i + 1];
        total_abertos--;

        atual->esta_aberto = 0;
        atual->esta_fechado = 1;

        // Movimentos possíveis (cima, direita, baixo, esquerda)
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};

        for (int d = 0; d < 4; d++) {
            int novo_x = atual->x + dx[d];
            int novo_y = atual->y + dy[d];

            // Ignorar se estiver fora do mapa
            if (novo_x < 0 || novo_y < 0 || novo_x >= LARGURA || novo_y >= ALTURA)
                continue;

            No* vizinho = &mapa[novo_y][novo_x];

            // Ignorar se for obstáculo ou já visitado
            if (vizinho->esta_fechado)
                continue;

            // Custo para chegar até o vizinho
            int g_temporario = atual->g + calcular_distancia(atual, vizinho);

            // Se o vizinho ainda não está na lista de abertos, adiciona
            if (!vizinho->esta_aberto) {
                abertos[total_abertos++] = vizinho;
                vizinho->esta_aberto = 1;
            }
            // Se o novo caminho não é melhor, ignora
            else if (g_temporario >= vizinho->g) {
                continue;
            }

            // Atualiza os dados do vizinho
            vizinho->pai = atual;
            vizinho->g = g_temporario;
            vizinho->h = calcular_heuristica(vizinho, objetivo);
            vizinho->f = vizinho->g + vizinho->h;
        }
    }

    // Se terminou sem encontrar o caminho
    printf("Nenhum caminho encontrado.\n");
}

int main() {
    // Criamos o mapa (uma grade 5x5)
    No mapa[ALTURA][LARGURA] = {0};

    // Inicializando todos os nós do mapa
    for (int y = 0; y < ALTURA; y++) {
        for (int x = 0; x < LARGURA; x++) {
            mapa[y][x].x = x;
            mapa[y][x].y = y;
            mapa[y][x].g = 99999; // valor inicial bem alto
            mapa[y][x].f = 99999;
            mapa[y][x].esta_aberto = 0;
            mapa[y][x].esta_fechado = 0;
            mapa[y][x].pai = NULL;
        }
    }

    // Definimos alguns obstáculos (muros que não podem ser atravessados)
    mapa[1][1].esta_fechado = 1;
    mapa[2][1].esta_fechado = 1;
    mapa[3][1].esta_fechado = 1;

    // Definimos o ponto de partida e o destino
    No* inicio = &mapa[0][0];      // canto superior esquerdo
    No* objetivo = &mapa[4][4];    // canto inferior direito

    // Executamos o algoritmo
    A_Estrela(inicio, objetivo, mapa);

    return 0;
}
