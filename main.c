#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_ENTRADA 256
#define ARQUIVO_PADRAO "lista.bin"

static int lerInteiro(const char *mensagem, int *valor) {
    char linha[TAMANHO_ENTRADA];
    char extra;
    printf("%s", mensagem);
    if (fgets(linha, sizeof linha, stdin) == NULL) return 0;
    return sscanf(linha, " %d %c", valor, &extra) == 1;
}

static int lerPosicao(const char *mensagem, size_t *posicao) {
    int valor;
    if (!lerInteiro(mensagem, &valor) || valor < 0) return 0;
    *posicao = (size_t)valor;
    return 1;
}

static void inserirPeloMenu(Lista *lista) {
    int tipo;
    if (!lerInteiro("Tipo (1-Inteiro, 2-Real, 3-Texto): ", &tipo)) {
        printf("Entrada invalida.\n"); return;
    }

    if (tipo == TIPO_INTEIRO) {
        int valor;
        if (!lerInteiro("Digite o inteiro: ", &valor)) {
            printf("Valor invalido.\n"); return;
        }
        printf(inserirElemento(lista, tipo, &valor) ? "Elemento inserido.\n" : "Falha ao inserir.\n");
    } else if (tipo == TIPO_REAL) {
        char linha[TAMANHO_ENTRADA];
        float valor;
        printf("Digite o numero real (use ponto decimal): ");
        if (fgets(linha, sizeof linha, stdin) == NULL || sscanf(linha, "%f", &valor) != 1) {
            printf("Valor invalido.\n"); return;
        }
        printf(inserirElemento(lista, tipo, &valor) ? "Elemento inserido.\n" : "Falha ao inserir.\n");
    } else if (tipo == TIPO_TEXTO) {
        char texto[TAMANHO_ENTRADA];
        printf("Digite o texto: ");
        if (fgets(texto, sizeof texto, stdin) == NULL) return;
        texto[strcspn(texto, "\n")] = '\0';
        printf(inserirElemento(lista, tipo, texto) ? "Elemento inserido.\n" : "Falha ao inserir.\n");
    } else {
        printf("Tipo inexistente.\n");
    }
}

static void buscarPeloMenu(const Lista *lista) {
    int tipo;
    if (!lerInteiro("Tipo para buscar (1-Inteiro, 2-Real, 3-Texto): ", &tipo)) {
        printf("Tipo invalido.\n"); return;
    }

    int inteiro;
    float real;
    char texto[TAMANHO_ENTRADA];
    const void *chave = NULL;

    if (tipo == TIPO_INTEIRO) {
        if (!lerInteiro("Digite o inteiro procurado: ", &inteiro)) {
            printf("Valor invalido.\n"); return;
        }
        chave = &inteiro;
    } else if (tipo == TIPO_REAL) {
        char linha[TAMANHO_ENTRADA];
        printf("Digite o numero real procurado: ");
        if (fgets(linha, sizeof linha, stdin) == NULL || sscanf(linha, "%f", &real) != 1) {
            printf("Valor invalido.\n"); return;
        }
        chave = &real;
    } else if (tipo == TIPO_TEXTO) {
        printf("Digite o texto procurado: ");
        if (fgets(texto, sizeof texto, stdin) == NULL) return;
        texto[strcspn(texto, "\n")] = '\0';
        chave = texto;
    } else {
        printf("Tipo inexistente.\n"); return;
    }

    No *no = buscarElemento(lista, tipo, chave);
    if (no == NULL) printf("Elemento nao encontrado.\n");
    else printf("Elemento encontrado: "),
         (tipo == TIPO_INTEIRO ? printf("%d\n", *(int *)no->dados) :
          tipo == TIPO_REAL ? printf("%.2f\n", *(float *)no->dados) :
                              printf("%s\n", (char *)no->dados));
}

int main(void) {
    Lista lista;
    inicializarLista(&lista);

    int opcao = -1;
    do {
        printf("\n===== LISTA ENCADEADA =====\n");
        printf("1 - Inserir elemento\n");
        printf("2 - Remover elemento\n");
        printf("3 - Buscar elemento por valor\n");
        printf("4 - Imprimir lista\n");
        printf("5 - Salvar lista em arquivo binario\n");
        printf("6 - Carregar lista de arquivo binario\n");
        printf("0 - Sair\n");

        if (!lerInteiro("Escolha: ", &opcao)) {
            printf("Opcao invalida.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                inserirPeloMenu(&lista);
                break;
            case 2: {
                size_t posicao;
                if (!lerPosicao("Posicao para remover (comeca em 0): ", &posicao))
                    printf("Posicao invalida.\n");
                else
                    printf(removerElemento(&lista, posicao) ? "Elemento removido.\n" : "Nao foi possivel remover.\n");
                break;
            }
            case 3:
                buscarPeloMenu(&lista);
                break;
            case 4:
                imprimirLista(&lista);
                break;
            case 5:
                printf(salvarLista(&lista, ARQUIVO_PADRAO) ? "Lista salva em %s.\n" : "Erro ao salvar arquivo.\n", ARQUIVO_PADRAO);
                break;
            case 6:
                printf(carregarLista(&lista, ARQUIVO_PADRAO) ? "Lista carregada de %s.\n" : "Erro ao carregar arquivo.\n", ARQUIVO_PADRAO);
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opcao inexistente.\n");
        }
    } while (opcao != 0);

    liberarLista(&lista);
    return 0;
}
