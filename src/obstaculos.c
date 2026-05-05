#include "obstaculos.h"

#include <stddef.h>
#include <stdlib.h>

#include "config.h"
#include "jogador.h"
#include "raylib.h"

#define QUANTIDADE_SPRITES_CARRO 3
#define QUANTIDADE_SPRITES_ONIBUS 2

static const char *CAMINHOS_SPRITES_CARRO[QUANTIDADE_SPRITES_CARRO] = {
    "assets/veiculos/carro_obstaculo_1.png",
    "assets/veiculos/carro_obstaculo_2.png",
    "assets/veiculos/carro_obstaculo_3.png"
};

static const char *CAMINHOS_SPRITES_ONIBUS[QUANTIDADE_SPRITES_ONIBUS] = {
    "assets/veiculos/onibus_1.png",
    "assets/veiculos/onibus_2.png"
};

static const Vector2 TAMANHOS_SPRITES_CARRO[QUANTIDADE_SPRITES_CARRO] = {
    { 56.0f, 84.0f },
    { 56.0f, 84.0f },
    { 56.0f, 84.0f }
};

static const Vector2 TAMANHOS_SPRITES_ONIBUS[QUANTIDADE_SPRITES_ONIBUS] = {
    { 68.0f, 124.0f },
    { 72.0f, 136.0f }
};

static Texture2D texturasCarro[QUANTIDADE_SPRITES_CARRO] = { 0 };
static Texture2D texturasOnibus[QUANTIDADE_SPRITES_ONIBUS] = { 0 };

static bool TipoObstaculoValido(TipoObstaculo tipo)
{
    return tipo == OBSTACULO_CARRO || tipo == OBSTACULO_ONIBUS;
}

static int ObterQuantidadeSprites(TipoObstaculo tipo)
{
    if (tipo == OBSTACULO_ONIBUS) {
        return QUANTIDADE_SPRITES_ONIBUS;
    }

    return QUANTIDADE_SPRITES_CARRO;
}

static int SortearIndiceSprite(TipoObstaculo tipo)
{
    return GetRandomValue(0, ObterQuantidadeSprites(tipo) - 1);
}

static Vector2 ObterTamanhoSprite(TipoObstaculo tipo, int indiceSprite)
{
    if (tipo == OBSTACULO_ONIBUS && indiceSprite >= 0 && indiceSprite < QUANTIDADE_SPRITES_ONIBUS) {
        return TAMANHOS_SPRITES_ONIBUS[indiceSprite];
    }

    if (indiceSprite >= 0 && indiceSprite < QUANTIDADE_SPRITES_CARRO) {
        return TAMANHOS_SPRITES_CARRO[indiceSprite];
    }

    return TAMANHOS_SPRITES_CARRO[0];
}

static Texture2D ObterTexturaObstaculo(const Obstaculo *obstaculo)
{
    if (obstaculo == NULL) {
        return (Texture2D){ 0 };
    }

    if (obstaculo->tipo == OBSTACULO_ONIBUS &&
        obstaculo->indiceSprite >= 0 &&
        obstaculo->indiceSprite < QUANTIDADE_SPRITES_ONIBUS) {
        return texturasOnibus[obstaculo->indiceSprite];
    }

    if (obstaculo->indiceSprite >= 0 && obstaculo->indiceSprite < QUANTIDADE_SPRITES_CARRO) {
        return texturasCarro[obstaculo->indiceSprite];
    }

    return (Texture2D){ 0 };
}

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

static void DesenharFallbackObstaculo(const Obstaculo *obstaculo)
{
    Color cor = (obstaculo->tipo == OBSTACULO_ONIBUS)
        ? (Color){ 220, 130, 35, 255 }
        : (Color){ 220, 80, 70, 255 };
    const char *texto = (obstaculo->tipo == OBSTACULO_ONIBUS) ? "BUS" : "CAR";

    DrawRectangleRounded(obstaculo->caixaColisao, 0.15f, 6, cor);
    DrawRectangleLinesEx(obstaculo->caixaColisao, 2.0f, RAYWHITE);
    DrawText(texto, (int)obstaculo->caixaColisao.x + 10, (int)obstaculo->caixaColisao.y + 34, 16, RAYWHITE);
}

static Obstaculo *CriarObstaculo(int faixa, float velocidade, TipoObstaculo tipo)
{
    Obstaculo *novoObstaculo = malloc(sizeof(*novoObstaculo));

    if (novoObstaculo == NULL) {
        return NULL;
    }

    if (!TipoObstaculoValido(tipo)) {
        tipo = OBSTACULO_CARRO;
    }

    int indiceSprite = SortearIndiceSprite(tipo);
    Vector2 tamanhoSprite = ObterTamanhoSprite(tipo, indiceSprite);

    novoObstaculo->faixa = faixa;
    novoObstaculo->tipo = tipo;
    novoObstaculo->indiceSprite = indiceSprite;
    novoObstaculo->posicaoY = -tamanhoSprite.y;
    novoObstaculo->velocidade = velocidade;
    novoObstaculo->largura = tamanhoSprite.x;
    novoObstaculo->altura = tamanhoSprite.y;
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

void CarregarTexturasObstaculos(void)
{
    for (int i = 0; i < QUANTIDADE_SPRITES_CARRO; i++) {
        if (!IsTextureValid(texturasCarro[i])) {
            texturasCarro[i] = LoadTexture(CAMINHOS_SPRITES_CARRO[i]);
        }
    }

    for (int i = 0; i < QUANTIDADE_SPRITES_ONIBUS; i++) {
        if (!IsTextureValid(texturasOnibus[i])) {
            texturasOnibus[i] = LoadTexture(CAMINHOS_SPRITES_ONIBUS[i]);
        }
    }
}

void LiberarTexturasObstaculos(void)
{
    for (int i = 0; i < QUANTIDADE_SPRITES_CARRO; i++) {
        if (IsTextureValid(texturasCarro[i])) {
            UnloadTexture(texturasCarro[i]);
            texturasCarro[i] = (Texture2D){ 0 };
        }
    }

    for (int i = 0; i < QUANTIDADE_SPRITES_ONIBUS; i++) {
        if (IsTextureValid(texturasOnibus[i])) {
            UnloadTexture(texturasOnibus[i]);
            texturasOnibus[i] = (Texture2D){ 0 };
        }
    }
}

bool AdicionarObstaculo(ListaObstaculos *lista, int faixa, float velocidade, TipoObstaculo tipo)
{
    if (lista == NULL || faixa < 0 || faixa >= QUANTIDADE_FAIXAS) {
        return false;
    }

    Obstaculo *novoObstaculo = CriarObstaculo(faixa, velocidade, tipo);

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
        Texture2D textura = ObterTexturaObstaculo(obstaculo);

        if (IsTextureValid(textura)) {
            Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
            DrawTexturePro(textura, origem, obstaculo->caixaColisao, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        } else {
            DesenharFallbackObstaculo(obstaculo);
        }
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
