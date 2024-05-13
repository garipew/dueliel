#ifndef ARCHER
#define ARCHER


typedef struct arch{

	int x, y;
	int flechas;
	int dir[2];
	char acao;

} Archer;


typedef struct mapa Mapa;

Archer* criarArq(int, int);
void mover(Archer*);
void atirar(Archer*);
void atualizarArq(Archer*, Mapa*);
void apagarArq(Archer*);
#endif
