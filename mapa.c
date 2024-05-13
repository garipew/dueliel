#include "mapa.h"
#include <stdlib.h>
#include <stdio.h>


/* criarMapa()
 * Aloca memoria para um novo mapa e retorna ponteiro para
 * nova struct Mapa
*/

Mapa* criarMapa(){

	Mapa* mapa = malloc(sizeof(*mapa));
	
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			mapa->pos[i][j] = 0;
		}
	}

	return mapa;

}


/* atualizarMapa(Archer*)
 * Atualiza posicoes no mapa
 * Jogador é registrado como -1.
 * Qualquer valor < -1 indica flechas na pos do Jogador.
 * Quantidade de flechas são armazenadas com valores positivos nas pos.
 *
*/

void atualizarMapa(Mapa* mapa, Archer* arq){
	
	int xFlecha, yFlecha;	

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if(mapa->pos[i][j] < 0){
				mapa->pos[i][j]++;
				mapa->pos[i][j] *= -1;
			}
		}
	}

	mapa->pos[arq->y][arq->x] *= -1;
	mapa->pos[arq->y][arq->x]--;
	
	
	if(arq->acao == 'a'){
		// Registra flecha atirada, deve ser feito no servidor?
		xFlecha = arq->x;
		yFlecha = arq->y;

		for(int i = 0; i < 8; i++){
			
			xFlecha += arq->dir[0];
			yFlecha += arq->dir[1];

			if(xFlecha < 0){
				xFlecha = 0;
				yFlecha -= arq->dir[1];
				break;
			}

			if(xFlecha > 7){
				xFlecha = 7;
				yFlecha -= arq->dir[1];
				break;
			}
			
			if(yFlecha < 0){
				yFlecha = 0;
				xFlecha -= arq->dir[0];
				break;
			}

			if(yFlecha > 7){
				yFlecha = 7;
				xFlecha -= arq->dir[0];
				break;
			}

		
		}

		mapa->pos[yFlecha][xFlecha]++;

	}

}


/* desenharMapa(Archer*)
 * Desenha o mapa e o jogador
*/ 
void desenharMapa(Mapa* mapa){

	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			
			if(mapa->pos[i][j] < 0){
				printf("@ ");
			}else if(mapa->pos[i][j] == 0){
				printf(". ");
			} else{
				printf("x ");
			}

		}
		printf("\n");
	}

	printf("\n\n");
}


/* apagarMapa(Mapa*)
 * Libera memoria alocada na funcao 'criarMapa()'
*/

void apagarMapa(Mapa* mapa){
	
	if(mapa == NULL){
		return;
	}

	free(mapa);

}
