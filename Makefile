all: client server
	clear

update:
	git pull origin main

client: client.o archer.o mapa.o mensagem.o gameMaster.o connection.o
	gcc out/client.o out/archer.o out/mapa.o out/mensagem.o out/gameMaster.o out/connection.o -o client

server: server.o connection.o archer.o mapa.o mensagem.o gameMaster.o
	gcc out/server.o out/connection.o out/archer.o out/mapa.o out/mensagem.o out/gameMaster.o -o server

client.o: src/mensagem.h src/mapa.h src/archer.h src/client.c 
	gcc -c src/client.c
	mv client.o out

archer.o: src/mensagem.h src/mapa.h src/archer.h src/archer.c
	gcc -c src/archer.c
	mv archer.o out

mapa.o: src/mensagem.h src/archer.h src/mapa.h src/mapa.c
	gcc -c src/mapa.c
	mv mapa.o out

mensagem.o: src/gameMaster.h src/archer.h src/mapa.h src/mensagem.h src/mensagem.c
	gcc -c src/mensagem.c
	mv mensagem.o out

gameMaster.o: src/mensagem.h src/archer.h src/mapa.h src/gameMaster.h src/gameMaster.c
	gcc -c src/gameMaster.c
	mv gameMaster.o out

server.o: src/connection.h src/mensagem.h src/archer.h src/mapa.h src/server.c
	gcc -c src/server.c
	mv server.o out

connection.o: src/mensagem.h src/connection.h src/connection.c
	gcc -c src/connection.c
	mv connection.o out

clean:
	rm -rf out/*.o client server
