RAYLIB=C:/raylib/raylib
FLAGS=-O2 -I $(RAYLIB)/src -I $(RAYLIB)/src/external
LINK=-L $(RAYLIB) -lraylib -lopengl32 -lgdi32 -lwinmm

all:
	gcc -c main.c ball.c $(FLAGS)
	gcc -o main main.c ball.c $(LINK)	
	rm *.o
	
