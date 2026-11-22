#include "lista.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_ENTRADA 256
#define ARQUIVO_PADRAO "lista.bin"

/* Le um inteiro e rejeita entradas com letras ou caracteres extras. */
static int lerInteiro(const char *mensagem, int *valor) {
    char linha[TAMANHO_ENTRADA];
    char *fim;
    long numero;

    printf("%s", mensagem);
    if (fgets(linha, sizeof linha, stdin) == NULL) return 0;

    errno = 0;
    numero = strtol(linha, &fim, 10);
    if (fim == linha || errno == ERANGE || numero < INT_MIN || numero > INT_MAX)
        return 0;

    while (isspace((unsigned char)*fim)) fim++;
    if (*fim != '\0') return 0;

    *valor = (int)numero;
    return 1;
}

/* Le um numero real finito e rejeita caracteres extras. */
static int lerReal(const char *mensagem, float *valor) {
    char linha[TAMANHO_ENTRADA];
    char *fim;
    float numero;

    printf("%s", mensagem);
    if (fgets(linha, sizeof linha, stdin) == NULL) return 0;

    errno = 0;
    numero = strtof(linha, &fim);
    if (fim == linha || errno == ERANGE || !isfinite(numero)) return 0;

    while (isspace((unsigned char)*fim)) fim++;
    if (*fim != '\0') return 0;

    *valor = numero;
    return 1;
}

static int lerPosicao(const char *mensagem, size_t *posicao) {
    char linha[TAMANHO_ENTRADA];
    char *fim;
    unsigned long long numero;

    printf("%s", mensagem);
    if (fgets(linha, sizeof linha, stdin) == NULL) return 0;
    if (linha[0] == '-') return 0;

    errno = 0;
    numero = strtoull(linha, &fim, 10);
    if (fim == linha || errno == ERANGE || numero > (unsigned long long)SIZE_MAX)
        return 0;

    while (isspace((unsigned char)*fim)) fim++;
    if (*fim != '\0') return 0;

    *posicao = (size_t)numero;
    return 1;
}

static int lerTexto(const char *mensagem, char *texto, size_t capacidade) {
    size_t tamanho;
    int caractere;

    printf("%s", mensagem);
    if (fgets(texto, (int)capacidade, stdin) == NULL) return 0;

    tamanho = strlen(texto);
    if (tamanho > 0 && texto[tamanho - 1] == '\n') {
        texto[tamanho - 1] = '\0';
    } else {
        /* Descarta o restante caso o texto ultrapasse o limite do buffer. */
        while ((caractere = getchar()) != '\n' && caractere != EOF) { }
        if (tamanho == capacidade - 1) return 0;
    }
    return 1;
}

static void inserirPeloMenu(Lista *lista) {
    int tipo;
    if (!lerInteiro("Tipo (1-Inteiro, 2-Real, 3-Texto): ", &tipo)) {
        printf("Entrada invalida. Digite 1, 2 ou 3.\n");
        return;
    }

    if (tipo == TIPO_INTEIRO) {
        int valor;
        if (!lerInteiro("Digite o inteiro: ", &valor)) {
            printf("Valor inteiro invalido.\n");
            return;
        }
        printf(inserirElemento(lista, tipo, &valor) ? "Elemento inserido.\n" : "Falha ao inserir.\n");
    } else if (tipo == TIPO_REAL) {
        float valor;
        if (!lerReal("Digite o numero real (use ponto decimal): ", &valor)) {
            printf("Numero real invalido.\n");
            return;
        }
        printf(inserirElemento(lista, tipo, &valor) ? "Elemento inserido.\n" : "Falha ao inserir.\n");
    } else if (tipo == TIPO_TEXTO) {
        char texto[TAMANHO_ENTRADA];
        if (!lerTexto("Digite o texto: ", texto, sizeof texto)) {
            printf("Texto invalido ou muito longo.\n");
            return;
        }
        printf(inserirElemento(lista, tipo, texto) ? "Elemento inserido.\n" : "Falha ao inserir.\n");
    } else {
        printf("Tipo inexistente. Escolha 1, 2 ou 3.\n");
    }
}

static void inserirNoInicioPeloMenu(Lista *lista) {
    int tipo;
    if (!lerInteiro("Tipo (1-Inteiro, 2-Real, 3-Texto): ", &tipo)) {
        printf("Tipo invalido.\n");
        return;
    }

    if (tipo == TIPO_INTEIRO) {
        int valor;
        if (!lerInteiro("Digite o inteiro: ", &valor)) { printf("Valor invalido.\n"); return; }
        printf(inserirNoInicio(lista, tipo, &valor) ? "Elemento inserido no inicio.\n" : "Falha ao inserir.\n");
    } else if (tipo == TIPO_REAL) {
        float valor;
        if (!lerReal("Digite o numero real: ", &valor)) { printf("Valor invalido.\n"); return; }
        printf(inserirNoInicio(lista, tipo, &valor) ? "Elemento inserido no inicio.\n" : "Falha ao inserir.\n");
    } else if (tipo == TIPO_TEXTO) {
        char texto[TAMANHO_ENTRADA];
        if (!lerTexto("Digite o texto: ", texto, sizeof texto)) { printf("Texto invalido.\n"); return; }
        printf(inserirNoInicio(lista, tipo, texto) ? "Elemento inserido no inicio.\n" : "Falha ao inserir.\n");
    } else {
        printf("Tipo inexistente. Escolha 1, 2 ou 3.\n");
    }
}

static void removerPorValorPeloMenu(Lista *lista) {
    int tipo;
    if (!lerInteiro("Tipo (1-Inteiro, 2-Real, 3-Texto): ", &tipo)) { printf("Tipo invalido.\n"); return; }

    int inteiro;
    float real;
    char texto[TAMANHO_ENTRADA];
    const void *chave = NULL;

    if (tipo == TIPO_INTEIRO) {
        if (!lerInteiro("Valor inteiro para remover: ", &inteiro)) { printf("Valor invalido.\n"); return; }
        chave = &inteiro;
    } else if (tipo == TIPO_REAL) {
        if (!lerReal("Valor real para remover: ", &real)) { printf("Valor invalido.\n"); return; }
        chave = &real;
    } else if (tipo == TIPO_TEXTO) {
        if (!lerTexto("Texto para remover: ", texto, sizeof texto)) { printf("Texto invalido.\n"); return; }
        chave = texto;
    } else { printf("Tipo inexistente.\n"); return; }

    printf(removerPorValor(lista, tipo, chave) ? "Elemento removido.\n" : "Elemento nao encontrado.\n");
}

static void buscarPeloMenu(const Lista *lista) {
    int tipo;
    if (!lerInteiro("Tipo para buscar (1-Inteiro, 2-Real, 3-Texto): ", &tipo)) {
        printf("Tipo invalido.\n");
        return;
    }

    int inteiro;
    float real;
    char texto[TAMANHO_ENTRADA];
    const void *chave = NULL;

    if (tipo == TIPO_INTEIRO) {
        if (!lerInteiro("Digite o inteiro procurado: ", &inteiro)) {
            printf("Valor inteiro invalido.\n");
            return;
        }
        chave = &inteiro;
    } else if (tipo == TIPO_REAL) {
        if (!lerReal("Digite o numero real procurado: ", &real)) {
            printf("Numero real invalido.\n");
            return;
        }
        chave = &real;
    } else if (tipo == TIPO_TEXTO) {
        if (!lerTexto("Digite o texto procurado: ", texto, sizeof texto)) {
            printf("Texto invalido ou muito longo.\n");
            return;
        }
        chave = texto;
    } else {
        printf("Tipo inexistente.\n");
        return;
    }

    No *no = buscarElemento(lista, tipo, chave);
    if (no == NULL) {
        printf("Elemento nao encontrado.\n");
    } else {
        printf("Elemento encontrado: ");
        if (tipo == TIPO_INTEIRO) printf("%d\n", *(int *)no->dados);
        else if (tipo == TIPO_REAL) printf("%.2f\n", *(float *)no->dados);
        else printf("%s\n", (char *)no->dados);
    }
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
        printf("7 - Inserir elemento no inicio\n");
        printf("8 - Remover elemento pelo valor\n");
        printf("0 - Sair\n");

        if (!lerInteiro("Escolha: ", &opcao)) {
            printf("Opcao invalida. Digite um numero do menu.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1:
                inserirPeloMenu(&lista);
                break;
            case 2: {
                size_t posicao;
                if (!lerPosicao("Posicao para remover (comeca em 0): ", &posicao))
                    printf("Posicao invalida. Use um numero inteiro nao negativo.\n");
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
            case 7:
                inserirNoInicioPeloMenu(&lista);
                break;
            case 8:
                removerPorValorPeloMenu(&lista);
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
