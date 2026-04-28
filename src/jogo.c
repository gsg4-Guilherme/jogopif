#include "jogo.h"

#include <string.h>

#include "config.h"
#include "raylib.h"

static float CalcularCentroFaixa(int faixa)
{
    float larguraPista = 450.0f;
    float margemEsquerda = (LARGURA_JANELA - larguraPista) / 2.0f;
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;

    return margemEsquerda + (larguraFaixa * faixa) + (larguraFaixa / 2.0f);
}

void InicializarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    memset(jogo, 0, sizeof(*jogo));
    ReiniciarJogo(jogo);
}

void ReiniciarJogo(EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    // Este estado inicial será distribuído nos módulos de jogador, pista e pontuação.
    jogo->jogador.faixaAtual = 1;
    jogo->jogador.largura = 54.0f;
    jogo->jogador.altura = 86.0f;
    jogo->jogador.posicaoX = CalcularCentroFaixa(jogo->jogador.faixaAtual) - (jogo->jogador.largura / 2.0f);
    jogo->jogador.posicaoY = ALTURA_JANELA - 130.0f;
    jogo->jogador.caixaColisao = (Rectangle){
        jogo->jogador.posicaoX,
        jogo->jogador.posicaoY,
        jogo->jogador.largura,
        jogo->jogador.altura
    };

    jogo->pontuacaoAtual = 0;
    jogo->tempoSobrevivencia = 0.0f;
    jogo->intervaloObstaculo = 1.5f;
    jogo->velocidadeBase = 180.0f;
    jogo->jogoAtivo = true;
}

void AtualizarJogo(EstadoJogo *jogo, float delta)
{
    if (jogo == NULL || !jogo->jogoAtivo) {
        return;
    }

    // TODO: Dev 2 deverá trocar este movimento simples por uma função em jogador.c.
    if (IsKeyPressed(KEY_LEFT) && jogo->jogador.faixaAtual > 0) {
        jogo->jogador.faixaAtual--;
    }

    if (IsKeyPressed(KEY_RIGHT) && jogo->jogador.faixaAtual < QUANTIDADE_FAIXAS - 1) {
        jogo->jogador.faixaAtual++;
    }

    jogo->jogador.posicaoX = CalcularCentroFaixa(jogo->jogador.faixaAtual) - (jogo->jogador.largura / 2.0f);
    jogo->jogador.caixaColisao.x = jogo->jogador.posicaoX;

    // TODO: Dev 3 deverá mover a regra de pontuação para pontuacao.c.
    jogo->tempoSobrevivencia += delta;
    jogo->pontuacaoAtual = (int)(jogo->tempoSobrevivencia * 10.0f);
}

void DesenharJogo(const EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    float larguraPista = 450.0f;
    float margemEsquerda = (LARGURA_JANELA - larguraPista) / 2.0f;
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;

    DrawRectangle((int)margemEsquerda, 0, (int)larguraPista, ALTURA_JANELA, (Color){ 42, 45, 52, 255 });

    for (int faixa = 1; faixa < QUANTIDADE_FAIXAS; faixa++) {
        int x = (int)(margemEsquerda + (larguraFaixa * faixa));
        DrawLine(x, 0, x, ALTURA_JANELA, YELLOW);
    }

    DrawRectangleRounded(jogo->jogador.caixaColisao, 0.18f, 6, (Color){ 40, 130, 220, 255 });
    DrawText("CARRO", (int)jogo->jogador.posicaoX + 2, (int)jogo->jogador.posicaoY + 32, 14, RAYWHITE);
    DrawText(TextFormat("Pontos: %d", jogo->pontuacaoAtual), 30, 30, 24, RAYWHITE);
}

void FinalizarJogo(EstadoJogo *jogo)
{
    // Ainda não há memória dinâmica nesta etapa. A lista encadeada será liberada no próximo commit.
    (void)jogo;
}
