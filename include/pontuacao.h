#ifndef PONTUACAO_H
#define PONTUACAO_H

#include "tipos.h"

// Prepara os valores de pontuação no início da partida.
void InicializarPontuacao(EstadoJogo *jogo);

// Atualiza a pontuação com base no tempo de sobrevivência.
void AtualizarPontuacao(EstadoJogo *jogo, float delta);

#endif
