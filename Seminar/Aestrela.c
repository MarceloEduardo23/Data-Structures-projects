#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <string.h> 
#include <stdbool.h>
#include <limits.h>

#define ALTURA 5
#define LARGURA 5
#define MAX_NODES (ALTURA*LARGURA)

int TERRENO[ALTURA][LARGURA] = {
          {1, 1, 1, 1, 1},
          {1, 4, 4, 1, 1},
          {1, 4, 1, 1, 1},
          {1, 1, 1, 3, 3},
          {1, 1, 1, 1, 1},
};

/* 1 = obstáculo (muro), 0 = livre */
int OBSTACULO[ALTURA][LARGURA] = {
          {0, 1, 0, 0, 0},
          {0, 1, 0, 1, 0},
          {0, 1, 0, 1, 0},
          {0, 0, 0, 1, 0},
          {0, 0, 0, 0, 0},
};

typedef struct node {
    int x , y ;
    int f, g , h; 
    int custo;
    struct node *next; 
    int closed , open ;
}NODE;
int heuristic (NODE *a, NODE *b){
    return abs(a->x - b->x) + abs(a->y - b->y);
}
int distancia(NODE *de, NODE *ate){
    return ate->custo;
}
void imprimir_caminho(NODE *current){
    printf("Caminho encontrado:\n");
    while (current){
        printf("(%d,%d) ", current->x, current->y);
        current = current->next;
        if(current){
            printf("<- ");
        }
    }
    printf("\n");
}
void A_star(NODE *start, NODE *goal, NODE nodes[ALTURA][LARGURA]){
    NODE *aberto[MAX_NODES];
    int count_aberto = 0;

    start->g = 0 ;
    start->h = heuristic(start, goal);
    start->f = start->g + start->h;
    start->open = 1;
    aberto[count_aberto++] = start;

    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    while (count_aberto){
        int  best = 0;
        for (int i = 1; i < count_aberto; i++){
            if(aberto[i]->f < aberto[best]->f) best = i;
        }

        NODE *current = aberto[best];

        if (current == goal){
            imprimir_caminho(current);
            return;
        }

        aberto[best] = aberto[--count_aberto];
        current->open = 0;
        current->closed = 1;

        for (int j = 0 ; j < 4; ++j){
            int nx = current->x + dx[j];
            int ny = current->y + dy[j];

            if (nx < 0 || ny < 0 || nx >= LARGURA || ny >= ALTURA){
                continue;
            }

            NODE *nbr = &nodes[ny][nx];
            if(nbr->closed) continue;

            int tentativa_g = current->g + distancia(current, nbr);
            if (!nbr->open){
                aberto[count_aberto++] = nbr;
                nbr->open = 1;
            }
            else if (tentativa_g >= nbr->g){
                continue;
            }

            nbr->next = current;
            nbr->g = tentativa_g;
            nbr->h = heuristic(nbr, goal);
            nbr->f = nbr->g + nbr->h;
        }
    }
    puts("Nenhum caminho encontrado.");
}
int main (){
    NODE nodes[ALTURA][LARGURA] = {0};

    for (int y = 0 ; y < ALTURA; ++y){
        for(int x = 0; x < LARGURA; ++x){
            NODE *n = &nodes[y][x];
            n->x = x;
            n->y = y;
            n->g = INT_MAX/2;
            n->f = INT_MAX/2;
            n->custo = TERRENO[x][y];
            n->open = n->closed = 0;
            n->next = NULL;
            if(OBSTACULO[y][x]) n->closed = 1;
        }
    }
    NODE *start = &nodes[0][0];
    NODE *goal = &nodes[4][4];

    A_star(start, goal, nodes);
    return 0; 
}
