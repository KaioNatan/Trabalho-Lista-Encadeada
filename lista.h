#ifndef LISTA_H
#define LISTA_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    TIPO_INTEIRO = 1,
    TIPO_REAL = 2,
    TIPO_TEXTO = 3
} TipoDado;

typedef struct No {
    struct No *proximo;
    int tipo;
    void *dados;
} No;

typedef struct {
    No *cabeca;
    size_t tamanho;
} Lista;

void inicializarLista(Lista *lista);
int inserirElemento(Lista *lista, int tipo, const void *dados);
int inserirNoInicio(Lista *lista, int tipo, const void *dados);
int removerElemento(Lista *lista, size_t posicao);
int removerPorValor(Lista *lista, int tipo, const void *dados);
No *buscarElemento(const Lista *lista, int tipo, const void *dados);
void imprimirLista(const Lista *lista);
int salvarLista(const Lista *lista, const char *nomeArquivo);
int carregarLista(Lista *lista, const char *nomeArquivo);
void liberarLista(Lista *lista);

#endif
