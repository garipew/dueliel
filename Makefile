teste: teste.o archer.o mapa.o
	gcc teste.o archer.o mapa.o -o teste

teste.o: mapa.h archer.h teste.c 
	gcc -c teste.c

archer.o: mapa.h archer.h archer.c
	gcc -c archer.c

mapa.o: mapa.h mapa.c
	gcc -c mapa.c

clean:
	rm -rf *.o teste
