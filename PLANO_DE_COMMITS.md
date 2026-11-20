# Plano sugerido de commits

Faça os commits na ordem abaixo, depois de implementar e testar cada etapa. Não é necessário esperar um dia inteiro entre eles: a divisão serve para registrar o progresso real.

1. `feat: cria estrutura da lista encadeada`
   - Criar `lista.h` com `TipoDado`, `No`, `Lista` e protótipos.
   - Criar a inicialização e a liberação básica em `lista.c`.

2. `feat: implementa insercao e impressao`
   - Implementar cópia dinâmica de int, float e texto.
   - Implementar inserção no final e impressão.

3. `feat: implementa busca e remocao`
   - Implementar busca por índice e remoção por índice.
   - Testar remoção do primeiro, do meio, do último e índice inválido.

4. `feat: implementa salvamento em arquivo binario`
   - Gravar assinatura, versão, quantidade, tipo, tamanho e dados.
   - Não gravar endereços de ponteiros.

5. `feat: implementa carregamento do arquivo binario`
   - Ler o arquivo, validar os campos e reconstruir os nós.
   - Testar arquivo inexistente e arquivo salvo anteriormente.

6. `feat: adiciona menu e integra operacoes`
   - Criar o menu em `main.c` e ligar todas as funções.
   - Testar o fluxo completo.

7. `docs: adiciona instrucoes de compilacao e uso`
   - Adicionar README e revisar o repositório.

Observação: este plano é uma sugestão de histórico. Se for entregar commits por dia, faça cada etapa de verdade naquele dia; não crie commits retroativos para dar a impressão de que o trabalho foi feito em datas diferentes.
