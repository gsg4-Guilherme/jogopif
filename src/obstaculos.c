#include "obstaculos.h"

#include <stddef.h>
#include <stdlib.h>

#include "config.h"
#include "jogador.h"
#include "raylib.h"

static void AtualizarCaixaObstaculo(Obstaculo *obstaculo)
{
    if (obstaculo == NULL) {
        return;
    }

    obstaculo->caixaColisao = (Rectangle){
        CalcularCentroFaixa(obstaculo->faixa) - (obstaculo->largura / 2.0f),
        obstaculo->posicaoY,
        obstaculo->largura,
        obstaculo->altura
    };
}

static Obstaculo *CriarObstaculo(int faixa, float velocidade)
{
    Obstaculo *novoObstaculo = malloc(sizeof(*novoObstaculo));

    if (novoObstaculo == NULL) {
        return NULL;
    }

    novoObstaculo->faixa = faixa;
    novoObstaculo->posicaoY = -90.0f;
    novoObstaculo->velocidade = velocidade;
    novoObstaculo->largura = 62.0f;
    novoObstaculo->altura = 88.0f;
    novoObstaculo->proximo = NULL;
    AtualizarCaixaObstaculo(novoObstaculo);

    return novoObstaculo;
}

void InicializarListaObstaculos(ListaObstaculos *lista)
{
    if (lista == NULL) {
        return;
    }

    lista->inicio = NULL;
    lista->quantidade = 0;
}

bool AdicionarObstaculo(ListaObstaculos *lista, int faixa, float velocidade)
{
    if (lista == NULL || faixa < 0 || faixa >= QUANTIDADE_FAIXAS) {
        return false;
    }

    Obstaculo *novoObstaculo = CriarObstaculo(faixa, velocidade);

    if (novoObstaculo == NULL) {
        return false;
    }

    // Inserção no início: simples, rápida e suficiente para uma lista encadeada de obstáculos.
    novoObstaculo->proximo = lista->inicio;
    lista->inicio = novoObstaculo;
    lista->quantidade++;
    return true;
}

void AtualizarObstaculos(ListaObstaculos *lista, float delta)
{
    if (lista == NULL) {
        return;
    }

    Obstaculo **ponteiroAtual = &lista->inicio;

    while (*ponteiroAtual != NULL) {
        Obstaculo *obstaculoAtual = *ponteiroAtual;
        obstaculoAtual->posicaoY += obstaculoAtual->velocidade * delta;
        AtualizarCaixaObstaculo(obstaculoAtual);

        if (obstaculoAtual->posicaoY > ALTURA_JANELA + 120.0f) {
            *ponteiroAtual = obstaculoAtual->proximo;
            free(obstaculoAtual);
            lista->quantidade--;
        } else {
            ponteiroAtual = &obstaculoAtual->proximo;
        }
    }
}

void DesenharObstaculos(const ListaObstaculos *lista)
{
    if (lista == NULL) {
        return;
    }

    for (const Obstaculo *obstaculo = lista->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        DrawRectangleRounded(obstaculo->caixaColisao, 0.15f, 6, (Color){ 220, 80, 70, 255 });
        DrawRectangleLinesEx(obstaculo->caixaColisao, 2.0f, RAYWHITE);
        DrawText("BUS", (int)obstaculo->caixaColisao.x + 12, (int)obstaculo->caixaColisao.y + 34, 16, RAYWHITE);
    }
}

bool VerificarColisaoJogadorObstaculos(const Jogador *jogador, const ListaObstaculos *lista)
{
    if (jogador == NULL || lista == NULL) {
        return false;
    }

    for (const Obstaculo *obstaculo = lista->inicio; obstaculo != NULL; obstaculo = obstaculo->proximo) {
        if (CheckCollisionRecs(jogador->caixaColisao, obstaculo->caixaColisao)) {
            return true;
        }
    }

    return false;
}

void LiberarObstaculos(ListaObstaculos *lista)
{
    if (lista == NULL) {
        return;
    }

    Obstaculo *obstaculo = lista->inicio;

    while (obstaculo != NULL) {
        Obstaculo *proximo = obstaculo->proximo;
        free(obstaculo);
        obstaculo = proximo;
    }

    InicializarListaObstaculos(lista);
}
