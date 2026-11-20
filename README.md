# Trabalho III - Lista Encadeada

Implementação em C de uma lista encadeada genérica para a disciplina Programação de Computadores I.

## Funcionalidades

- Inserir inteiros, números reais (`float`) e textos.
- Remover um nó pela posição (índice iniciado em zero).
- Buscar um nó pelo valor e pelo tipo.
- Imprimir todos os elementos.
- Salvar e carregar a lista em arquivo binário (`lista.bin`).

## Estrutura

- `lista.h`: tipos, estruturas e protótipos.
- `lista.c`: implementação das operações da lista e persistência binária.
- `main.c`: menu de interação.

Cada nó possui `proximo`, `tipo` e `dados` (`void *`), conforme solicitado no enunciado. O programa copia os dados para memória própria e libera toda a memória ao remover nós ou encerrar.

## Compilação

Com GCC:

```bash
gcc -Wall -Wextra -std=c11 main.c lista.c -o lista
```

No Windows com MinGW:

```bash
gcc -Wall -Wextra -std=c11 main.c lista.c -o lista.exe
```

## Execução

Linux/macOS:

```bash
./lista
```

Windows:

```bash
lista.exe
```

O menu permite inserir dados de três tipos, buscar e remover por posição, imprimir e salvar/carregar a lista. Para números reais, digite o separador decimal como ponto, por exemplo `3.5`.

## Arquivo binário

O arquivo `lista.bin` contém uma assinatura, a versão do formato, a quantidade de nós e, para cada nó, seu tipo, tamanho e conteúdo. Os endereços de memória dos ponteiros não são gravados: os nós e seus dados são recriados quando o arquivo é carregado.

## Limitações / decisões

- Os tipos demonstrados são `int`, `float` e texto (`char *`). Novos tipos podem ser adicionados ampliando `TipoDado`, `tamanhoDoDado`, `imprimirLista` e as rotinas de gravação/leitura.
- A busca e a remoção são feitas por posição, começando em zero.
- O formato binário é destinado ao mesmo ambiente/arquitetura de compilação; a representação binária de `int` e `float` pode variar entre plataformas.
