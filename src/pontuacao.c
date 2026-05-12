#include "pontuacao.h"

#include <stddef.h>

#include "config.h"
#include "ranking.h"

void InicializarPontuacao(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    jogo->pontuacaoAtual = 0;
    jogo->tempoSobrevivencia = 0.0f;
    jogo->pontosBonusPowerUp = 0.0f;
    jogo->melhorPontuacao = CarregarMelhorPontuacao(CAMINHO_RANKING);
    jogo->chuvaAtiva = false;
    jogo->engarrafamentoAtivo = false;
}

void AtualizarPontuacao(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL) {
        return;
    }

    jogo->tempoSobrevivencia += delta;

    if (jogo->tempoDobroPontos > 0.0f) {
        jogo->pontosBonusPowerUp += delta * 10.0f;
    }

    jogo->pontuacaoAtual = (int)((jogo->tempoSobrevivencia * 10.0f) + jogo->pontosBonusPowerUp);

    jogo->chuvaAtiva = ((int)jogo->tempoSobrevivencia % 18) >= 12;
    jogo->engarrafamentoAtivo = ((int)jogo->tempoSobrevivencia % 25) >= 20;
}
