#include "structs.h"

/**
 * @brief Cria uma árvore binária de decisão para as variáveis.
 * 
 * Esta função cria recursivamente uma árvore binária para representar as variáveis de uma fórmula CNF,
 * com os valores de estado das variáveis inicialmente definidos como UNDEF (indefinido).
 * A árvore é construída de forma que as variáveis de um nível da árvore correspondem àquelas com
 * valores verdadeiros e falsos nos filhos esquerdo e direito, respectivamente.
 * 
 * @param raiz Ponteiro para a raiz da árvore binária.
 * @param var Número da variável atual.
 * @param max_vars Número máximo de variáveis a serem representadas na árvore.
 * 
 * @return Ponteiro para a raiz da árvore binária criada.
 */
ARVORE *criar_arvore_binaria(ARVORE *raiz, int var, int max_vars){
    if(var > max_vars){
        return NULL;
    }

    raiz = (ARVORE*)malloc(sizeof(ARVORE));

    raiz->var = var;
    raiz->estado = UNDEF;
    raiz->esquerda = criar_arvore_binaria(NULL, var + 1, max_vars);
    raiz->direita = criar_arvore_binaria(NULL, var + 1, max_vars);

    return raiz;
}

/**
 * @brief Adiciona um literal (variável ou seu complemento) à lista encadeada.
 * 
 * Esta função cria um novo nó na lista de literais e o adiciona ao final da lista encadeada
 * de literais, retornando o ponteiro atualizado para a lista.
 * 
 * @param no Ponteiro para o primeiro nó da lista de literais.
 * @param var Literal a ser adicionado (representado como um número inteiro).
 * 
 * @return Ponteiro para o primeiro nó da lista de literais após a adição.
 */
NO *adicionar_literal(NO *no, int var){
    NO *novo = (NO*)malloc(sizeof(NO));
    
    novo->item = var;
    novo->prox = NULL;

    if(no == NULL){
        return novo;
    }

    NO *aux = no;
    while(aux->prox)
        aux = aux->prox;
    
    aux->prox = novo;
    return no;
}

/**
 * @brief Adiciona uma cláusula à lista de cláusulas da fórmula.
 * 
 * Esta função cria um novo nó de cláusula e o adiciona ao final da lista de cláusulas da fórmula,
 * retornando o ponteiro atualizado para a lista de cláusulas.
 * 
 * @param head Ponteiro para o primeiro nó da lista de cláusulas.
 * @param lit Ponteiro para a lista de literais da cláusula.
 * 
 * @return Ponteiro para o primeiro nó da lista de cláusulas após a adição.
 */
CLAUSULA *adicionar_clausula(CLAUSULA *head, NO *lit){
    CLAUSULA *nova = (CLAUSULA*)malloc(sizeof(CLAUSULA));

    nova->literais = lit;
    nova->prox = NULL;

    if(head == NULL) return nova;

    CLAUSULA *aux = head;
    while (aux->prox)
        aux = aux->prox;

    aux->prox = nova;
    return head;
}

/**
 * @brief Lê uma fórmula CNF a partir de um arquivo.
 * 
 * Esta função lê uma fórmula no formato CNF de um arquivo e armazena as cláusulas e literais
 * na estrutura `FORMULA`, construindo a lista de cláusulas e as listas encadeadas de literais.
 * 
 * @param arquivo Ponteiro para o arquivo de onde a fórmula CNF será lida.
 * 
 * @return A fórmula CNF lida e armazenada em uma estrutura `FORMULA`.
 */
FORMULA ler_formula(FILE *arquivo){
    FORMULA formula;
    formula.inicio = NULL;
    formula.num_sentencas = 0;
    formula.num_var = 0;

    char linha[MAX];

    while(fgets(linha, MAX, arquivo)){
        if(linha[0] == 'c')
            continue;
        if(linha[0] == 'p'){
            sscanf(linha, "p cnf %d %d", &formula.num_var, &formula.num_sentencas);
            continue;
        }

        int lit;
        NO *lit_head = NULL;

        char *token = strtok(linha, " \n");
        while(token){
            lit = atoi(token);

            if(lit == 0){
                formula.inicio = adicionar_clausula(formula.inicio, lit_head);
                lit_head = NULL;
            }else{
                lit_head = adicionar_literal(lit_head, lit);
            }
            token = strtok(NULL, " \n");
        }
    }
    return formula;
}

/**
 * @brief Imprime a atribuição de valores das variáveis.
 * 
 * Esta função percorre a árvore binária e imprime os valores atribuídos a cada variável,
 * ou "UNDEF" caso a variável ainda não tenha sido atribuída.
 * 
 * @param raiz Ponteiro para a raiz da árvore binária de decisão.
 * @param num_vars Número total de variáveis na fórmula CNF.
 */
void imprimir_atribuicao(ARVORE *raiz, int num_vars){
    printf("\nAtribuicao de variaveis:\n");
    char **valores = (char**)malloc((num_vars + 1) * sizeof(char*));
    for(int i = 1; i <= num_vars; i++)
        valores[i] = "UNDEF";
    
    ARVORE *atual = raiz;
    while(atual && atual->var <= num_vars){
        if(atual->estado == TRUE){
            valores[atual->var] = "TRUE";
            atual = atual->esquerda;
        }else if (atual->estado == FALSE){
            valores[atual->var] = "FALSE";
            atual = atual->direita;
        }else{
            valores[atual->var] = "UNDEF";
            break;
        }
    }

    for(int i = 1; i <= num_vars; i++){
        printf("x%d = %s\n", i, valores[i]);
    }

    free(valores);
}

/**
 * @brief Interpreta o valor de um literal (variável ou seu complemento) na árvore.
 * 
 * Esta função verifica o estado do literal (variável ou seu complemento) na árvore binária
 * de decisão e retorna seu valor correspondente (TRUE, FALSE ou UNDEF).
 * 
 * @param raiz Ponteiro para a raiz da árvore binária de decisão.
 * @param var Literal a ser interpretado (representado como um número inteiro).
 * 
 * @return O estado do literal (TRUE, FALSE ou UNDEF).
 */
ESTADO interpretacao_parcial(ARVORE *raiz, int var){
    int var_abs = abs(var);
    ARVORE *atual = raiz;

    while(atual){
        if(atual->var == var_abs){
            if(atual->estado == UNDEF)
                return UNDEF;
            else
                return (var > 0) ? atual->estado : (atual->estado == TRUE ? FALSE : TRUE);
        }

        if(atual->estado == TRUE)
            atual = atual->esquerda;
        else if(atual->estado == FALSE)
            atual = atual->direita;
        else
            return UNDEF;
    }

    return UNDEF;
}

/**
 * @brief Avalia o valor de uma cláusula na fórmula CNF.
 * 
 * Esta função avalia a cláusula dada com base na interpretação das variáveis na árvore
 * binária de decisão, retornando se a cláusula é satisfeita (TRUE), insatisfatória (FALSE)
 * ou indefinida (UNDEF).
 * 
 * @param clausula Ponteiro para a cláusula a ser avaliada.
 * @param raiz Ponteiro para a raiz da árvore binária de decisão.
 * 
 * @return O estado da cláusula (TRUE, FALSE ou UNDEF).
 */
ESTADO avaliar_clausula(CLAUSULA *clausula, ARVORE *raiz){
    bool tem_undef = false;
    NO *lit = clausula->literais;

    while(lit){
        ESTADO valor_lit = interpretacao_parcial(raiz, lit->item);

        if(valor_lit == TRUE)
            return TRUE;
        if(valor_lit == UNDEF)
            tem_undef = true;
        
        lit = lit->prox;
    }

    if(tem_undef)
        return UNDEF;
    else
        return FALSE;
}

/**
 * @brief Avalia a fórmula CNF com base na árvore binária de decisão.
 * 
 * Esta função avalia a fórmula CNF, verificando se todas as cláusulas são satisfeitas.
 * Se alguma cláusula não for satisfeita, a fórmula é insatisfatória (FALSE), caso contrário,
 * a fórmula é satisfatória (TRUE). Se alguma cláusula não puder ser avaliada, a fórmula é
 * indefinida (UNDEF).
 * 
 * @param formula A fórmula CNF a ser avaliada.
 * @param raiz Ponteiro para a raiz da árvore binária de decisão.
 * 
 * @return O estado da fórmula (TRUE, FALSE ou UNDEF).
 */
ESTADO avaliar_formula(FORMULA formula, ARVORE *raiz){
    bool tem_undef = false;
    CLAUSULA *clausula = formula.inicio;

    while(clausula){
        ESTADO valor_cl = avaliar_clausula(clausula, raiz);

        if(valor_cl == FALSE)
            return FALSE;
        if(valor_cl == UNDEF)
            tem_undef = true;
        
        clausula = clausula->prox;
    }

    if(tem_undef)
        return UNDEF;
    else{
        return TRUE;
    }
}

/**
 * @brief Resolve o problema SAT usando uma árvore binária de decisão.
 * 
 * Esta função tenta encontrar uma atribuição de valores para as variáveis que satisfaça
 * a fórmula CNF. A busca é feita de forma recursiva, atribuindo valores TRUE ou FALSE
 * às variáveis até encontrar uma solução ou determinar que não há solução.
 * 
 * @param raiz Ponteiro para a raiz da árvore binária de decisão.
 * @param formula A fórmula CNF a ser resolvida.
 * 
 * @return Verdadeiro se a fórmula for satisfatória, falso caso contrário.
 */
bool sat(ARVORE *raiz, FORMULA formula){
    ESTADO resultado = avaliar_formula(formula, raiz);

    if(resultado == TRUE)
        return true;
    if(resultado == FALSE)
        return false;
    
    ARVORE *atual = raiz;
    while(atual && atual->estado != UNDEF){
        if(atual->estado == TRUE)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    if(!atual) return false;

    atual->estado = TRUE;
    if(sat(raiz, formula)) return true; 

    atual->estado = FALSE;
    if(sat(raiz, formula)) return true;

    atual->estado = UNDEF;
    return false;
}

/**
 * @brief Libera a memória alocada para a lista de literais.
 * 
 * Esta função percorre a lista de literais e libera a memória alocada para cada nó.
 * 
 * @param no Ponteiro para o primeiro nó da lista de literais.
 */
void liberar_literais(NO *no){
    NO *atual = no;
    while(atual){
        NO *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

/**
 * @brief Libera a memória alocada para a fórmula CNF.
 * 
 * Esta função libera a memória alocada para todas as cláusulas e literais da fórmula CNF.
 * 
 * @param formula A fórmula CNF a ser liberada.
 */
void liberar_formula(FORMULA formula) {
    CLAUSULA *atual = formula.inicio;
    while (atual) {
        CLAUSULA *proxima = atual->prox;
        liberar_literais(atual->literais);
        free(atual);
        atual = proxima;
    }
}

/**
 * @brief Libera a memória alocada para a árvore binária de decisão.
 * 
 * Esta função libera a memória alocada para cada nó da árvore binária de decisão, realizando
 * uma busca recursiva e liberando a memória das subárvores à esquerda e direita de cada nó.
 * 
 * @param raiz Ponteiro para a raiz da árvore binária.
 */
void liberar_arvore(ARVORE *raiz) {
    if (!raiz)  return;
    
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}
