#include "jogo.h"

#include <string.h>
#include <time.h>

#include "config.h"
#include "interface.h"
#include "jogador.h"
#include "obstaculos.h"
#include "pista.h"
#include "pontuacao.h"
#include "raylib.h"
#include "ranking.h"

static float CalcularProximoIntervaloObstaculo(const EstadoJogo *jogo)
{
    float intervaloBase = 1.35f - (jogo->tempoSobrevivencia * 0.015f);
    float variacaoAleatoria = (float)GetRandomValue(0, 30) / 100.0f;

    if (intervaloBase < 0.45f) {
        intervaloBase = 0.45f;
    }

    return intervaloBase + variacaoAleatoria;
}

static void GerarObstaculoAleatorio(EstadoJogo *jogo)
{
    int faixa = GetRandomValue(0, QUANTIDADE_FAIXAS - 1);
    float aumentoPorTempo = jogo->tempoSobrevivencia * 5.0f;
    float variacaoVelocidade = (float)GetRandomValue(-20, 35);

    if (aumentoPorTempo > 220.0f) {
        aumentoPorTempo = 220.0f;
    }

    AdicionarObstaculo(&jogo->obstaculos, faixa, jogo->velocidadeBase + aumentoPorTempo + variacaoVelocidade);
}

static void AtualizarGeracaoObstaculos(EstadoJogo *jogo, float delta)
{
    jogo->tempoGerarObstaculo += delta;

    if (jogo->tempoGerarObstaculo >= jogo->intervaloObstaculo) {
        GerarObstaculoAleatorio(jogo);
        jogo->tempoGerarObstaculo = 0.0f;
        jogo->intervaloObstaculo = CalcularProximoIntervaloObstaculo(jogo);
    }
}

void InicializarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    SetRandomSeed((unsigned int)time(NULL));
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
    jogo->intervaloObstaculo = 1.0f;
    jogo->velocidadeBase = 190.0f;
    jogo->jogoAtivo = true;
}

void AtualizarJogo(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL || !jogo->jogoAtivo) {
        return;
    }

    AtualizarJogador(&jogo->jogador, delta);
    AtualizarPontuacao(jogo, delta);
    AtualizarGeracaoObstaculos(jogo, delta);

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
