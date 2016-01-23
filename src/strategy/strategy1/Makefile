all: strategy1.o
	gcc -shared -o libA3_elmassari_lequerec_negueloua_efficient.so strategy1.o

strategy1.o: strategy1.c
	gcc -std=c99 -c -fPIC strategy1.c -o strategy1.o

clean:
	rm -f strategy1.o libA3_elmassari_lequerec_negueloua_efficient.so
