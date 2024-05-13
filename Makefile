all: teste server

teste: teste.o archer.o mapa.o mensagem.o gameMaster.o connection.o
	gcc teste.o archer.o mapa.o mensagem.o gameMaster.o connection.o -o teste

server: server.o connection.o archer.o mapa.o mensagem.o gameMaster.o
	gcc server.o connection.o archer.o mapa.o mensagem.o gameMaster.o -o server

teste.o: mensagem.h mapa.h archer.h teste.c 
	gcc -c teste.c

archer.o: mensagem.h mapa.h archer.h archer.c
	gcc -c archer.c

mapa.o: mensagem.h archer.h mapa.h mapa.c
	gcc -c mapa.c

mensagem.o: gameMaster.h archer.h mapa.h mensagem.h mensagem.c
	gcc -c mensagem.c

gameMaster.o: mensagem.h archer.h mapa.h gameMaster.h gameMaster.c
	gcc -c gameMaster.c

server.o: connection.h mensagem.h archer.h mapa.h server.c
	gcc -c server.c

connection.o: mensagem.h connection.h connection.c
	gcc -c connection.c

clean:
	rm -rf *.o teste server
