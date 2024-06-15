#include "gameMaster.h"
#include "mapa.h"
#include "archer.h"
#include "mensagem.h"
#include "connection.h"
#include <stdlib.h>
#include <stdio.h>
#include <lualib.h>
#include <lauxlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>


#define MAXBUFFSIZE 6
#define MAXMSGSIZE 6



/* registrarFlecha(Archer*, Mapa*, Mensagem*)
 * Encontra a posicao final de uma flecha disparada
 * Checa se flecha atinge alguem
 * Prepara mensagem para enviar
 * Usado pelo servidor
*/ 
void registrarFlecha(Archer* arq, Mapa* mapa, Mensagem* msg){

	int xFlecha = arq->x;
	int yFlecha = arq->y;

	for(int i = 0; i < MAXHEIGHT; i++){
			
		xFlecha += msg->x;
		yFlecha += msg->y;

		if(xFlecha < 0){
			xFlecha = 0;
			yFlecha -= msg->y;
			break;
		}

		if(xFlecha > MAXWIDTH - 1){
			xFlecha = MAXWIDTH - 1;
			yFlecha -= msg->y;
			break;
		}
			
		if(yFlecha < 0){
			yFlecha = 0;
			xFlecha -= msg->x;
			break;
		}

		if(yFlecha > MAXHEIGHT - 1){
			yFlecha = MAXHEIGHT - 1;
			xFlecha -= msg->x;
			break;
		}

		if(mapa->pos[yFlecha][xFlecha] < 0 && mapa->pos[yFlecha][xFlecha] > -20 && msg != NULL){
			msg->acao = 'l';
			break;
			// Acertou alguem.
		}	
	}

	if(msg == NULL){
		return;
	}
	msg->x = xFlecha;
	msg->y = yFlecha;
	if(msg->acao != 'l'){
		mapa->pos[yFlecha][xFlecha]++;
		msg->acao = 'f';
	}

}


/* registrarFlechaLocal(Archer*, Mapa*)
 * Calcula trajetoria de uma flecha localmente
 * Usando pelos jogadores
*/

void registrarFlechaLocal(Archer* arq, Mapa* mapa){

	int xFlecha = arq->x;
	int yFlecha = arq->y;

	for(int i = 0; i < MAXHEIGHT; i++){
			
		xFlecha += arq->dir[0];
		yFlecha += arq->dir[1];

		if(xFlecha < 0){
			xFlecha = 0;
			yFlecha -= arq->dir[1];
			break;
		}

		if(xFlecha > MAXWIDTH - 1){
			xFlecha = MAXWIDTH - 1;
			yFlecha -= arq->dir[1];
			break;
		}
			
		if(yFlecha < 0){
			yFlecha = 0;
			xFlecha -= arq->dir[0];
			break;
		}

		if(yFlecha > MAXHEIGHT - 1){
			yFlecha = MAXHEIGHT - 1;
			xFlecha -= arq->dir[0];
			break;
		}

		
	}

	mapa->pos[yFlecha][xFlecha]++;
}


/* escreverTutorial()
 * Escreve na tela os comandos do jogo antes do início da partida
*/

void escreverTutorial(){

	printf("\n\nVocê (@) é um arqueiro preso num campo de grama alta.\n");
	printf("Se esgueirando pela grama alta existe algo ou alguém tentando te matar.\n");
	printf("Sua única escolha é retribuir o favor.\n\n");

	
	printf("Em seu turno, há duas opções [m]over ou [a]tirar.\n");
	printf("Após escolher sua ação, informe em qual direção será realizada. As possíveis direções são:\n\n");

	printf("nw nn ne\nww @ ee\nsw ss se\n\n");

	printf("Por fim, caso a ação escolhida seja [m]over, resta dizer a distancia desejada (valor entre 1-3).\n");

	printf("Boa sorte e que vença o melhor.\n");

}


/* run_client(lua_state)
 * Conecta com um servidor e executa duel
*/

int run_client(lua_State* L){
	
	char* ip = (char*)lua_tostring(L, 1);	
	pthread_t client;

	pthread_create(&client, NULL, client_thread, (void*)ip);
	pthread_join(client, NULL);

}


/* start_server(lua_State*)
 * Abre o servidor para jogadores se conectarem.
*/

int start_server(lua_State* L){

	int servfd;
	
	servfd = findConnection(NULL, "3490");
	
	if(listen(servfd, 10) == -1){
		handle_error("listen");
	}
	
	printf("Open to connect...\n");	
	
	lua_pushnumber(L, servfd);

	return 1;

}


/* run_server(lua_State*)
 * Executa o servidor, validações entre jogadores ocorre aqui
*/

int run_server(lua_State* L){
	
	int servfd = (int)lua_tonumber(L, 1);
	char* ip = (char*)lua_tostring(L, 2);

	pthread_t server, client;

	pthread_create(&server, NULL, server_thread, (void*)&servfd);
	pthread_create(&client, NULL, client_thread, (void*)ip);

	pthread_join(server, NULL);
	pthread_join(client, NULL);

}


/* client_thread()
 * Cria uma thread para executar as funções do client
*/

void* client_thread(void* ip){
	
	char* ipString = (char*)ip;

	int serverfd;

	Mapa* mapa = criarMapa();
	Archer* arq1 = criarArq(0, 0);

	Mensagem* msg = criarMensagem();
	
	serverfd = findConnection(ipString, "3490");
	
	// Recebe do serv, le e interpreta.
	
	recv(serverfd, (char*)msg->string, sizeof(msg->string), 0);
	
	lerMensagem(msg);
	interpretarMensagem(msg, arq1, mapa);	
	
	atualizarMapa(mapa, arq1);	
	escreverTutorial();

	do{
		// Recebe mensagem do serv
		recv(serverfd, (char*)msg->string, sizeof(msg->string), 0);
		lerMensagem(msg);
		interpretarMensagem(msg, arq1, mapa);	
		
		desenharMapa(mapa);
		if(msg->acao != 'o' && msg->acao != 'k'){
			// Faz acao
			atualizarArq(arq1, mapa, msg);
			atualizarMapa(mapa, arq1);
			desenharMapa(mapa);
		}

		// Envia mensagem pro serv
		escreverMensagem(msg);
		send(serverfd, (char*)msg->string, sizeof(msg->string), 0);	
	}while(msg->acao != 'k' && msg->acao != 'o');
	
	
	if(msg->acao == 'k'){
		printf("Voce ganhou!!!\n");
	}else if(msg->acao == 'o'){
		printf("Voce perdeu...\n");
	}

	apagarArq(arq1);
	apagarMapa(mapa);

	close(serverfd);

	return 0;

}


/* server_thread()
 * Cria uma thread para executar as funções do server
*/


void* server_thread(void* serverfd){
	
	int* servfd = (int*)serverfd;
	int archfd[2];
	struct addrinfo hint, *p, *servinfo;

	int turno = 0;

	Mensagem* msg = criarMensagem();
	int num_bytes;
	
	srand(time(NULL));
	Archer* arqs[] = {criarArq(rand() % MAXWIDTH, rand() % MAXHEIGHT), criarArq(rand() % MAXWIDTH, rand() % MAXHEIGHT)};
	Mapa* mapa = criarMapa();

	archfd[0] = conectarJogador(*servfd, msg, arqs[0]);
	archfd[1] = conectarJogador(*servfd, msg, arqs[1]);
	
	msg->acao = 't';

	do{

		/*		Game Loop
		 *	Envia mensagem pro dono do turno
		 *	Recebe resposta
		 *	Processa resposta
		 *	Passa o turno.
		 */ 
		escreverMensagem(msg);	
		if(send(archfd[turno%2], (char*)msg->string, sizeof(msg->string), 0) == -1){
			handle_error("send");
		}

		if(num_bytes = recv(archfd[turno%2], (char*)msg->string, sizeof(msg->string), 0) == -1){
			handle_error("recv");
		}

		lerMensagem(msg);
		interpretarMensagem(msg, arqs[turno%2], mapa);
		
		atualizarMapaServer(mapa, arqs[0], arqs[1], msg);
		
		turno++;
			
	} while(msg->acao != 'k');	
	
	apagarArq(arqs[0]);
	apagarArq(arqs[1]);
	apagarMapa(mapa);

	close(archfd[0]);
	close(archfd[1]);
	close(*servfd);


	return 0;

}
