#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necessário para medir o tempo (clock())

// ---------------------------------------------
// DEFINIÇÕES E ESTRUTURAS
// ---------------------------------------------

// Capacidade máxima de componentes na mochila
#define CAPACIDADE_MAXIMA 20 
// Tamanhos máximos das strings
#define NOME_MAX 30
#define TIPO_MAX 20

// Estrutura que representa um Componente da Torre de Fuga
typedef struct {
    char nome[NOME_MAX];       // Nome do componente
    char tipo[TIPO_MAX];       // Tipo do componente (controle, suporte, propulsão, etc.)
    int prioridade;            // Prioridade de montagem (1 a 10)
} Componente;

// Variáveis de estado global para controle da mochila
Componente mochila[CAPACIDADE_MAXIMA];
int totalComponentes = 0; 
// Status de ordenação: 0 = NÃO ORDENADO, 1 = ORDENADO POR NOME
int statusOrdenacao = 0; 

// ---------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// ---------------------------------------------

// Funções Auxiliares
void limparBuffer();
void pausarSistema();
void trocar(Componente *a, Componente *b); // Implementação movida para o topo

// Funções de Gerenciamento da Mochila
void adicionarComponente();
void descartarComponente();
void listarComponentes();

// Funções de Ordenação (Medem tempo e comparações)
void bubbleSortNome();
void insertionSortTipo();
void selectionSortPrioridade();

// Funções de Busca
void buscaBinariaPorNome();

// Menu
void menuOrganizarMochila();

// ---------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES
// ---------------------------------------------

/**
 * @brief Limpa o buffer de entrada (stdin).
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
    getchar(); 
}

/**
 * @brief Troca dois componentes de posição na memória.
 * Esta função é crucial para os algoritmos de ordenação.
 */
void trocar(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

// ---------------------------------------------
// FUNÇÃO PRINCIPAL (main)
// ---------------------------------------------

int main() {
    int opcao;

    printf("--- INICIANDO PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE) ---\n");
    
    do {
        printf("\n======================================================\n");
        printf("PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
        printf("======================================================\n");
        printf("Itens na Mochila: %d/%d\n", totalComponentes, CAPACIDADE_MAXIMA);
        printf("Status da Ordenacao por Nome: %s\n", statusOrdenacao ? "ORDENADO" : "NAO ORDENADO");
        printf("------------------------------------------------------\n");
        printf("1. Adicionar Componente\n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventario)\n");
        printf("4. Organizar Mochila (Ordenar Componentes)\n");
        printf("5. Busca Binaria por Componente-Chave (por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("------------------------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1; 
        }
        limparBuffer(); 

        switch (opcao) {
            case 1:
                adicionarComponente();
                break;
            case 2:
                descartarComponente();
                break;
            case 3:
                listarComponentes();
                pausarSistema();
                break;
            case 4:
                menuOrganizarMochila();
                break;
            case 5:
                buscaBinariaPorNome();
                pausarSistema();
                break;
            case 0:
                printf("\n--- ATIVANDO TORRE DE FUGA! O jogo termina aqui. ---\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ---------------------------------------------
// FUNÇÕES DE GERENCIAMENTO (CRUD)
// ---------------------------------------------

/**
 * @brief Cadastra um novo componente na mochila.
 */
void adicionarComponente() {
    if (totalComponentes >= CAPACIDADE_MAXIMA) {
        printf("\nERRO: Mochila cheia! (%d/%d) Nao e possivel adicionar mais componentes.\n", totalComponentes, CAPACIDADE_MAXIMA);
        return;
    }

    Componente *novoComp = &mochila[totalComponentes];

    printf("\n--- ADICIONAR NOVO COMPONENTE ---\n");

    // Lendo o Nome
    printf("Nome do Componente (max %d): ", NOME_MAX - 1);
    if (fgets(novoComp->nome, NOME_MAX, stdin) == NULL) return;
    novoComp->nome[strcspn(novoComp->nome, "\n")] = 0; 

    // Lendo o Tipo
    printf("Tipo do Componente (ex: controle, suporte - max %d): ", TIPO_MAX - 1);
    if (fgets(novoComp->tipo, TIPO_MAX, stdin) == NULL) return;
    novoComp->tipo[strcspn(novoComp->tipo, "\n")] = 0;

    // Lendo a Prioridade
    printf("Prioridade (1 a 10): ");
    // Correção: Garantir que o buffer seja limpo em caso de falha no scanf
    if (scanf("%d", &novoComp->prioridade) != 1 || novoComp->prioridade < 1 || novoComp->prioridade > 10) {
        printf("ERRO: Prioridade invalida (deve ser entre 1 e 10). Cancelando insercao.\n");
        limparBuffer();
        return; 
    }
    limparBuffer(); 

    statusOrdenacao = 0;
    totalComponentes++;
    printf("\nSUCESSO: Componente \"%s\" adicionado a mochila!\n", novoComp->nome);
    listarComponentes();
    pausarSistema();
}

/**
 * @brief Remove um componente da mochila pelo nome.
 */
void descartarComponente() {
    if (totalComponentes == 0) {
        printf("\nERRO: A mochila esta vazia. Nao ha o que descartar.\n");
        return;
    }

    char nomeBusca[NOME_MAX];
    int posEncontrada = -1; 

    printf("\n--- DESCARTAR COMPONENTE ---\n");
    printf("Digite o nome exato do componente para descartar: ");
    
    if (fgets(nomeBusca, NOME_MAX, stdin) == NULL) return;
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    for (int i = 0; i < totalComponentes; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            posEncontrada = i;
            break; 
        }
    }

    if (posEncontrada != -1) {
        printf("\nSUCESSO: Componente \"%s\" descartado.\n", mochila[posEncontrada].nome);

        // Deslocamento dos elementos
        for (int i = posEncontrada; i < totalComponentes - 1; i++) {
            mochila[i] = mochila[i+1]; 
        }

        totalComponentes--;
        statusOrdenacao = 0; 
        listarComponentes();
        pausarSistema();
    } else {
        printf("\nERRO: Componente \"%s\" nao encontrado na mochila.\n", nomeBusca);
    }
}

/**
 * @brief Lista todos os componentes na mochila.
 */
void listarComponentes() {
    if (totalComponentes == 0) {
        printf("\n--- INVENTARIO VAZIO ---\n");
        return;
    }

    printf("\n----------------------------------------------------------------\n");
    // Correção: Uso correto do especificador de largura para "QTD"
    printf("| %-*s | %-*s | %-*s | %s |\n", NOME_MAX - 1, "NOME", TIPO_MAX - 1, "TIPO", 10, "PRIORIDADE", "QTD");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < totalComponentes; i++) {
        // Correção: Alinhamento da coluna QTD para 3 caracteres
        printf("| %-*s | %-*s | %-10d | %-3d |\n", 
               NOME_MAX - 1, mochila[i].nome, 
               TIPO_MAX - 1, mochila[i].tipo, 
               mochila[i].prioridade, 
               1
        );
    }
    printf("----------------------------------------------------------------\n");
    printf("Total de %d componentes diferentes na mochila.\n", totalComponentes);
}

// ---------------------------------------------
// FUNÇÕES DE ORDENAÇÃO (com Medição de Desempenho)
// ---------------------------------------------

/**
 * @brief Menu para escolher o método de ordenação.
 */
void menuOrganizarMochila() {
    int opcao;
    
    printf("\n--- ORGANIZAR MOCHILA (ORDENAR COMPONENTES) ---\n");
    printf("Escolha o criterio de organizacao:\n");
    printf("1. Bubble Sort por Nome (para Busca Binaria)\n");
    printf("2. Insertion Sort por Tipo\n");
    printf("3. Selection Sort por Prioridade\n");
    printf("0. Cancelar\n");
    printf("Opcao: ");

    if (scanf("%d", &opcao) != 1) {
        limparBuffer();
        printf("Opcao invalida.\n");
        return;
    }
    limparBuffer();

    // Verificação para evitar que o algoritmo rode em um vetor vazio
    if (totalComponentes < 1 && opcao != 0) {
        printf("\nERRO: E necessario ter pelo menos 1 componente para ordenar.\n");
        return;
    }

    switch (opcao) {
        case 1:
            bubbleSortNome();
            break;
        case 2:
            insertionSortTipo();
            break;
        case 3:
            selectionSortPrioridade();
            break;
        case 0:
            printf("Organizacao cancelada.\n");
            break;
        default:
            printf("Opcao invalida.\n");
    }
}

/**
 * @brief Implementa o Bubble Sort para ordenar por Nome (string).
 */
void bubbleSortNome() {
    if (totalComponentes <= 1) return;

    int comparacoes = 0;
    clock_t inicio = clock(); 

    for (int i = 0; i < totalComponentes - 1; i++) {
        for (int j = 0; j < totalComponentes - 1 - i; j++) {
            comparacoes++; 
            if (strcmp(mochila[j].nome, mochila[j+1].nome) > 0) {
                trocar(&mochila[j], &mochila[j+1]);
            }
        }
    }

    clock_t fim = clock();
    double tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    statusOrdenacao = 1;

    printf("\n--- ORDENACAO CONCLUIDA (Bubble Sort por Nome) ---\n");
    listarComponentes(); 
    printf("\n--- DESEMPENHO EDUCAIONAL ---\n");
    printf("Comparacoes realizadas: %d\n", comparacoes);
    printf("Tempo de execucao: %.6f segundos\n", tempo_execucao);
    pausarSistema();
}

/**
 * @brief Implementa o Insertion Sort para ordenar por Tipo (string).
 */
void insertionSortTipo() {
    if (totalComponentes <= 1) return;

    int comparacoes = 0;
    clock_t inicio = clock(); 

    for (int i = 1; i < totalComponentes; i++) {
        Componente chave = mochila[i];
        int j = i - 1;

        while (j >= 0) {
            comparacoes++; 
            if (strcmp(mochila[j].tipo, chave.tipo) > 0) {
                mochila[j+1] = mochila[j];
                j--;
            } else {
                break;
            }
        }
        mochila[j+1] = chave;
    }

    clock_t fim = clock();
    double tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    statusOrdenacao = 0; 

    printf("\n--- ORDENACAO CONCLUIDA (Insertion Sort por Tipo) ---\n");
    listarComponentes();
    printf("\n--- DESEMPENHO EDUCAIONAL ---\n");
    printf("Comparacoes realizadas: %d\n", comparacoes);
    printf("Tempo de execucao: %.6f segundos\n", tempo_execucao);
    pausarSistema();
}

/**
 * @brief Implementa o Selection Sort para ordenar por Prioridade (int).
 */
void selectionSortPrioridade() {
    if (totalComponentes <= 1) return;

    int comparacoes = 0;
    clock_t inicio = clock(); 

    for (int i = 0; i < totalComponentes - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < totalComponentes; j++) {
            comparacoes++; 
            if (mochila[j].prioridade < mochila[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            trocar(&mochila[min_idx], &mochila[i]);
        }
    }

    clock_t fim = clock();
    double tempo_execucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    statusOrdenacao = 0; 

    printf("\n--- ORDENACAO CONCLUIDA (Selection Sort por Prioridade) ---\n");
    listarComponentes();
    printf("\n--- DESEMPENHO EDUCAIONAL ---\n");
    printf("Comparacoes realizadas: %d\n", comparacoes);
    printf("Tempo de execucao: %.6f segundos\n", tempo_execucao);
    pausarSistema();
}

// ---------------------------------------------
// FUNÇÃO DE BUSCA BINÁRIA
// ---------------------------------------------

/**
 * @brief Implementa a Busca Binária por Nome (string).
 */
void buscaBinariaPorNome() {
    if (totalComponentes == 0) {
        printf("\nERRO: Mochila vazia. Nada para buscar.\n");
        return;
    }

    if (statusOrdenacao == 0) {
        printf("\nERRO: A busca binaria por nome so pode ser executada APOS a ordenacao por Nome (Opcao 4 -> 1).\n");
        return;
    }

    char nomeBusca[NOME_MAX];
    printf("\n--- Busca Binaria por Componente-Chave ---\n");
    printf("Nome do componente a buscar: ");
    
    if (fgets(nomeBusca, NOME_MAX, stdin) == NULL) return;
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int inicio = 0;
    int fim = totalComponentes - 1;
    int posEncontrada = -1;
    int comparacoes = 0;

    // Implementação da Busca Binária
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int resultado = strcmp(mochila[meio].nome, nomeBusca);
        comparacoes++;

        if (resultado == 0) {
            posEncontrada = meio; // Componente encontrado
            break;
        } else if (resultado < 0) {
            inicio = meio + 1; // Buscar na metade direita
        } else {
            fim = meio - 1; // Buscar na metade esquerda
        }
    }

    // Exibir resultado e desempenho
    printf("\n--- RESULTADO DA BUSCA BINARIA ---\n");
    if (posEncontrada != -1) {
        Componente *item = &mochila[posEncontrada];
        printf("--- Componente-Chave Encontrado! ---\n");
        printf("Nome: %s, Tipo: %s, Prioridade: %d, Qtd: 1\n", item->nome, item->tipo, item->prioridade);
        printf("Posicao no Inventario (Indice): %d\n", posEncontrada);
    } else {
        printf("ERRO: Componente \"%s\" nao foi encontrado na mochila.\n", nomeBusca);
    }

    printf("\n--- DESEMPENHO ---\n");
    printf("Comparacoes de strings realizadas: %d\n", comparacoes);
}