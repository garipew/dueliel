#include <stdio.h>
#include <stdlib.h>
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
#include "connection.h"
#include "mensagem.h"
#include "archer.h"
#include "mapa.h"
#include <time.h>


#define MAXBUFFSIZE 6
#define MAXMSGSIZE 6

int main(){
	
	int servfd, archfd[2];
	struct addrinfo hint, *p, *servinfo;
	int client = 0;

	int game = 1;
	int turno = 0;

	Mensagem* msg = criarMensagem();
	int num_bytes;
	
	srand(time(NULL));
	Archer* arqs[] = {criarArq(rand() % MAXWIDTH, rand() % MAXHEIGHT), criarArq(rand() % MAXWIDTH, rand() % MAXHEIGHT)};
	Mapa* mapa = criarMapa();

	servfd = findConnection(NULL, "3490");
	
	if(listen(servfd, 10) == -1){
		handle_error("listen");
	}
	
	printf("Server open for connection...\n");

	
	printf("Arq1 em: %d %d\nArq2 em: %d %d\n", arqs[0]->x, arqs[0]->y, arqs[1]->x, arqs[1]->y);	
	archfd[0] = conectarJogador(servfd, msg, arqs[0]);
	archfd[1] = conectarJogador(servfd, msg, arqs[1]);
	
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
		desenharMapa(mapa);
		
		turno++;
			
	} while(msg->acao != 'k');	
	
	apagarArq(arqs[0]);
	apagarArq(arqs[1]);
	apagarMapa(mapa);

	close(archfd[0]);
	close(archfd[1]);
	close(servfd);


	return 0;

}
