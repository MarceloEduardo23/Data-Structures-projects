#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 5
#define HEIGHT 5
#define MAX_NODES 1000

typedef struct Node {
    int x, y;       // Coordenadas na grade
    int g, h, f;    // g : custo do iníco até o nó
                    // h : estimativa até  o destino (função heurística)
                    // f = g + h
    struct Node* parent;  // Ponteiro para o nó anterior no caminho
    int in_open;           // Marcam se o nó está na openList ou closedList
    int in_closed;
} Node;

int heuristic(Node* a, Node* b) {
    return abs(a->x - b->x) + abs(a->y - b->y); // Utiliza da distâcia de Manhattan para calcular o melhor caminho
}

int distance(Node* a, Node* b) {  // O peso das arestas entre os nós (seria legal colocar pesos diferentes)
    return 1; 
}

void reconstruct_path(Node* current) { // Caminha de trás para frente (do destino até o início)
    printf("Caminho encontrado:\n");
    while (current != NULL) {
        printf("(%d, %d) <- ", current->x, current->y);
        current = current->parent;
    }
    printf("start\n");
}

void A_Star(Node* start, Node* goal, Node nodes[HEIGHT][WIDTH]) {
    Node* openList[MAX_NODES];
    int openCount = 0;
    // Iniciando os dados de busca
    start->g = 0;
    start->h = heuristic(start, goal);
    start->f = start->g + start->h;
    start->parent = NULL;
    start->in_open = 1;

    openList[openCount++] = start;

    while (openCount > 0) {
        // Encontrar nó com menor f
        int bestIdx = 0;
        for (int i = 1; i < openCount; i++) {
            if (openList[i]->f < openList[bestIdx]->f)
                bestIdx = i;
        }

        Node* current = openList[bestIdx];

        if (current->x == goal->x && current->y == goal->y) { // Se for caminho reconstroi o caminho da open list
            reconstruct_path(current);
            return;
        }

        // Remove current da openList
        for (int i = bestIdx; i < openCount - 1; i++)
            openList[i] = openList[i + 1];
        openCount--;
        current->in_open = 0;
        current->in_closed = 1;

        // define o deslocamento para 4 vizinho ortogonais (não conta as diagonais)
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};

        for (int d = 0; d < 4; d++) {
            int nx = current->x + dx[d];
            int ny = current->y + dy[d];

            if (nx < 0 || ny < 0 || nx >= WIDTH || ny >= HEIGHT)
                continue;

            Node* neighbor = &nodes[ny][nx];

            if (neighbor->in_closed)
                continue;

            int tentative_g = current->g + distance(current, neighbor);

            if (!neighbor->in_open) {
                openList[openCount++] = neighbor;
                neighbor->in_open = 1;
            } else if (tentative_g >= neighbor->g) {
                continue;
            }

            neighbor->parent = current;
            neighbor->g = tentative_g;
            neighbor->h = heuristic(neighbor, goal);
            neighbor->f = neighbor->g + neighbor->h;
        }
    }

    printf("Nenhum caminho encontrado.\n");
}

int main() {
    Node nodes[HEIGHT][WIDTH] = {0};

    // Inicialização dos nós
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            nodes[y][x].x = x;
            nodes[y][x].y = y;
            nodes[y][x].g = 99999;
            nodes[y][x].f = 99999;
            nodes[y][x].in_open = 0;
            nodes[y][x].in_closed = 0;
            nodes[y][x].parent = NULL;
        }
    }

    // Obstáculos (podemos modficar de arcodo com o nosso puzzle)
    nodes[1][1].in_closed = 1;
    nodes[2][1].in_closed = 1;
    nodes[3][1].in_closed = 1;

    Node* start = &nodes[0][0];
    Node* goal = &nodes[4][4];

    A_Star(start, goal, nodes); // Execução do algoritmo 

    return 0;
}
