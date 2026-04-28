#include "jogo.h"

#include <string.h>

#include "config.h"
#include "interface.h"
#include "jogador.h"
#include "obstaculos.h"
#include "pista.h"
#include "pontuacao.h"
#include "raylib.h"
#include "ranking.h"

void InicializarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    memset(jogo, 0, sizeof(*jogo));
    InicializarListaObstaculos(&jogo->obstaculos);
    ReiniciarJogo(jogo);
}

void ReiniciarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    LiberarObstaculos(&jogo->obstaculos);
    InicializarListaObstaculos(&jogo->obstaculos);
    InicializarJogador(&jogo->jogador);
    InicializarPista(jogo->pistaLogica);
    InicializarPontuacao(jogo);

    jogo->tempoGerarObstaculo = 0.0f;
    jogo->intervaloObstaculo = 1.5f;
    jogo->velocidadeBase = 180.0f;
    jogo->jogoAtivo = true;
}

void AtualizarJogo(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL || !jogo->jogoAtivo) {
        return;
    }

    AtualizarJogador(&jogo->jogador, delta);
    AtualizarPontuacao(jogo, delta);

    // TODO: Dev 2 pode trocar o SPACE por geração temporizada baseada em dificuldade.
    if (IsKeyPressed(KEY_SPACE)) {
        int faixa = GetRandomValue(0, QUANTIDADE_FAIXAS - 1);
        AdicionarObstaculo(&jogo->obstaculos, faixa, jogo->velocidadeBase);
    }

    AtualizarObstaculos(&jogo->obstaculos, delta);
    AtualizarPista(jogo->pistaLogica, &jogo->jogador, &jogo->obstaculos);

    if (VerificarColisaoJogadorObstaculos(&jogo->jogador, &jogo->obstaculos)) {
        jogo->jogoAtivo = false;
        SalvarPontuacaoSeRecorde(CAMINHO_RANKING, jogo->pontuacaoAtual);
    }
}

void DesenharJogo(const EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    DesenharPista(jogo->pistaLogica);
    DesenharObstaculos(&jogo->obstaculos);
    DesenharJogador(&jogo->jogador);
    DesenharEventos(jogo);
    DesenharHud(jogo);
}

void FinalizarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    LiberarObstaculos(&jogo->obstaculos);
}
