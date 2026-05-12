#include "powerups.h"

#include <stddef.h>

#include "config.h"
#include "jogador.h"
#include "obstaculos.h"
#include "raylib.h"

#define TAMANHO_POWERUP 40.0f
#define TAMANHO_SLOT_POWERUP 56.0f
#define TEMPO_FREIO 5.0f
#define TEMPO_DOBRO_PONTOS 6.0f
#define TEMPO_MANUTENCAO 12.0f
#define VELOCIDADE_LIMPA_FAIXA -420.0f

static const float LIMITE_SUPERIOR_SPAWN_POWERUP = -160.0f;
static const float LIMITE_INFERIOR_SPAWN_POWERUP = 180.0f;

static const char *ObterRotuloPowerUp(TipoPowerUp tipo)
{
    switch (tipo) {
        case POWERUP_ESCUDO:
            return "ESC";
        case POWERUP_FREIO:
            return "FRE";
        case POWERUP_LIMPA_FAIXA:
            return "LIM";
        case POWERUP_DOBRO_PONTOS:
            return "2X";
        case POWERUP_MANUTENCAO:
            return "MAN";
        case POWERUP_NENHUM:
        default:
            return "UP";
    }
}

static Color ObterCorPowerUp(TipoPowerUp tipo)
{
    switch (tipo) {
        case POWERUP_ESCUDO:
            return (Color){ 45, 145, 235, 255 };
        case POWERUP_FREIO:
            return (Color){ 235, 180, 45, 255 };
        case POWERUP_LIMPA_FAIXA:
            return (Color){ 225, 75, 85, 255 };
        case POWERUP_DOBRO_PONTOS:
            return (Color){ 95, 200, 105, 255 };
        case POWERUP_MANUTENCAO:
            return (Color){ 150, 90, 210, 255 };
        case POWERUP_NENHUM:
        default:
            return (Color){ 42, 45, 52, 255 };
    }
}

static float SortearIntervaloPowerUp(void)
{
    return 6.5f + ((float)GetRandomValue(0, 450) / 100.0f);
}

static TipoPowerUp SortearTipoPowerUp(void)
{
    return (TipoPowerUp)GetRandomValue(POWERUP_ESCUDO, POWERUP_MANUTENCAO);
}

static float CalcularVelocidadePowerUp(const EstadoJogo *jogo)
{
    float aumentoPorTempo = 0.0f;
    float velocidadeBase = 190.0f;

    if (jogo != NULL) {
        aumentoPorTempo = jogo->tempoSobrevivencia * 5.0f;

        if (jogo->velocidadeBase > 0.0f) {
            velocidadeBase = jogo->velocidadeBase;
        }
    }

    if (aumentoPorTempo > 220.0f) {
        aumentoPorTempo = 220.0f;
    }

    return velocidadeBase + aumentoPorTempo;
}

static void AtualizarCaixaPowerUp(PowerUpColetavel *powerUp)
{
    if (powerUp == NULL) {
        return;
    }

    powerUp->caixaColisao = (Rectangle){
        CalcularCentroFaixa(powerUp->faixa) - (TAMANHO_POWERUP / 2.0f),
        powerUp->posicaoY,
        TAMANHO_POWERUP,
        TAMANHO_POWERUP
    };
}

static bool ObstaculoBloqueiaSpawnPowerUp(const Obstaculo *obstaculo, int faixa)
{
    return obstaculo != NULL &&
        obstaculo->faixa == faixa &&
        obstaculo->posicaoY >= LIMITE_SUPERIOR_SPAWN_POWERUP &&
        obstaculo->posicaoY <= LIMITE_INFERIOR_SPAWN_POWERUP;
}

static bool SortearFaixaPowerUp(const EstadoJogo *jogo, int *faixaEscolhida)
{
    int faixasDisponiveis[QUANTIDADE_FAIXAS] = { 0 };
    int quantidadeDisponiveis = 0;

    if (jogo == NULL || faixaEscolhida == NULL) {
        return false;
    }

    for (int faixa = 0; faixa < QUANTIDADE_FAIXAS; faixa++) {
        bool bloqueada = false;

        for (const Obstaculo *obstaculo = jogo->obstaculos.inicio;
             obstaculo != NULL;
             obstaculo = obstaculo->proximo) {
            if (ObstaculoBloqueiaSpawnPowerUp(obstaculo, faixa)) {
                bloqueada = true;
                break;
            }
        }

        if (!bloqueada) {
            faixasDisponiveis[quantidadeDisponiveis] = faixa;
            quantidadeDisponiveis++;
        }
    }

    if (quantidadeDisponiveis == 0) {
        return false;
    }

    *faixaEscolhida = faixasDisponiveis[GetRandomValue(0, quantidadeDisponiveis - 1)];
    return true;
}

static void DesativarPowerUpColetavel(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    jogo->powerUpColetavel.ativo = false;
    jogo->powerUpColetavel.tipo = POWERUP_NENHUM;
    jogo->powerUpColetavel.faixa = 0;
    jogo->powerUpColetavel.posicaoY = 0.0f;
    jogo->powerUpColetavel.velocidade = 0.0f;
    jogo->powerUpColetavel.caixaColisao = (Rectangle){ 0 };
}

static void CriarPowerUpColetavel(EstadoJogo *jogo)
{
    int faixa = 0;

    if (jogo == NULL || jogo->powerUpGuardado != POWERUP_NENHUM) {
        return;
    }

    if (!SortearFaixaPowerUp(jogo, &faixa)) {
        jogo->tempoGerarPowerUp = 0.0f;
        jogo->intervaloPowerUp = 1.5f;
        return;
    }

    jogo->powerUpColetavel.ativo = true;
    jogo->powerUpColetavel.tipo = SortearTipoPowerUp();
    jogo->powerUpColetavel.faixa = faixa;
    jogo->powerUpColetavel.posicaoY = -TAMANHO_POWERUP;
    jogo->powerUpColetavel.velocidade = CalcularVelocidadePowerUp(jogo);
    AtualizarCaixaPowerUp(&jogo->powerUpColetavel);
}

static void AtivarPowerUpGuardado(EstadoJogo *jogo)
{
    if (jogo == NULL || jogo->powerUpGuardado == POWERUP_NENHUM) {
        return;
    }

    switch (jogo->powerUpGuardado) {
        case POWERUP_ESCUDO:
            jogo->escudoAtivo = true;
            break;
        case POWERUP_FREIO:
            jogo->tempoFreio = TEMPO_FREIO;
            break;
        case POWERUP_LIMPA_FAIXA:
            AplicarVelocidadeObstaculosFaixa(
                &jogo->obstaculos,
                jogo->jogador.faixaAtual,
                VELOCIDADE_LIMPA_FAIXA
            );
            break;
        case POWERUP_DOBRO_PONTOS:
            jogo->tempoDobroPontos = TEMPO_DOBRO_PONTOS;
            break;
        case POWERUP_MANUTENCAO:
            jogo->tempoManutencao = TEMPO_MANUTENCAO;
            jogo->faixaManutencao = jogo->jogador.faixaAtual;
            break;
        case POWERUP_NENHUM:
        default:
            break;
    }

    jogo->powerUpGuardado = POWERUP_NENHUM;
    jogo->tempoGerarPowerUp = 0.0f;
    jogo->intervaloPowerUp = SortearIntervaloPowerUp();
}

static void AtualizarTemporizadoresPowerUps(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL) {
        return;
    }

    if (jogo->tempoFreio > 0.0f) {
        jogo->tempoFreio -= delta;

        if (jogo->tempoFreio < 0.0f) {
            jogo->tempoFreio = 0.0f;
        }
    }

    if (jogo->tempoDobroPontos > 0.0f) {
        jogo->tempoDobroPontos -= delta;

        if (jogo->tempoDobroPontos < 0.0f) {
            jogo->tempoDobroPontos = 0.0f;
        }
    }

    if (jogo->tempoManutencao > 0.0f) {
        jogo->tempoManutencao -= delta;

        if (jogo->tempoManutencao <= 0.0f) {
            jogo->tempoManutencao = 0.0f;
            jogo->faixaManutencao = -1;
        }
    }
}

static void AtualizarPowerUpColetavel(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL || !jogo->powerUpColetavel.ativo) {
        return;
    }

    jogo->powerUpColetavel.posicaoY += jogo->powerUpColetavel.velocidade * delta;
    AtualizarCaixaPowerUp(&jogo->powerUpColetavel);

    if (jogo->powerUpColetavel.posicaoY > ALTURA_JANELA + TAMANHO_POWERUP) {
        DesativarPowerUpColetavel(jogo);
        jogo->tempoGerarPowerUp = 0.0f;
        jogo->intervaloPowerUp = SortearIntervaloPowerUp();
        return;
    }

    if (jogo->powerUpGuardado == POWERUP_NENHUM &&
        CheckCollisionRecs(jogo->jogador.caixaColisao, jogo->powerUpColetavel.caixaColisao)) {
        jogo->powerUpGuardado = jogo->powerUpColetavel.tipo;
        DesativarPowerUpColetavel(jogo);
        jogo->tempoGerarPowerUp = 0.0f;
    }
}

static void AtualizarGeracaoPowerUp(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL ||
        jogo->powerUpGuardado != POWERUP_NENHUM ||
        jogo->powerUpColetavel.ativo) {
        return;
    }

    jogo->tempoGerarPowerUp += delta;

    if (jogo->tempoGerarPowerUp >= jogo->intervaloPowerUp) {
        CriarPowerUpColetavel(jogo);
        jogo->tempoGerarPowerUp = 0.0f;
        jogo->intervaloPowerUp = SortearIntervaloPowerUp();
    }
}

static void DesenharQuadradoPowerUp(Rectangle caixa, TipoPowerUp tipo, bool vazio)
{
    const char *rotulo = vazio ? "" : ObterRotuloPowerUp(tipo);
    int tamanhoFonte = caixa.width > TAMANHO_POWERUP ? 23 : 17;
    int larguraTexto = MeasureText(rotulo, tamanhoFonte);
    Color corFundo = vazio ? (Color){ 30, 32, 38, 210 } : ObterCorPowerUp(tipo);
    Color corBorda = vazio ? (Color){ 160, 165, 176, 210 } : RAYWHITE;

    DrawRectangleRec(caixa, corFundo);
    DrawRectangleLinesEx(caixa, 2.0f, corBorda);

    if (vazio) {
        return;
    }

    DrawText(
        rotulo,
        (int)(caixa.x + ((caixa.width - (float)larguraTexto) / 2.0f)),
        (int)(caixa.y + ((caixa.height - (float)tamanhoFonte) / 2.0f)),
        tamanhoFonte,
        RAYWHITE
    );
}

void InicializarPowerUps(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    jogo->tempoGerarPowerUp = 0.0f;
    jogo->intervaloPowerUp = SortearIntervaloPowerUp();
    jogo->tempoFreio = 0.0f;
    jogo->tempoDobroPontos = 0.0f;
    jogo->tempoManutencao = 0.0f;
    jogo->pontosBonusPowerUp = 0.0f;
    jogo->faixaManutencao = -1;
    jogo->powerUpGuardado = POWERUP_NENHUM;
    jogo->escudoAtivo = false;
    DesativarPowerUpColetavel(jogo);
}

void AtualizarPowerUps(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL) {
        return;
    }

    if (IsKeyPressed(KEY_UP)) {
        AtivarPowerUpGuardado(jogo);
    }

    AtualizarTemporizadoresPowerUps(jogo, delta);

    if (jogo->powerUpGuardado != POWERUP_NENHUM) {
        DesativarPowerUpColetavel(jogo);
        jogo->tempoGerarPowerUp = 0.0f;
        return;
    }

    AtualizarPowerUpColetavel(jogo, delta);
    AtualizarGeracaoPowerUp(jogo, delta);
}

void DesenharPowerUpColetavel(const EstadoJogo *jogo)
{
    if (jogo == NULL || !jogo->powerUpColetavel.ativo) {
        return;
    }

    DesenharQuadradoPowerUp(jogo->powerUpColetavel.caixaColisao, jogo->powerUpColetavel.tipo, false);
}

void DesenharZonaManutencao(const EstadoJogo *jogo)
{
    if (jogo == NULL ||
        jogo->tempoManutencao <= 0.0f ||
        jogo->faixaManutencao < 0 ||
        jogo->faixaManutencao >= QUANTIDADE_FAIXAS) {
        return;
    }

    float larguraFaixa = LARGURA_PISTA_VISUAL / QUANTIDADE_FAIXAS;
    float x = CalcularCentroFaixa(jogo->faixaManutencao) - (larguraFaixa / 2.0f);

    DrawRectangleRec(
        (Rectangle){ x, 0.0f, larguraFaixa, (float)ALTURA_JANELA },
        (Color){ 80, 210, 120, 42 }
    );
    DrawRectangleLinesEx(
        (Rectangle){ x + 4.0f, 4.0f, larguraFaixa - 8.0f, (float)ALTURA_JANELA - 8.0f },
        2.0f,
        (Color){ 105, 245, 145, 130 }
    );
}

void DesenharEscudoJogador(const EstadoJogo *jogo)
{
    if (jogo == NULL || !jogo->escudoAtivo) {
        return;
    }

    Rectangle caixaEscudo = {
        jogo->jogador.caixaColisao.x - 8.0f,
        jogo->jogador.caixaColisao.y - 8.0f,
        jogo->jogador.caixaColisao.width + 16.0f,
        jogo->jogador.caixaColisao.height + 16.0f
    };

    DrawRectangleRoundedLinesEx(caixaEscudo, 0.35f, 8, 4.0f, (Color){ 90, 190, 255, 230 });
}

void DesenharPowerUpGuardado(const EstadoJogo *jogo)
{
    Rectangle slot = {
        (float)LARGURA_JANELA - 76.0f,
        22.0f,
        TAMANHO_SLOT_POWERUP,
        TAMANHO_SLOT_POWERUP
    };

    if (jogo == NULL || jogo->powerUpGuardado == POWERUP_NENHUM) {
        DesenharQuadradoPowerUp(slot, POWERUP_NENHUM, true);
        return;
    }

    DesenharQuadradoPowerUp(slot, jogo->powerUpGuardado, false);
}

void DesenharIndicadoresPowerUps(const EstadoJogo *jogo)
{
    int x = LARGURA_JANELA - 230;
    int y = 92;

    if (jogo == NULL) {
        return;
    }

    if (jogo->escudoAtivo) {
        DrawText("ESCUDO", x, y, 18, SKYBLUE);
        y += 24;
    }

    if (jogo->tempoFreio > 0.0f) {
        DrawText(TextFormat("FREIO %.0f", jogo->tempoFreio), x, y, 18, GOLD);
        y += 24;
    }

    if (jogo->tempoDobroPontos > 0.0f) {
        DrawText(TextFormat("PONTOS 2X %.0f", jogo->tempoDobroPontos), x, y, 18, LIME);
        y += 24;
    }

    if (jogo->tempoManutencao > 0.0f) {
        DrawText(TextFormat("MANUTENCAO %.0f", jogo->tempoManutencao), x, y, 18, VIOLET);
    }
}
