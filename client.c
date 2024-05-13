#include "mensagem.h"
#include "archer.h"
#include "mapa.h"
#include "connection.h"
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



int main(int argc, char* argv[]){
	
	int serverfd;

	Mapa* mapa = criarMapa();
	Archer* arq1 = criarArq(0, 0);

	Mensagem* msg = criarMensagem();
	
	if(argc != 2){
		handle_error("arguments");
	}


	serverfd = findConnection(argv[1], "3490");
	
	// Recebe do serv, le e interpreta.
	
	recv(serverfd, (char*)msg->string, sizeof(msg->string), 0);
	
	lerMensagem(msg);
	interpretarMensagem(msg, arq1, mapa);	
	
	atualizarMapa(mapa, arq1);	
	desenharMapa(mapa);

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
