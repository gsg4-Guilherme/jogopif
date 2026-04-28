#include "jogador.h"

#include <stddef.h>

#include "config.h"
#include "raylib.h"

float CalcularCentroFaixa(int faixa)
{
    float larguraPista = 450.0f;
    float margemEsquerda = (LARGURA_JANELA - larguraPista) / 2.0f;
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;

    return margemEsquerda + (larguraFaixa * faixa) + (larguraFaixa / 2.0f);
}

void InicializarJogador(Jogador *jogador)
{
    if (jogador == NULL) {
        return;
    }

    jogador->faixaAtual = 1;
    jogador->largura = 54.0f;
    jogador->altura = 86.0f;
    jogador->velocidadeLateral = 0.0f;
    jogador->posicaoX = CalcularCentroFaixa(jogador->faixaAtual) - (jogador->largura / 2.0f);
    jogador->posicaoY = ALTURA_JANELA - 130.0f;
    jogador->caixaColisao = (Rectangle){
        jogador->posicaoX,
        jogador->posicaoY,
        jogador->largura,
        jogador->altura
    };
}

void AtualizarJogador(Jogador *jogador, float delta)
{
    (void)delta;

    if (jogador == NULL) {
        return;
    }

    // As setas mudam de faixa. Depois, o grupo pode suavizar o movimento com interpolação.
    if (IsKeyPressed(KEY_LEFT) && jogador->faixaAtual > 0) {
        jogador->faixaAtual--;
    }

    if (IsKeyPressed(KEY_RIGHT) && jogador->faixaAtual < QUANTIDADE_FAIXAS - 1) {
        jogador->faixaAtual++;
    }

    jogador->posicaoX = CalcularCentroFaixa(jogador->faixaAtual) - (jogador->largura / 2.0f);
    jogador->caixaColisao.x = jogador->posicaoX;
    jogador->caixaColisao.y = jogador->posicaoY;
}

void DesenharJogador(const Jogador *jogador)
{
    if (jogador == NULL) {
        return;
    }

    DrawRectangleRounded(jogador->caixaColisao, 0.18f, 6, (Color){ 40, 130, 220, 255 });
    DrawRectangleLinesEx(jogador->caixaColisao, 2.0f, RAYWHITE);
    DrawText("CARRO", (int)jogador->posicaoX + 2, (int)jogador->posicaoY + 32, 14, RAYWHITE);
}
