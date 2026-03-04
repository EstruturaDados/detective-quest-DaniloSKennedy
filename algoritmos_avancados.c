#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// ============================
// ===== ÁRVORE BINÁRIA ======
// ============================

typedef struct Sala {
    char nome[50];
    char pista[50]; // vazio se não houver pista
    struct Sala *esq;
    struct Sala *dir;
} Sala;

Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = nova->dir = NULL;
    return nova;
}

// ============================
// ===== BST DE PISTAS =======
// ============================

typedef struct NoBST {
    char pista[50];
    struct NoBST *esq;
    struct NoBST *dir;
} NoBST;

NoBST* inserirBST(NoBST *raiz, char *pista) {
    if (raiz == NULL) {
        NoBST *novo = (NoBST*) malloc(sizeof(NoBST));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirBST(raiz->dir, pista);

    return raiz;
}

void emOrdem(NoBST *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf(" - %s\n", raiz->pista);
        emOrdem(raiz->dir);
    }
}

// ============================
// ===== TABELA HASH =========
// ============================

typedef struct NoHash {
    char pista[50];
    char suspeito[50];
    struct NoHash *prox;
} NoHash;

NoHash* tabela[TAM_HASH];
int contadorSuspeito[TAM_HASH];

int funcaoHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);

    NoHash *novo = (NoHash*) malloc(sizeof(NoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;

    contadorSuspeito[indice]++;
}

void mostrarHash() {
    printf("\n=== Relações Pista → Suspeito ===\n");
    for (int i = 0; i < TAM_HASH; i++) {
        NoHash *aux = tabela[i];
        while (aux != NULL) {
            printf("%s → %s\n", aux->pista, aux->suspeito);
            aux = aux->prox;
        }
    }
}

void suspeitoMaisCitado() {
    int max = 0, indice = -1;
    for (int i = 0; i < TAM_HASH; i++) {
        if (contadorSuspeito[i] > max) {
            max = contadorSuspeito[i];
            indice = i;
        }
    }

    if (indice == -1) {
        printf("Nenhum suspeito registrado.\n");
        return;
    }

    printf("\nSuspeito mais citado:\n");
    NoHash *aux = tabela[indice];
    if (aux != NULL)
        printf(">> %s (associado a %d pistas)\n", aux->suspeito, max);
}

// ============================
// ===== EXPLORAÇÃO =========
// ============================

void explorarSalas(Sala *raiz, NoBST **bst) {
    Sala *atual = raiz;
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Você encontrou a pista: %s\n", atual->pista);
            *bst = inserirBST(*bst, atual->pista);
        }

        printf("Escolha (e=esquerda, d=direita, s=sair): ");
        scanf(" %c", &opcao);

        if (opcao == 'e')
            atual = atual->esq;
        else if (opcao == 'd')
            atual = atual->dir;
        else if (opcao == 's')
            break;
        else
            printf("Opção inválida!\n");
    }
}

// ============================
// ===== MAIN (MESTRE) ======
// ============================

int main() {

    // Inicializa hash
    for (int i = 0; i < TAM_HASH; i++) {
        tabela[i] = NULL;
        contadorSuspeito[i] = 0;
    }

    // Monta árvore binária da mansão
    Sala *hall = criarSala("Hall de Entrada", "");
    hall->esq = criarSala("Biblioteca", "Livro Rasgado");
    hall->dir = criarSala("Cozinha", "Faca Ensanguentada");

    hall->esq->esq = criarSala("Escritório", "Carta Misteriosa");
    hall->esq->dir = criarSala("Sala de Estar", "");

    hall->dir->esq = criarSala("Jardim", "Pegadas na Terra");
    hall->dir->dir = criarSala("Porão", "Luvas Sujas");

    NoBST *bstPistas = NULL;

    int opcao;

    do {
        printf("\n===== DETECTIVE QUEST =====\n");
        printf("1 - Explorar mansão\n");
        printf("2 - Ver pistas coletadas\n");
        printf("3 - Associar pistas a suspeitos\n");
        printf("4 - Ver associações\n");
        printf("5 - Ver suspeito mais citado\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                explorarSalas(hall, &bstPistas);
                break;

            case 2:
                printf("\n=== Pistas em ordem alfabética ===\n");
                emOrdem(bstPistas);
                break;

            case 3: {
                char pista[50], suspeito[50];
                printf("Digite a pista: ");
                scanf(" %[^\n]", pista);
                printf("Digite o suspeito: ");
                scanf(" %[^\n]", suspeito);
                inserirNaHash(pista, suspeito);
                break;
            }

            case 4:
                mostrarHash();
                break;

            case 5:
                suspeitoMaisCitado();
                break;

            case 0:
                printf("Encerrando investigação...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while(opcao != 0);

    return 0;
}
