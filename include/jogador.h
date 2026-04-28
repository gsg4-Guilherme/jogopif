#ifndef JOGADOR_H
#define JOGADOR_H

#include "tipos.h"

// Calcula o centro horizontal de uma faixa da pista.
float CalcularCentroFaixa(int faixa);

// Prepara o carro do jogador no centro da pista.
void InicializarJogador(Jogador *jogador);

// Lê o teclado e muda a faixa do carro.
void AtualizarJogador(Jogador *jogador, float delta);

// Desenha um carro provisório. Depois, este ponto pode trocar retângulo por textura.
void DesenharJogador(const Jogador *jogador);

#endif
