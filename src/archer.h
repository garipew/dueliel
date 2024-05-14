#ifndef ARCHER
#define ARCHER

#include "mensagem.h"

struct arch{

	int x, y;
	int flechas;
	int dir[2];
	char acao;

};


Archer* criarArq(int, int);
void mover(Archer*, Mensagem*);
void atirar(Archer*, Mensagem*);
void atualizarArq(Archer*, Mapa*, Mensagem*);
void apagarArq(Archer*);
#endif
