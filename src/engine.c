#include "engine.h"

#include "jogo.h"
#include "raylib.h"

ConfiguracaoJanela CriarConfiguracaoPadrao(void)
{
    ConfiguracaoJanela configuracao = {
        .largura = LARGURA_JANELA,
        .altura = ALTURA_JANELA,
        .fps = FPS_ALVO,
        .titulo = TITULO_JANELA
    };

    return configuracao;
}

int RodarEngine(ConfiguracaoJanela configuracao)
{
    EstadoJogo jogo;
    TelaAtual telaAtual = TELA_MENU;

    InitWindow(configuracao.largura, configuracao.altura, configuracao.titulo);
    SetTargetFPS(configuracao.fps);

    InicializarJogo(&jogo);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // ESC volta para o menu durante a partida; no menu, fecha a janela.
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (telaAtual == TELA_MENU) {
                break;
            }

            telaAtual = TELA_MENU;
        }

        if (telaAtual == TELA_MENU && IsKeyPressed(KEY_ENTER)) {
            ReiniciarJogo(&jogo);
            telaAtual = TELA_JOGANDO;
        } else if (telaAtual == TELA_JOGANDO) {
            AtualizarJogo(&jogo, delta);

            if (!jogo.jogoAtivo) {
                telaAtual = TELA_GAME_OVER;
            }
        } else if (telaAtual == TELA_GAME_OVER && IsKeyPressed(KEY_ENTER)) {
            ReiniciarJogo(&jogo);
            telaAtual = TELA_JOGANDO;
        }

        BeginDrawing();
        ClearBackground((Color){ 22, 25, 35, 255 });

        if (telaAtual == TELA_MENU) {
            DrawText("Pânico na Agamenon", 250, 230, 38, RAYWHITE);
            DrawText("Pressione ENTER para iniciar", 285, 300, 22, LIGHTGRAY);
            DrawText("ESC fecha a janela", 345, 335, 18, GRAY);
        } else {
            DesenharJogo(&jogo);

            if (telaAtual == TELA_GAME_OVER) {
                DrawRectangle(0, 0, configuracao.largura, configuracao.altura, (Color){ 0, 0, 0, 150 });
                DrawText("Fim de jogo", 335, 270, 40, RAYWHITE);
                DrawText("Pressione ENTER para tentar de novo", 255, 330, 22, LIGHTGRAY);
            }
        }

        EndDrawing();
    }

    FinalizarJogo(&jogo);
    CloseWindow();
    return 0;
}
