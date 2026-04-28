#ifndef ENGINE_H
#define ENGINE_H

#include "tipos.h"

// Cria a configuração padrão da janela. Deixe os ajustes globais concentrados aqui.
ConfiguracaoJanela CriarConfiguracaoPadrao(void);

// Executa a janela, o loop principal e o controle de telas.
int RodarEngine(ConfiguracaoJanela configuracao);

#endif
