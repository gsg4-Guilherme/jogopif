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

static const float MULTIPLICADOR_INTERVALO_ENGARRAFAMENTO = 0.70f;
static const float MULTIPLICADOR_VELOCIDADE_CHUVA = 1.25f;
static const float INTERVALO_MINIMO_NORMAL = 0.45f;
static const float INTERVALO_MINIMO_ENGARRAFAMENTO = 0.35f;

static float ObterMultiplicadorVelocidadeChuva(const EstadoJogo *jogo)
{
    return (jogo != NULL && jogo->chuvaAtiva) ? MULTIPLICADOR_VELOCIDADE_CHUVA : 1.0f;
}

static float CalcularProximoIntervaloObstaculo(const EstadoJogo *jogo)
{
    float intervaloBase = 1.35f - (jogo->tempoSobrevivencia * 0.015f);
    float variacaoAleatoria = (float)GetRandomValue(0, 30) / 100.0f;
    float intervaloMinimo = INTERVALO_MINIMO_NORMAL;

    if (intervaloBase < INTERVALO_MINIMO_NORMAL) {
        intervaloBase = INTERVALO_MINIMO_NORMAL;
    }

    float intervalo = intervaloBase + variacaoAleatoria;

    if (jogo->engarrafamentoAtivo) {
        intervalo *= MULTIPLICADOR_INTERVALO_ENGARRAFAMENTO;
        intervaloMinimo = INTERVALO_MINIMO_ENGARRAFAMENTO;
    }

    if (intervalo < intervaloMinimo) {
        intervalo = intervaloMinimo;
    }

    return intervalo;
}

static TipoObstaculo SortearTipoObstaculo(const EstadoJogo *jogo)
{
    int chanceOnibus = (jogo != NULL && jogo->engarrafamentoAtivo) ? 45 : 20;

    if (GetRandomValue(1, 100) <= chanceOnibus) {
        return OBSTACULO_ONIBUS;
    }

    return OBSTACULO_CARRO;
}

static void GerarObstaculoAleatorio(EstadoJogo *jogo)
{
    int faixa = GetRandomValue(0, QUANTIDADE_FAIXAS - 1);
    TipoObstaculo tipo = SortearTipoObstaculo(jogo);
    float aumentoPorTempo = jogo->tempoSobrevivencia * 5.0f;
    float variacaoVelocidade = (float)GetRandomValue(-20, 35);

    if (aumentoPorTempo > 220.0f) {
        aumentoPorTempo = 220.0f;
    }

    AdicionarObstaculo(&jogo->obstaculos, faixa, jogo->velocidadeBase + aumentoPorTempo + variacaoVelocidade, tipo);
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

static void AtualizarDeslocamentoCenario(EstadoJogo *jogo, float delta)
{
    float aumentoPorTempo = jogo->tempoSobrevivencia * 5.0f;

    if (aumentoPorTempo > 220.0f) {
        aumentoPorTempo = 220.0f;
    }

    jogo->deslocamentoCenario += (jogo->velocidadeBase + aumentoPorTempo) *
        ObterMultiplicadorVelocidadeChuva(jogo) * delta;
}

void InicializarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    SetRandomSeed((unsigned int)time(NULL));
    memset(jogo, 0, sizeof(*jogo));
    CarregarTexturaJogador();
    CarregarTexturasObstaculos();
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
    jogo->deslocamentoCenario = 0.0f;
    jogo->jogoAtivo = true;
}

void AtualizarJogo(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL || !jogo->jogoAtivo) {
        return;
    }

    AtualizarJogador(&jogo->jogador, delta);
    AtualizarPontuacao(jogo, delta);
    AtualizarDeslocamentoCenario(jogo, delta);
    AtualizarGeracaoObstaculos(jogo, delta);

    AtualizarObstaculos(&jogo->obstaculos, delta * ObterMultiplicadorVelocidadeChuva(jogo));
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

    DesenharPista(jogo->pistaLogica, jogo->deslocamentoCenario);
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
    LiberarTexturasObstaculos();
    LiberarTexturaJogador();
}
