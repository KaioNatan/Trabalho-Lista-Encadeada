#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_MAX_TEXTO 1000000u
#define VERSAO_ARQUIVO 1u

static void liberarNo(No *no) {
    if (no != NULL) {
        free(no->dados);
        free(no);
    }
}

void inicializarLista(Lista *lista) {
    if (lista == NULL) return;
    lista->cabeca = NULL;
    lista->tamanho = 0;
}

static size_t tamanhoDoDado(int tipo, const void *dados) {
    if (dados == NULL) return 0;

    switch (tipo) {
        case TIPO_INTEIRO: return sizeof(int);
        case TIPO_REAL: return sizeof(float);
        case TIPO_TEXTO: return strlen((const char *)dados) + 1;
        default: return 0;
    }
}

int inserirElemento(Lista *lista, int tipo, const void *dados) {
    if (lista == NULL || dados == NULL) return 0;

    size_t bytes = tamanhoDoDado(tipo, dados);
    if (bytes == 0 || (tipo == TIPO_TEXTO && bytes > TAMANHO_MAX_TEXTO)) return 0;

    No *novo = malloc(sizeof(No));
    if (novo == NULL) return 0;

    novo->dados = malloc(bytes);
    if (novo->dados == NULL) {
        free(novo);
        return 0;
    }

    memcpy(novo->dados, dados, bytes);
    novo->tipo = tipo;
    novo->proximo = NULL;

    if (lista->cabeca == NULL) {
        lista->cabeca = novo;
    } else {
        No *atual = lista->cabeca;
        while (atual->proximo != NULL) atual = atual->proximo;
        atual->proximo = novo;
    }

    lista->tamanho++;
    return 1;
}

int inserirNoInicio(Lista *lista, int tipo, const void *dados) {
    if (lista == NULL || dados == NULL) return 0;

    size_t bytes = tamanhoDoDado(tipo, dados);
    if (bytes == 0 || (tipo == TIPO_TEXTO && bytes > TAMANHO_MAX_TEXTO)) return 0;

    No *novo = malloc(sizeof(No));
    if (novo == NULL) return 0;

    novo->dados = malloc(bytes);
    if (novo->dados == NULL) {
        free(novo);
        return 0;
    }

    memcpy(novo->dados, dados, bytes);
    novo->tipo = tipo;
    novo->proximo = lista->cabeca;
    lista->cabeca = novo;
    lista->tamanho++;
    return 1;
}

No *buscarElemento(const Lista *lista, int tipo, const void *dados) {
    if (lista == NULL || dados == NULL) return NULL;

    const No *atual = lista->cabeca;
    while (atual != NULL) {
        if (atual->tipo == tipo) {
            int iguais = 0;
            if (tipo == TIPO_INTEIRO)
                iguais = *(int *)atual->dados == *(const int *)dados;
            else if (tipo == TIPO_REAL)
                iguais = *(float *)atual->dados == *(const float *)dados;
            else if (tipo == TIPO_TEXTO)
                iguais = strcmp((const char *)atual->dados, (const char *)dados) == 0;
            if (iguais) return (No *)atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

static No *buscarPorIndice(const Lista *lista, size_t posicao) {
    if (lista == NULL || posicao >= lista->tamanho) return NULL;
    No *atual = lista->cabeca;
    for (size_t i = 0; i < posicao; i++) atual = atual->proximo;
    return atual;
}

int removerElemento(Lista *lista, size_t posicao) {
    if (lista == NULL || posicao >= lista->tamanho) return 0;

    No *removido;
    if (posicao == 0) {
        removido = lista->cabeca;
        lista->cabeca = removido->proximo;
    } else {
        No *anterior = buscarPorIndice(lista, posicao - 1);
        removido = anterior->proximo;
        anterior->proximo = removido->proximo;
    }

    liberarNo(removido);
    lista->tamanho--;
    return 1;
}

int removerPorValor(Lista *lista, int tipo, const void *dados) {
    if (lista == NULL || dados == NULL) return 0;

    No *atual = lista->cabeca;
    No *anterior = NULL;

    while (atual != NULL) {
        int iguais = 0;
        if (atual->tipo == tipo) {
            if (tipo == TIPO_INTEIRO)
                iguais = *(int *)atual->dados == *(const int *)dados;
            else if (tipo == TIPO_REAL)
                iguais = *(float *)atual->dados == *(const float *)dados;
            else if (tipo == TIPO_TEXTO)
                iguais = strcmp((const char *)atual->dados, (const char *)dados) == 0;
        }

        if (iguais) {
            if (anterior == NULL) lista->cabeca = atual->proximo;
            else anterior->proximo = atual->proximo;
            liberarNo(atual);
            lista->tamanho--;
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0;
}

void imprimirLista(const Lista *lista) {
    if (lista == NULL) {
        printf("Lista invalida.\n");
        return;
    }

    printf("\n===== CONTEUDO DA LISTA =====\n");
    if (lista->cabeca == NULL) {
        printf("Lista vazia.\n");
        printf("Total de elementos: 0\n");
        return;
    }

    const No *atual = lista->cabeca;
    size_t indice = 0;
    while (atual != NULL) {
        printf("[%zu] ", indice);
        switch (atual->tipo) {
            case TIPO_INTEIRO:
                printf("Inteiro: %d", *(int *)atual->dados);
                break;
            case TIPO_REAL:
                printf("Real: %.2f", *(float *)atual->dados);
                break;
            case TIPO_TEXTO:
                printf("Texto: %s", (char *)atual->dados);
                break;
            default:
                printf("Tipo desconhecido");
        }
        printf("\n");
        atual = atual->proximo;
        indice++;
    }

    printf("Total de elementos: %zu\n", lista->tamanho);
}

int salvarLista(const Lista *lista, const char *nomeArquivo) {
    if (lista == NULL || nomeArquivo == NULL) return 0;

    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) return 0;

    const char assinatura[4] = {'L', 'E', 'N', 'C'};
    uint32_t versao = VERSAO_ARQUIVO;
    uint32_t quantidade = (uint32_t)lista->tamanho;
    int sucesso = 1;

    if (fwrite(assinatura, sizeof assinatura, 1, arquivo) != 1 ||
        fwrite(&versao, sizeof versao, 1, arquivo) != 1 ||
        fwrite(&quantidade, sizeof quantidade, 1, arquivo) != 1) sucesso = 0;

    const No *atual = lista->cabeca;
    while (sucesso && atual != NULL) {
        uint32_t tipo = (uint32_t)atual->tipo;
        uint32_t tamanho = (uint32_t)tamanhoDoDado(atual->tipo, atual->dados);
        if (atual->tipo == TIPO_TEXTO) tamanho--;

        if (tamanho == 0 ||
            fwrite(&tipo, sizeof tipo, 1, arquivo) != 1 ||
            fwrite(&tamanho, sizeof tamanho, 1, arquivo) != 1) {
            sucesso = 0;
            break;
        }

        size_t bytes = atual->tipo == TIPO_TEXTO ? tamanho : tamanho;
        if (fwrite(atual->dados, 1, bytes, arquivo) != bytes) sucesso = 0;
        atual = atual->proximo;
    }

    if (fclose(arquivo) != 0) sucesso = 0;
    return sucesso;
}

int carregarLista(Lista *lista, const char *nomeArquivo) {
    if (lista == NULL || nomeArquivo == NULL) return 0;

    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) return 0;

    Lista temporaria;
    inicializarLista(&temporaria);
    char assinatura[4];
    uint32_t versao, quantidade;
    int sucesso = 1;

    if (fread(assinatura, sizeof assinatura, 1, arquivo) != 1 ||
        memcmp(assinatura, "LENC", 4) != 0 ||
        fread(&versao, sizeof versao, 1, arquivo) != 1 ||
        versao != VERSAO_ARQUIVO ||
        fread(&quantidade, sizeof quantidade, 1, arquivo) != 1 ||
        quantidade > 1000000u) sucesso = 0;

    for (uint32_t i = 0; sucesso && i < quantidade; i++) {
        uint32_t tipo, tamanho;
        if (fread(&tipo, sizeof tipo, 1, arquivo) != 1 ||
            fread(&tamanho, sizeof tamanho, 1, arquivo) != 1) {
            sucesso = 0;
            break;
        }

        if ((tipo == TIPO_INTEIRO && tamanho != sizeof(int)) ||
            (tipo == TIPO_REAL && tamanho != sizeof(float)) ||
            (tipo == TIPO_TEXTO && (tamanho == 0 || tamanho > TAMANHO_MAX_TEXTO)) ||
            (tipo < TIPO_INTEIRO || tipo > TIPO_TEXTO)) {
            sucesso = 0;
            break;
        }

        size_t bytes = tipo == TIPO_TEXTO ? (size_t)tamanho + 1 : tamanho;
        void *dados = malloc(bytes);
        if (dados == NULL) {
            sucesso = 0;
            break;
        }

        if (fread(dados, 1, tamanho, arquivo) != tamanho) {
            free(dados);
            sucesso = 0;
            break;
        }
        if (tipo == TIPO_TEXTO) ((char *)dados)[tamanho] = '\0';

        if (!inserirElemento(&temporaria, (int)tipo, dados)) sucesso = 0;
        free(dados);
    }

    fclose(arquivo);

    if (!sucesso) {
        liberarLista(&temporaria);
        return 0;
    }

    liberarLista(lista);
    *lista = temporaria;
    return 1;
}

void liberarLista(Lista *lista) {
    if (lista == NULL) return;

    No *atual = lista->cabeca;
    while (atual != NULL) {
        No *proximo = atual->proximo;
        liberarNo(atual);
        atual = proximo;
    }
    lista->cabeca = NULL;
    lista->tamanho = 0;
}
