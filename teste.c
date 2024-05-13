#include "archer.h"
#include "mapa.h"
#include <stdlib.h>
#include <stdio.h>


int main(){
	
	Mapa* mapa = criarMapa();
	Archer* arq1 = criarArq(0, 0);
	
	
	atualizarMapa(mapa, arq1);	
	desenharMapa(mapa);

	do{
		atualizarArq(arq1, mapa);
		atualizarMapa(mapa, arq1);
		desenharMapa(mapa);

	}while(arq1->acao != 'q');

	apagarArq(arq1);
	apagarMapa(mapa);

	return 0;

}
