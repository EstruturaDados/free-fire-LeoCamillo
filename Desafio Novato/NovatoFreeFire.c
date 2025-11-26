#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Necessário para strcpy, strcmp, strcspn

// ---------------------------------------------
// REQUISITO 1: Criação da Struct Item
// ---------------------------------------------

// Define a capacidade máxima da mochila (Lista Sequencial Estática)
#define CAPACIDADE_MAXIMA 10 
// Define o tamanho máximo de caracteres para o nome do item
#define NOME_MAX 30
// Define o tamanho máximo de caracteres para o tipo do item
#define TIPO_MAX 20 

// Estrutura que representa um item dentro da mochila
typedef struct {
    char nome[NOME_MAX];       // Nome do item (ex: KitMedico, Municao)
    char tipo[TIPO_MAX];       // Tipo do item (ex: cura, municao, arma)
    int quantidade;            // Quantidade deste item
} Item;

// ---------------------------------------------
// REQUISITO 2: Vetor de Structs e Variáveis de Controle
// ---------------------------------------------

// A mochila é representada por um vetor de structs Item.
Item mochila[CAPACIDADE_MAXIMA];

// Variável de controle: rastreia quantos itens (slots) estão ocupados na mochila.
int totalItens = 0; 

// ---------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES OBRIGATÓRIAS
// ---------------------------------------------

void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void limparBuffer();
void pausarSistema();

// ---------------------------------------------
// FUNÇÃO PRINCIPAL (main)
// ---------------------------------------------

int main() {
    int opcao;

    // A mochila começa vazia (totalItens = 0)
    printf("--- INICIANDO JOGO: CODIGO DA ILHA ---\n");
    
    do {
        // --- REQUISITO: Usabilidade (Interface Clara) ---
        printf("\n============================================\n");
        printf("MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
        printf("============================================\n");
        printf("Itens na Mochila: %d/%d\n", totalItens, CAPACIDADE_MAXIMA);
        printf("1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens na Mochila\n");
        printf("4. Buscar Item por Nome\n");
        printf("0. Sair\n");
        printf("--------------------------------------------\n");
        printf("Escolha uma opcao: ");

        // Leitura da opção
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Garante que caia no default em caso de erro na leitura
        }
        limparBuffer(); // Limpa o buffer após scanf

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                pausarSistema();
                break;
            case 4:
                buscarItem();
                pausarSistema();
                break;
            case 0:
                printf("\nFechando inventário. Sobrevivência concluída!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ---------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ---------------------------------------------

/**
 * @brief Limpa o buffer de entrada (stdin) após usar scanf.
 * Essencial para evitar que entradas indesejadas atrapalhem o fgets.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * @brief Pausa o sistema até o usuário pressionar Enter.
 */
void pausarSistema() {
    printf("\n--------------------------------------------\n");
    printf("Pressione Enter para continuar...");
    getchar(); // Espera a tecla Enter
}

/**
 * @brief Cadastra um novo item na mochila (Lista Sequencial).
 */
void inserirItem() {
    // Verifica se a mochila está cheia
    if (totalItens >= CAPACIDADE_MAXIMA) {
        printf("\nERRO: Mochila cheia! (%d/%d) Nao e possivel adicionar mais itens.\n", totalItens, CAPACIDADE_MAXIMA);
        return;
    }

    // --- REQUISITO: Leitura de dados (fgets para strings seguras) ---

    // Posição de inserção é o primeiro índice livre (totalItens)
    Item *novoItem = &mochila[totalItens];

    printf("\n--- ADICIONAR ITEM (LOOT) ---\n");

    // Lendo o Nome
    printf("Nome do Item (max %d): ", NOME_MAX - 1);
    // Uso de fgets para leitura segura e prevenção de buffer overflow
    if (fgets(novoItem->nome, NOME_MAX, stdin) == NULL) return;
    // Remove o caractere de nova linha '\n' adicionado pelo fgets
    novoItem->nome[strcspn(novoItem->nome, "\n")] = 0; 

    // Lendo o Tipo
    printf("Tipo do Item (ex: arma, cura, municao - max %d): ", TIPO_MAX - 1);
    if (fgets(novoItem->tipo, TIPO_MAX, stdin) == NULL) return;
    novoItem->tipo[strcspn(novoItem->tipo, "\n")] = 0;

    // Lendo a Quantidade
    printf("Quantidade: ");
    if (scanf("%d", &novoItem->quantidade) != 1 || novoItem->quantidade <= 0) {
        printf("ERRO: Quantidade invalida. Cancelando insercao.\n");
        limparBuffer(); // Limpa o buffer se o scanf falhar
        return; 
    }
    limparBuffer(); // Limpa o buffer após scanf

    // Incrementa o contador da mochila
    totalItens++;
    printf("\nSUCESSO: Item \"%s\" adicionado a mochila!\n", novoItem->nome);
    listarItens(); // --- REQUISITO: Listar após cada operação ---
    pausarSistema();
}

/**
 * @brief Remove um item da mochila pelo nome (Busca Sequencial e Deslocamento).
 */
void removerItem() {
    if (totalItens == 0) {
        printf("\nERRO: A mochila esta vazia. Nao ha o que remover.\n");
        return;
    }

    char nomeBusca[NOME_MAX];
    int posEncontrada = -1; // Posição do item a ser removido

    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o nome exato do item para remover: ");
    
    // Leitura do nome a ser buscado
    if (fgets(nomeBusca, NOME_MAX, stdin) == NULL) return;
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    // --- REQUISITO: Uso de Laços (for) para percorrer o vetor ---
    for (int i = 0; i < totalItens; i++) {
        // strcmp retorna 0 se as strings forem idênticas
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            posEncontrada = i;
            break; // Item encontrado, para a busca
        }
    }

    if (posEncontrada != -1) {
        printf("\nSUCESSO: Item \"%s\" removido da posicao %d.\n", mochila[posEncontrada].nome, posEncontrada + 1);

        // Deslocamento dos elementos:
        // Move cada item subsequente uma posição para trás, fechando o "buraco".
        for (int i = posEncontrada; i < totalItens - 1; i++) {
            // Copia a struct completa (ou campo por campo)
            mochila[i] = mochila[i+1]; 
        }

        // Decrementa o contador da mochila
        totalItens--;
        listarItens(); // --- REQUISITO: Listar após cada operação ---
        pausarSistema();
    } else {
        printf("\nERRO: Item \"%s\" nao encontrado na mochila.\n", nomeBusca);
    }
}

/**
 * @brief Lista todos os itens e seus dados na mochila.
 */
void listarItens() {
    if (totalItens == 0) {
        printf("\n--- MOCHILA VAZIA ---\n");
        return;
    }

    // --- REQUISITO: Saída formatada como na imagem de exemplo ---
    printf("\n--------------------------------------------\n");
    printf("| %-*s | %-*s | %-*s |\n", NOME_MAX - 1, "NOME", TIPO_MAX - 1, "TIPO", 10, "QUANTIDADE");
    printf("--------------------------------------------\n");

    for (int i = 0; i < totalItens; i++) {
        printf("| %-*s | %-*s | %-10d |\n", 
               NOME_MAX - 1, mochila[i].nome, 
               TIPO_MAX - 1, mochila[i].tipo, 
               mochila[i].quantidade);
    }
    printf("--------------------------------------------\n");
    printf("Total de %d itens diferentes na mochila.\n", totalItens);
}

/**
 * @brief Busca um item pelo nome e exibe seus dados.
 */
void buscarItem() {
    if (totalItens == 0) {
        printf("\nERRO: A mochila esta vazia. Nada para buscar.\n");
        return;
    }

    char nomeBusca[NOME_MAX];
    int posEncontrada = -1;

    printf("\n--- BUSCA SEQUENCIAL ---\n");
    printf("Digite o nome exato do item para buscar: ");
    
    // Leitura do nome a ser buscado
    if (fgets(nomeBusca, NOME_MAX, stdin) == NULL) return;
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    // --- REQUISITO: Busca Sequencial ---
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            posEncontrada = i;
            break;
        }
    }

    if (posEncontrada != -1) {
        Item *item = &mochila[posEncontrada];
        printf("\n--- ITEM ENCONTRADO ---\n");
        printf("Nome: %s\n", item->nome);
        printf("Tipo: %s\n", item->tipo);
        printf("Quantidade: %d\n", item->quantidade);
        printf("Posicao no Inventario (Indice): %d\n", posEncontrada);
    } else {
        printf("\nRESULTADO: Item \"%s\" nao foi encontrado na mochila.\n", nomeBusca);
    }
}