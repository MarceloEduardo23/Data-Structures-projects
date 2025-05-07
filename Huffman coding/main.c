#include "bibliotecas/huffman.h"

void compactar(char *caminho, char *nome_arquivo){
    /***** PARTE 1: LER ARQUIVO E GERAR A TABELA DE FREQUÊNCIA *****/
    FILE *arquivo_entrada = fopen(caminho, "rb");
    if(!arquivo_entrada){
        printf("\n\tERRO AO ABRIR ARQUIVO ENTRADA.\n");
        return;
    }

    unsigned long tam_arq = tamanho_arquivo(arquivo_entrada);
    printf("\n\tTAMANHO DO ARQUIVO ORIGINAL: %ld bytes\n", tam_arq);

    unsigned long *frequencia = contar_frequencia(arquivo_entrada, tam_arq);
    
    /***** PASSO 2: GERAR FILA DE PRIORIDADE *****/
    LISTA fila;
    criar_fila(&fila);

    preencher_fila(frequencia, &fila);

    /***** PASSO 3: GERAR ÁRVORE DE HUFFMAN *****/
    NOHUFF *arvore = montar_arvore(&fila);

    /***** PASSO 4: GERAR DICIONARIO *****/
    unsigned int tam_max = altura_arvore(arvore) + 1;
    unsigned char **dicionario = malloc(sizeof(unsigned char*) * TAM_ASCII);

    for(int i = 0; i < TAM_ASCII; i++)
        dicionario[i] = calloc(tam_max, sizeof(unsigned char));

    gerar_dicionario(dicionario, arvore, "", tam_max);
    
    /***** PARTE 5: GERAR ARQUIVO COMPACTADO *****/
    FILE *arquivo_saida = fopen(nome_arquivo, "wb");
    if(!arquivo_saida){
        printf("\n\tERRO AO CRIAR ARQUIVO SAIDA");
        return;
    }

    fwrite("AB", sizeof(unsigned char), 2, arquivo_saida);

    short tam_arvore = salvar_arvore(arvore, arquivo_saida);
    printf("\n\tTAMANHO ARVORE: %d", tam_arvore);

    short tam_lixo = salvar_dados(arquivo_entrada, arquivo_saida, dicionario, tam_arq, tam_arvore);
    printf("\n\tTAMANHO LIXO: %d", tam_lixo);

    salvar_cabecalho(arquivo_saida, tam_lixo, tam_arvore);

    fclose(arquivo_saida);
}

int main(){
    int escolha;

    SetConsoleOutputCP(65001);

    printf("\n\t=== COMPRESSOR HUFFMAN ===\n");
    printf("\n\tDigite uma opcao:");
    printf("\n\t1 - Compactar\n\t2 - Descompactar\n\t0 - Sair\n\n\tescolha: ");
    scanf("%d", &escolha);
    getchar(); // Remove o '\n' do texto

    switch (escolha){
    case 1:{
        printf("\n\tDIGITE O CAMINHO COMPLETO DO ARQUIVO QUE DESEJA ABRIR: ");

        char caminho[MAX_LEITURA];

        if(!fgets(caminho, MAX_LEITURA, stdin)){ //CASO DE ALGUM BUG
            printf("\n\tERRO: FALHA AO LER A ENTRADA.\n");
            break;
        }
        // Verifica se o caminho está vazio
        if (strlen(caminho) == 0) {
            printf("\n\tERRO: CAMINHO VAZIO.\n");
            break;
        }

        char nome_arquivo[MAX_LEITURA];

        printf("\n\tDIGITE O NOME PARA O ARQUIVO COMPACTADO: ");
        if(!fgets(nome_arquivo, MAX_LEITURA, stdin)){ //CASO DE ALGUM BUG
            printf("\n\tERRO: FALHA AO LER A NOME DO ARQUIVO.\n");
            break;
        }
        // Verifica se o caminho está vazio
        if (strlen(nome_arquivo) == 0) {
            printf("\n\tERRO: NOME DO ARQUIVO VAZIO.\n");
            break;
        }

        caminho[strcspn(caminho, "\n")] = '\0'; // REMOVE O '\n' do fim do texto caminho 
        nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0'; 
        strcat(nome_arquivo, ".huff");

        compactar(caminho, nome_arquivo);

        break;
    }/*
    case 2:{
        printf("\n\tDIGITE O CAMINHO COMPLETO DO ARQUIVO QUE DESEJA ABRIR: ");

        char caminho[MAX_LEITURA];

        if(!fgets(caminho, MAX_LEITURA, stdin)){ //CASO DE ALGUM BUG
            printf("\n\tERRO: FALHA AO LER A ENTRADA.huff.\n");
            break;
        }
        // Verifica se o caminho está vazio
        if (strlen(caminho) == 0) {
            printf("\n\tERRO: CAMINHO VAZIO.\n");
            break;
        }

        char nome_arquivo[MAX_LEITURA];

        printf("\n\tDIGITE O NOME PARA O ARQUIVO DESCOMPACTADO (COM SUA EXTENÇÃO): ");
        if(!fgets(nome_arquivo, MAX_LEITURA, stdin)){ //CASO DE ALGUM BUG
            printf("\n\tERRO: FALHA AO LER A NOME DO ARQUIVO.\n");
            break;
        }
        // Verifica se o caminho está vazio
        if (strlen(nome_arquivo) == 0) {
            printf("\n\tERRO: NOME DO ARQUIVO VAZIO.\n");
            break;
        }

        caminho[strcspn(caminho, "\n")] = '\0'; // REMOVE O '\n' do fim do texto caminho 
        nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0'; 

        descompactar(caminho, nome_arquivo);

        break;
    }*/
    
    default:
        printf("\n\tOPÇÃO INVÁLIDA!");
        break;
    }

    printf("\n\tSaindo...");

    return 0;
}