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

#define MAXBUFFSIZE 3 
#define MAXMSGSIZE 3

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); }while(0)




void* get_in_addr(struct sockaddr* sa){

	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*) sa)->sin_addr);
	}
	
	return &(((struct sockaddr_in6*) sa)->sin6_addr);

}


int main(int argc, char* argv[]){
	
	int serverfd; // hold server file descriptor value
	
	struct addrinfo hint, *servinfo, *p; // used to find server address
	socklen_t sin_size; // sizeof server address struct
	char s[INET6_ADDRSTRLEN]; // hold printable value of server IP
	char buffer[MAXBUFFSIZE]; // recv messages from server 
	char msg[MAXMSGSIZE]; 
	int num_bytes;
	int condicao;
	
	if(argc != 2){
		handle_error("arguments");
	}
	
	// clear and fill hint struct
	memset(&hint, 0, sizeof(hint)); 
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	// find possible addresses to connect	
	if(getaddrinfo(argv[1], "3490", &hint, &servinfo) != 0){
		handle_error("addrinfo");
	}
	
	// filter address list and make connection
	for(p = servinfo; p != NULL; p = p->ai_next){
		
		if((serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){ printf("socket\n"); continue; }
		
		if(connect(serverfd, p->ai_addr, p->ai_addrlen) == -1){ close(serverfd); printf("connect\n"); continue; }

		break;

	}

	
	if(p == NULL){ // connection not established
		handle_error("connect error");
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)&p->ai_addr), s, sizeof(s));
	printf("connected to: %s\n", s);
		
	freeaddrinfo(servinfo);
	
	do{	
       		if((num_bytes = recv(serverfd, (char*)buffer, MAXBUFFSIZE -1, 0)) == -1){
			handle_error("recv");
		}
		
		buffer[num_bytes] = '\0';
		condicao = strcmp(buffer, "exit\n");
		if(num_bytes == 0 || condicao == 0){
			printf("Server closed connection\n");
			break;
		}
	
		printf("%s\n", buffer);
		
	} while(condicao != 0);

	close(serverfd);

	return 0;

}


