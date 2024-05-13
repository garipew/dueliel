#ifndef MAP
#define MAP
#include "archer.h"


struct mapa{

	int pos[8][8];

};


Mapa* criarMapa();
void atualizarMapa(Mapa*, Archer*);
void desenharMapa(Mapa*);
void apagarMapa(Mapa*);
#endif
