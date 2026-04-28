#ifndef RANKING_H
#define RANKING_H

// Lê o melhor recorde salvo em arquivo. Se não existir, retorna zero.
int CarregarMelhorPontuacao(const char *caminho);

// Salva a pontuação atual somente quando ela superar o recorde anterior.
void SalvarPontuacaoSeRecorde(const char *caminho, int pontuacao);

#endif
