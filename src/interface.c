#include "interface.h"

#include <stddef.h>

#include "config.h"
#include "powerups.h"
#include "raylib.h"

static void DesenharFiltroChuva(void)
{
    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 0, 0, 0, 68 });
}

static void DesenharGotasChuva(void)
{
    const int quantidadeGotas = 76;
    const int cicloVertical = ALTURA_JANELA + 160;
    const float larguraGota = 3.0f;
    const float alturaGota = 34.0f;
    const float velocidadeChuva = 520.0f;
    const Color corGota = (Color){ 150, 205, 230, 120 };
    int deslocamento = (int)((long long)(GetTime() * velocidadeChuva) % cicloVertical);

    for (int indice = 0; indice < quantidadeGotas; indice++) {
        int x = (indice * 97) % (LARGURA_JANELA + 180) - 90;
        int y = (indice * 53 + deslocamento) % cicloVertical - 120;
        Rectangle gota = {
            (float)x,
            (float)y,
            larguraGota,
            alturaGota
        };

        DrawRectanglePro(gota, (Vector2){ larguraGota / 2.0f, 0.0f }, -18.0f, corGota);
    }
}

static void DesenharEfeitoChuva(void)
{
    DesenharFiltroChuva();
    DesenharGotasChuva();
}

void DesenharTelaMenu(void)
{
    DrawText("Pânico na Agamenon", 250, 220, 38, RAYWHITE);
    DrawText("ENTER inicia a partida", 315, 290, 22, LIGHTGRAY);
    DrawText("ESC fecha a janela", 345, 325, 18, GRAY);
    DrawText("Objetivo: desviar dos veículos e sobreviver mais tempo.", 175, 390, 20, RAYWHITE);
}

void DesenharHud(const EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    DrawText(TextFormat("Pontos: %d", jogo->pontuacaoAtual), 30, 30, 24, RAYWHITE);
    DrawText(TextFormat("Recorde: %d", jogo->melhorPontuacao), 30, 62, 20, LIGHTGRAY);
    DrawText(TextFormat("Obstáculos: %d", jogo->obstaculos.quantidade), 30, 90, 18, LIGHTGRAY);
    DesenharPowerUpGuardado(jogo);
    DrawText("Setas mudam de faixa | Seta cima ativa power-up", 245, 650, 18, LIGHTGRAY);
}

void DesenharTelaGameOver(const EstadoJogo *jogo)
{
    int pontuacao = jogo != NULL ? jogo->pontuacaoAtual : 0;

    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 0, 0, 0, 150 });
    DrawText("Fim de jogo", 335, 250, 40, RAYWHITE);
    DrawText(TextFormat("Pontuação: %d", pontuacao), 365, 305, 22, LIGHTGRAY);
    DrawText("Pressione ENTER para tentar de novo", 255, 355, 22, LIGHTGRAY);
}

void DesenharEventos(const EstadoJogo *jogo)
{
    if (jogo == NULL) {
        return;
    }

    if (jogo->chuvaAtiva) {
        DesenharEfeitoChuva();
        DrawText("CHUVA", 680, 30, 20, SKYBLUE);
    }

    if (jogo->engarrafamentoAtivo) {
        DrawText("ENGARRAFAMENTO", 680, 60, 20, ORANGE);
    }

    DesenharIndicadoresPowerUps(jogo);
}
