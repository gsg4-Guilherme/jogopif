# Guia da estrutura do projeto

Este guia explica a intenção de cada arquivo para que o grupo consiga continuar o jogo sem depender de uma implementação pronta.

## Fluxo principal

O fluxo foi separado em camadas simples:

```text
main.c -> engine.c -> jogo.c -> módulos específicos
```

- `main.c`: só cria a configuração padrão e chama a engine.
- `engine.c`: abre a janela Raylib, controla FPS, troca de telas e chama atualização/desenho.
- `jogo.c`: coordena a partida e chama os módulos de jogador, pista, obstáculos, pontuação, ranking e interface.

## Dev 1: engine e núcleo

Arquivos principais:

- `include/config.h`: constantes globais, como tamanho da janela, FPS, quantidade de faixas e caminho do ranking.
- `include/tipos.h`: `structs` e `enum` compartilhados pelo projeto.
- `include/engine.h` e `src/engine.c`: loop principal, tela atual, inicialização e fechamento da Raylib.
- `src/main.c`: ponto de entrada do programa.

Tarefas sugeridas:

- Melhorar transições entre menu, jogo e fim de jogo.
- Criar uma tela de pausa, se o grupo quiser.
- Garantir que a engine não misture regra de jogo com desenho específico.

## Dev 2: jogabilidade

Arquivos principais:

- `src/jogador.c`: posição do carro, faixa atual e leitura de teclado.
- `src/obstaculos.c`: lista encadeada com alocação dinâmica para os veículos.
- `src/pista.c`: matriz lógica da avenida, usada para explicar o requisito de matrizes.

Tarefas sugeridas:

- Trocar o `SPACE` por geração automática de obstáculos.
- Aumentar velocidade e frequência com o tempo.
- Ajustar colisão, tamanhos e posições para o jogo ficar justo.

## Dev 3: interface, pontuação e dados

Arquivos principais:

- `src/interface.c`: menu, HUD, fim de jogo e indicadores visuais.
- `src/pontuacao.c`: regra de pontuação e eventos de chuva/engarrafamento.
- `src/ranking.c`: leitura e escrita do melhor recorde em arquivo.

Tarefas sugeridas:

- Melhorar visual das telas sem deixar a lógica presa na interface.
- Salvar nome e pontuação, usando `RegistroPontuacao`.
- Transformar chuva e engarrafamento em eventos com efeito real na jogabilidade.

## Requisitos da disciplina já previstos

- `structs`: `Jogador`, `Obstaculo`, `ListaObstaculos`, `EstadoJogo`, `RegistroPontuacao`.
- Ponteiros: funções recebem ponteiros para alterar estado sem copiar tudo.
- Alocação dinâmica: `obstaculos.c` usa `malloc` para criar obstáculos.
- Lista encadeada: cada `Obstaculo` aponta para o próximo.
- Matrizes: `pistaLogica[LINHAS_PISTA][COLUNAS_PISTA]`.
- Arquivos: `ranking.c` lê e escreve pontuação em `data/ranking.txt`.
- Biblioteca de jogo: Raylib.
- Makefile: `make`, `make run` e `make clean`.

## Ordem recomendada para continuar

1. Compilar a base com Raylib instalada.
2. Fazer o Dev 2 trocar `SPACE` por geração automática.
3. Fazer o Dev 3 melhorar ranking e eventos.
4. Fazer o Dev 1 revisar troca de telas e pausa.
5. Gravar um vídeo curto mostrando compilação e execução.
6. Preparar slides com explicação das funções principais e dificuldades.

## Cuidados

- Mantenham nomes de variáveis em português sem acento, por portabilidade em C.
- Mantenham comentários em português com acentuação.
- Não coloquem arquivos de `build/` no Git.
- Não façam commit depois da data final definida pelo professor.
