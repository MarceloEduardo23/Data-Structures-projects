#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <stdbool.h> 
#define MAX 100 

//---------Árvore-----------
typedef struct tree{
    int var ; // Varialvel
    bool result ; // Resultado da interpretação parcial
    struct tree *direita; // Nó para a direita FALSE 
    struct tree *esquerda; // Nó para a esquerda TRUE
}tree;
//----- Nos ------
typedef struct node {
    int item;
    struct node *next;
}node;
//---- Literais -----
typedef struct clausula{
    node *literias; // Literias x1 , x2 ,x3 ..... e já vê se ele está negado
    struct clausula *next; // Ponteiro para o próximo nó de literais
}clausula;
//---- representação da formula ------
typedef struct formula{
    int num_variaveis; // Número de variáveis 
    int num_setencas ; // Número de seteças
    clausula *inicio ;    // lista de cláusulas
}formula;

node *creat_linked_list (){ // Inicializa a lista encadeada
    return NULL;
}
//------- Criando uma arvore-------
tree *creat_binary_tree ( int var, int max_vars){
    if (var > max_vars){ // Caso o número de var for maior do que aquele fornecido pelo o arquivo
        return NULL;
    }
    tree *root = (tree*) malloc(sizeof(tree));
    root->var = var;
    // Filho esquerda (TRUE)
    root->esquerda = creat_binary_tree( var + 1, max_vars); // Var começa com 0  e vai aumentando de pouco a pouco até chegar no limite

    // Filho direito (FALSE)
    root->direita = creat_binary_tree( var + 1, max_vars);
    return root;
}
//------Adiciona elementos aos literais--------
node *add_literal (node *head, int var){
    node *new_node = (node*)malloc(sizeof(node));
    new_node->item = var ;
    new_node->next = NULL;
    
    // Isso aqui tudo ésó para adicionar na ordem que tá no arquivo
    if (head == NULL){
        return new_node;
    }
    node *temp = head; 
    while (temp->next != NULL){
        temp = temp->next;
    }
    temp->next = new_node;
    return head;
}
//---------Adiciona elementos as cláusulas--------
clausula *add_clausula (clausula *head, node *lit){
    clausula *new_clausula = (clausula*)malloc(sizeof(clausula));
    new_clausula->literias = lit;
    new_clausula->next = NULL; // Não tem mais nenhuma cláusula a ser adicionada na lista ; Por enquanto

    if (head == NULL){ // Não tem nenhum elemento anterior 
        return new_clausula; // Retorna a lista de cláusula atual
    }
    clausula *temp = head;

    while (temp->next != NULL){ // Caso tenha algum elemento, pecorremos até o ultimo nó
        temp = temp->next ;
    }
    temp->next = new_clausula; // Adicionamos a novo cláusula no fim da lista 
    return head ; 
}
//------Leitura do arquivo .cnf--------
formula read_formula (FILE *fp){
    formula F; 
    F.inicio = NULL;
    F.num_setencas = 0;
    F.num_variaveis = 0;

    char line[MAX];

    while (fgets(line, MAX, fp)){
        if (line[0] == 'c'){ // ignora comentários
            continue;
        }
        if (line[0] == 'p'){ // Lê cabeçalho
            sscanf(line, "p cnf %d %d", &F.num_variaveis, &F.num_setencas);
            continue;
        }

        int lit; 
        node *lit_head = creat_linked_list();

        char *token = strtok(line, " \n");
        while (token != NULL){
            lit = atoi(token);
            if (lit == 0){
                F.inicio = add_clausula(F.inicio, lit_head);
                lit_head = NULL;
            }
            else {
                lit_head = add_literal(lit_head, lit);
            }
            token = strtok(NULL, " \n");
        }
    }
    return F;
}
bool eh_sat (formula *F, bool *interpretacoes){ // Acessar os literais, os modificando e  a formula e atraibuindo a um array de booleanos
    clausula *cl = F->inicio;
    while (cl != NULL){  //Ver todas as cláusulas
        node *lt = cl->literias; 
        bool cl_sat = false; // Assumimos que ela não é sat até ser provado o contrário
        while (lt != NULL){ // Ver os literias x1 x2 ....
            int var = lt->item;
            bool valor;
            if (var > 0){ // Se  não tiver negado
               valor = interpretacoes[var - 1] ; // Pegamos o valor no array de atribuição // Reolhar "interpretacoes[var - 1]""
            }
            else { // Caso não
                valor = !interpretacoes[-var - 1]; // Vamos negar o valor de atribuição
            }
            if (valor){ // Se o valor é verdadeiro
                cl_sat = true;
                break;
            }
            lt = lt->next;
        }
        if(!cl_sat){ // Se a cláusula for falsa
            return false;
        }
        cl = cl->next;
    }
    return true;
}
bool SAT_SOLVER (tree *root, formula *F, bool *interpretacoes, int nivel){
    if (root == NULL){
        return eh_sat(F, interpretacoes);
    }
    interpretacoes[nivel] = true ; // True para a variavel atual
    if(SAT_SOLVER(root->esquerda, F, interpretacoes, nivel + 1)){
        return true;
    }
    interpretacoes[nivel] = false;
    if (SAT_SOLVER(root->direita, F, interpretacoes, nivel + 1)){
        return true;
    }
    return false ;
}
void solucao (bool *interpretacao, int num_var){
    printf("SAT !\n");
    printf("Solucoes :\n");
    for (int i = 0; i < num_var; i++){
        printf("x%d = %s\n", i + 1, interpretacao[i] ? "TRUE" : "FALSE");
    }
}
int main (){
    FILE *fp = fopen("teste6.cnf", "r");
    if (fp == NULL){
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    formula F = read_formula(fp);
    fclose(fp);

    tree *root = creat_binary_tree(1, F.num_variaveis);
    bool *interpretacao = (bool*)malloc(F.num_variaveis * sizeof(bool));

    if (SAT_SOLVER(root, &F, interpretacao, 0)){
        solucao(interpretacao, F.num_variaveis);
    }
    else {
        printf("UNSAT!\n");
    }
    free(interpretacao);
    return 0;
}
