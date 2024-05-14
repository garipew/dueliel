#ifndef MAP
#define MAP
#include "mensagem.h"
#define MAXWIDTH 16
#define MAXHEIGHT 16

struct mapa{

	int pos[MAXHEIGHT][MAXWIDTH];

};


Mapa* criarMapa();
void atualizarMapa(Mapa*, Archer*);
void atualizarMapaServer(Mapa*, Archer*, Archer*, Mensagem*);
void desenharMapa(Mapa*);
void apagarMapa(Mapa*);
#endif
