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
#include "connection.h"


#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); }while(0)

int findConnection(char* ip, char* port){
	
	int serverfd;
	struct addrinfo hint, *p, *servinfo; 

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	if(ip == NULL){
		hint.ai_flags = AI_PASSIVE;
	}

	if(getaddrinfo(ip, port, &hint, &servinfo) != 0){
		handle_error("addrinfo");
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
	
		serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		
		if(serverfd == -1){
			continue;
		}
		
		if(ip == NULL){
			if(bind(serverfd, p->ai_addr, p->ai_addrlen) == -1){
				close(serverfd);
				continue;
			}
		} else{
			if(connect(serverfd, p->ai_addr, p->ai_addrlen) == -1){
				close(serverfd);
				continue;
			}
		}
	

		break;
	
	}
		
	if(p == NULL){
		handle_error("addr");
	}

	freeaddrinfo(servinfo);
	

	return serverfd;

}


/* connectPlayer(int)
 * Aceita tentativa de conexão e retorna o fd
*/ 
int conectarJogador(int servfd, Mensagem* msg, Archer* arq){
	
	struct sockaddr_storage client_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];
	int playerfd;
		
	sin_size = sizeof(playerfd);
	playerfd = accept(servfd, (struct sockaddr*)&client_addr, &sin_size);
		
	inet_ntop(client_addr.ss_family, &(((struct sockaddr_in*)&client_addr)->sin_addr), s, sizeof(s));
	printf("connection from: %s\n", s);

	mensagemInicial(msg, arq, playerfd);
	
	return playerfd;
}
