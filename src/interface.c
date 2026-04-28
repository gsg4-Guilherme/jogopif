#include "interface.h"

#include <stddef.h>

#include "config.h"
#include "raylib.h"

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
    DrawText("Setas mudam de faixa | Obstáculos aparecem sozinhos", 230, 650, 18, LIGHTGRAY);
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

    // Estes indicadores são ganchos visuais; a regra definitiva fica para o Dev 3 completar.
    if (jogo->chuvaAtiva) {
        DrawText("CHUVA", 770, 30, 20, SKYBLUE);
    }

    if (jogo->engarrafamentoAtivo) {
        DrawText("ENGARRAFAMENTO", 680, 60, 20, ORANGE);
    }
}
