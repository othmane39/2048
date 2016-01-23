all: strategy.o
	gcc -shared -o libA3_elmassari_lequerec_negueloua_fast.so strategy.o

strategy.o: strategy.c
	gcc -std=c99 -c -fPIC strategy.c -o strategy.o

clean:
	rm -f strategy.o libA3_elmassari_lequerec_negueloua_fast.so
