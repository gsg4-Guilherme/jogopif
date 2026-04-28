#include "pista.h"

#include <stddef.h>

#include "config.h"
#include "raylib.h"

void InicializarPista(int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA])
{
    for (int linha = 0; linha < LINHAS_PISTA; linha++) {
        for (int coluna = 0; coluna < COLUNAS_PISTA; coluna++) {
            pistaLogica[linha][coluna] = 0;
        }
    }
}

void AtualizarPista(
    int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA],
    const Jogador *jogador,
    const ListaObstaculos *obstaculos
)
{
    InicializarPista(pistaLogica);

    if (jogador != NULL && jogador->faixaAtual >= 0 && jogador->faixaAtual < COLUNAS_PISTA) {
        pistaLogica[LINHAS_PISTA - 2][jogador->faixaAtual] = 1;
    }

    if (obstaculos == NULL) {
        return;
    }

    for (const Obstaculo *obstaculo = obstaculos->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        int linha = (int)((obstaculo->posicaoY / ALTURA_JANELA) * LINHAS_PISTA);

        if (linha >= 0 && linha < LINHAS_PISTA && obstaculo->faixa >= 0 && obstaculo->faixa < COLUNAS_PISTA) {
            pistaLogica[linha][obstaculo->faixa] = 2;
        }
    }
}

void DesenharPista(const int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA])
{
    float larguraPista = 450.0f;
    float margemEsquerda = (LARGURA_JANELA - larguraPista) / 2.0f;
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;
    float alturaLinha = (float)ALTURA_JANELA / LINHAS_PISTA;

    DrawRectangle((int)margemEsquerda, 0, (int)larguraPista, ALTURA_JANELA, (Color){ 42, 45, 52, 255 });

    for (int faixa = 1; faixa < QUANTIDADE_FAIXAS; faixa++) {
        int x = (int)(margemEsquerda + (larguraFaixa * faixa));
        DrawLine(x, 0, x, ALTURA_JANELA, YELLOW);
    }

    // A matriz aparece como marcações discretas para ajudar a explicar o requisito na apresentação.
    for (int linha = 0; linha < LINHAS_PISTA; linha++) {
        for (int coluna = 0; coluna < COLUNAS_PISTA; coluna++) {
            if (pistaLogica[linha][coluna] != 0) {
                int x = (int)(margemEsquerda + (larguraFaixa * coluna) + (larguraFaixa / 2.0f) - 5.0f);
                int y = (int)((alturaLinha * linha) + 8.0f);
                Color cor = pistaLogica[linha][coluna] == 1 ? BLUE : RED;
                DrawCircle(x, y, 5.0f, cor);
            }
        }
    }
}
