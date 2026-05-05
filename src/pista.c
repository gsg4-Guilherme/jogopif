#include "pista.h"

#include <stddef.h>

#include "config.h"
#include "raylib.h"

#define LARGURA_PISTA_VISUAL 450.0f
#define LARGURA_ACOSTAMENTO 24.0f

static float CalcularMargemEsquerdaPista(void)
{
    return (LARGURA_JANELA - LARGURA_PISTA_VISUAL) / 2.0f;
}

static float NormalizarDeslocamento(float deslocamento, float intervalo)
{
    while (deslocamento >= intervalo) {
        deslocamento -= intervalo;
    }

    while (deslocamento < 0.0f) {
        deslocamento += intervalo;
    }

    return deslocamento;
}

static void DesenharArvore(float centroX, float baseY, float escala)
{
    int troncoLargura = (int)(10.0f * escala);
    int troncoAltura = (int)(28.0f * escala);
    int troncoX = (int)(centroX - (troncoLargura / 2));
    int troncoY = (int)(baseY - troncoAltura);

    DrawEllipse((int)centroX + 6, (int)baseY + 1, 24.0f * escala, 7.0f * escala, (Color){ 11, 32, 25, 120 });
    DrawRectangle(troncoX, troncoY, troncoLargura, troncoAltura, (Color){ 104, 68, 38, 255 });
    DrawRectangle(troncoX + 2, troncoY, 2, troncoAltura, (Color){ 145, 94, 50, 255 });

    DrawCircle((int)centroX, (int)(baseY - 42.0f * escala), 25.0f * escala, (Color){ 28, 112, 56, 255 });
    DrawCircle((int)(centroX - 18.0f * escala), (int)(baseY - 32.0f * escala), 20.0f * escala, (Color){ 34, 134, 65, 255 });
    DrawCircle((int)(centroX + 18.0f * escala), (int)(baseY - 31.0f * escala), 20.0f * escala, (Color){ 23, 96, 50, 255 });
    DrawCircle((int)(centroX + 2.0f * escala), (int)(baseY - 22.0f * escala), 18.0f * escala, (Color){ 42, 145, 70, 255 });
}

static void DesenharGramado(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    static const int posicoesGrama[] = { 18, 47, 91, 136, 180, 711, 756, 804, 846, 879 };
    float deslocamentoGrama = NormalizarDeslocamento(deslocamentoCenario * 0.45f, 42.0f);

    DrawRectangle(0, 0, LARGURA_JANELA, ALTURA_JANELA, (Color){ 25, 82, 52, 255 });
    DrawRectangle(0, 0, (int)margemEsquerda, ALTURA_JANELA, (Color){ 30, 92, 57, 255 });
    DrawRectangle((int)(margemEsquerda + larguraPista), 0, (int)margemEsquerda, ALTURA_JANELA, (Color){ 30, 92, 57, 255 });

    for (int linha = -1; linha < 18; linha++) {
        int y = (int)(deslocamentoGrama + (linha * 42.0f));

        for (int i = 0; i < (int)(sizeof(posicoesGrama) / sizeof(posicoesGrama[0])); i++) {
            int altura = 7 + ((linha + i) % 4);
            DrawLine(posicoesGrama[i], y, posicoesGrama[i] + 4, y - altura, (Color){ 55, 132, 73, 180 });
        }
    }
}

static void DesenharArvoresLaterais(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    float deslocamentoArvore = NormalizarDeslocamento(deslocamentoCenario * 0.75f, 120.0f);

    for (int i = -1; i < 8; i++) {
        float y = deslocamentoArvore + (i * 120.0f);
        float xEsquerda = margemEsquerda - 145.0f + ((i % 2) * 54.0f);
        float xDireita = margemEsquerda + larguraPista + 92.0f + ((i % 2) * 48.0f);
        float escalaEsquerda = (i % 2 == 0) ? 0.9f : 1.08f;
        float escalaDireita = (i % 2 == 0) ? 1.05f : 0.92f;

        DesenharArvore(xEsquerda, y, escalaEsquerda);
        DesenharArvore(xDireita, y + 58.0f, escalaDireita);
    }
}

static void DesenharAsfalto(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    static const int marcasX[] = { 32, 108, 176, 248, 327, 404 };
    float deslocamentoMarcas = NormalizarDeslocamento(deslocamentoCenario * 0.6f, 58.0f);

    DrawRectangle((int)margemEsquerda, 0, (int)larguraPista, ALTURA_JANELA, (Color){ 42, 45, 52, 255 });
    DrawRectangle((int)margemEsquerda, 0, 10, ALTURA_JANELA, (Color){ 35, 38, 44, 255 });
    DrawRectangle((int)(margemEsquerda + larguraPista - 10.0f), 0, 10, ALTURA_JANELA, (Color){ 35, 38, 44, 255 });

    for (int linha = -1; linha < 15; linha++) {
        int y = (int)(deslocamentoMarcas + (linha * 58.0f));

        for (int i = 0; i < (int)(sizeof(marcasX) / sizeof(marcasX[0])); i++) {
            int x = (int)margemEsquerda + marcasX[i] + ((linha % 2) * 9);
            int larguraMarca = 2 + ((linha + i) % 4);
            int alturaMarca = 8 + ((linha + i) % 6);
            Color cor = ((linha + i) % 2 == 0)
                ? (Color){ 55, 58, 65, 150 }
                : (Color){ 30, 33, 39, 120 };

            DrawRectangle(x, y, larguraMarca, alturaMarca, cor);
        }
    }
}

static void DesenharAcostamento(float margemEsquerda, float larguraPista)
{
    int xEsquerda = (int)(margemEsquerda - LARGURA_ACOSTAMENTO);
    int xDireita = (int)(margemEsquerda + larguraPista);

    DrawRectangle(xEsquerda, 0, (int)LARGURA_ACOSTAMENTO, ALTURA_JANELA, (Color){ 86, 90, 84, 255 });
    DrawRectangle(xDireita, 0, (int)LARGURA_ACOSTAMENTO, ALTURA_JANELA, (Color){ 86, 90, 84, 255 });
    DrawLine((int)margemEsquerda, 0, (int)margemEsquerda, ALTURA_JANELA, (Color){ 210, 210, 190, 255 });
    DrawLine((int)(margemEsquerda + larguraPista), 0, (int)(margemEsquerda + larguraPista), ALTURA_JANELA, (Color){ 210, 210, 190, 255 });
    DrawLine(xEsquerda, 0, xEsquerda, ALTURA_JANELA, (Color){ 65, 69, 63, 255 });
    DrawLine(xDireita + (int)LARGURA_ACOSTAMENTO, 0, xDireita + (int)LARGURA_ACOSTAMENTO, ALTURA_JANELA, (Color){ 65, 69, 63, 255 });
}

static void DesenharFaixasTracejadas(float margemEsquerda, float larguraPista, float deslocamentoCenario)
{
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;
    float deslocamentoFaixa = NormalizarDeslocamento(deslocamentoCenario, 92.0f);

    for (int faixa = 1; faixa < QUANTIDADE_FAIXAS; faixa++) {
        int x = (int)(margemEsquerda + (larguraFaixa * faixa));

        for (int i = -1; i < 9; i++) {
            int y = (int)(deslocamentoFaixa + (i * 92.0f));
            DrawRectangle(x - 3, y, 6, 48, (Color){ 235, 206, 62, 255 });
            DrawRectangle(x - 1, y + 4, 2, 40, (Color){ 250, 232, 118, 255 });
        }
    }
}

static void DesenharMarcadoresLogicos(const int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA], float margemEsquerda, float larguraPista)
{
    float larguraFaixa = larguraPista / QUANTIDADE_FAIXAS;
    float alturaLinha = (float)ALTURA_JANELA / LINHAS_PISTA;

    for (int linha = 0; linha < LINHAS_PISTA; linha++) {
        for (int coluna = 0; coluna < COLUNAS_PISTA; coluna++) {
            if (pistaLogica[linha][coluna] != 0) {
                int x = (int)(margemEsquerda + (larguraFaixa * coluna) + (larguraFaixa / 2.0f));
                int y = (int)((alturaLinha * linha) + 10.0f);
                Color cor = pistaLogica[linha][coluna] == 1
                    ? (Color){ 80, 160, 255, 90 }
                    : (Color){ 255, 80, 70, 90 };

                DrawCircle(x, y, 4.0f, cor);
            }
        }
    }
}

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

void DesenharPista(const int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA], float deslocamentoCenario)
{
    float larguraPista = LARGURA_PISTA_VISUAL;
    float margemEsquerda = CalcularMargemEsquerdaPista();

    DesenharGramado(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharArvoresLaterais(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharAcostamento(margemEsquerda, larguraPista);
    DesenharAsfalto(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharFaixasTracejadas(margemEsquerda, larguraPista, deslocamentoCenario);
    DesenharMarcadoresLogicos(pistaLogica, margemEsquerda, larguraPista);
}
