all: build run
build: geometria.c inimigo.c recorde.c projetil.c main.c
	gcc -c geometria.c
	gcc -c inimigo.c
	gcc -c recorde.c
	gcc -c projetil.c
	gcc -o Game geometria.o inimigo.o recorde.o projetil.o main.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm
run: Game
	./Game
cleano: *.o
	rm *.o
	clear
clean: Game *.o
	rm Game *.o
	clear