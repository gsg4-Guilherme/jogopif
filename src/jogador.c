#include "jogador.h"

#include <stddef.h>

#include "config.h"
#include "raylib.h"

#define CAMINHO_SPRITE_JOGADOR "assets/veiculos/carro_jogador.png"
#define LARGURA_SPRITE_JOGADOR 60.0f
#define ALTURA_SPRITE_JOGADOR 90.0f

static Texture2D texturaJogador = { 0 };

float CalcularCentroFaixa(int faixa)
{
    float larguraPista = LARGURA_PISTA_VISUAL;
    float margemEsquerda = (LARGURA_JANELA - larguraPista) / 2.0f;
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;

    return margemEsquerda + (larguraFaixa * faixa) + (larguraFaixa / 2.0f);
}

void CarregarTexturaJogador(void)
{
    if (IsTextureValid(texturaJogador)) {
        return;
    }

    texturaJogador = LoadTexture(CAMINHO_SPRITE_JOGADOR);
}

void LiberarTexturaJogador(void)
{
    if (IsTextureValid(texturaJogador)) {
        UnloadTexture(texturaJogador);
        texturaJogador = (Texture2D){ 0 };
    }
}

void InicializarJogador(Jogador *jogador)
{
    if (jogador == NULL) {
        return;
    }

    jogador->faixaAtual = QUANTIDADE_FAIXAS / 2;
    jogador->largura = LARGURA_SPRITE_JOGADOR;
    jogador->altura = ALTURA_SPRITE_JOGADOR;
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

    if (IsTextureValid(texturaJogador)) {
        Rectangle origem = { 0.0f, 0.0f, (float)texturaJogador.width, (float)texturaJogador.height };
        DrawTexturePro(texturaJogador, origem, jogador->caixaColisao, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        return;
    }

    DrawRectangleRounded(jogador->caixaColisao, 0.18f, 6, (Color){ 40, 130, 220, 255 });
    DrawRectangleLinesEx(jogador->caixaColisao, 2.0f, RAYWHITE);
    DrawText("CARRO", (int)jogador->posicaoX + 2, (int)jogador->posicaoY + 34, 14, RAYWHITE);
}
