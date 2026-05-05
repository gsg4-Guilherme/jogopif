#ifndef PISTA_H
#define PISTA_H

#include "tipos.h"

// Zera a matriz lógica da pista.
void InicializarPista(int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA]);

// Atualiza a matriz com a posição aproximada do jogador e dos obstáculos.
void AtualizarPista(
    int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA],
    const Jogador *jogador,
    const ListaObstaculos *obstaculos
);

// Desenha a pista e o ambiente lateral com deslocamento de movimento.
void DesenharPista(const int pistaLogica[LINHAS_PISTA][COLUNAS_PISTA], float deslocamentoCenario);

#endif
