#include "archer.h"
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>


/* criarArq(int, int)
 * Aloca memoria e retorna um ponteiro para struct Archer
*/

Archer* criarArq(int x, int y){

	Archer* arq = malloc(sizeof(*arq));
	
	arq->x = x;
	arq->y = y;
	arq->flechas = 3;
	arq->dir[0] = 0;
	arq->dir[1] = 0;
	arq->acao = ' ';	

	return arq;
}



/* mudarDirecao(Archer*)
 * Recebe ponteiro para uma struct Archer e atualiza sua direcao
*/

void mudarDirecao(Archer* arq){
	
	char dir[10];
	
	printf("Direcao: ");
	fgets(dir, 10, stdin);
	sscanf(dir, "%d %d", &(arq->dir[0]), &(arq->dir[1]));

}


/* mover(Archer*)
 * Recebe ponteiro para uma struct Archer e atualiza sua posicao
*/

void mover(Archer* arq){
	
	mudarDirecao(arq);
	arq->x += arq->dir[0];
	arq->y += arq->dir[1];
	
	if(arq->x > 7){
		arq->x = 7;
	} else if(arq->x < 0){
		arq->x = 0;
	}

	if(arq->y > 7){
		arq->y = 7;
	} else if(arq->y < 0){
		arq->y = 0;
	}
}


/* atirar(Archer*)
 * Recebe ponteiro para uma struct Archer, e, caso possua flechas
 * atira uma
*/

void atirar(Archer* arq){

	if(arq->flechas == 0){
		arq->acao = 'i';
		printf("Flechas insuficientes.\n");
		return;
	}


	mudarDirecao(arq);
	arq->flechas--;

	printf("Flecha disparada de %d %d na direcao %d %d\n", arq->x, arq->y, arq->dir[0], arq->dir[1]);

}


/* coletarFlecha(Archer*, Mapa*)
 * Coleta uma flecha no chao caso mochila nao esteja cheia e esteja proximo
*/

void coletarFlecha(Archer* arq, Mapa* mapa){
	
	if(mapa->pos[arq->y][arq->x] > 0){
		if(arq->flechas < 3){
			arq->flechas++;
			mapa->pos[arq->y][arq->x]--;
			printf("Flechas: %d\n", arq->flechas);
		}
	}
}


/* atualizarAcao(Archer*)
 * Atualiza proxima açao de um Archer
*/

void atualizarAcao(Archer* arq){
	
	char acao[4];

	printf("\t\tSeu turno\n");
	printf("Escolha sua proxima ação, digite [m] para mover ou [a] para atirar.\n");

	printf("Sua escolha: ");
	
	fgets(acao, 4, stdin);
	sscanf(acao, "%c", &(arq->acao));

}


/* atualizar(Archer*)
 * Atualiza todas as propriedades de um Archer
*/

void atualizarArq(Archer* arq, Mapa* mapa){
	
	atualizarAcao(arq);

	switch(arq->acao){
		
		case 'i':
			atualizarAcao(arq);
		case 'a':
			atirar(arq);
			break;
		case 'm':
			mover(arq);
			coletarFlecha(arq, mapa);
			break;
		
	}
}


/* apagarArq(Archer*)
 * Libera memória alocada ao utilizar a função 'criarArq(int, int)'
*/ 

void apagarArq(Archer* arq){
	
	if(arq == NULL){
		return;
	}

	free(arq);

}
