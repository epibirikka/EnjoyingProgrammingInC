FLAGS= -O2 -std=c99 -Wno-missing-braces
RAYLIB_PATH=C:/raylib/raylib
RAYLIB_LINK= -I $(RAYLIB_PATH)/src -I $(RAYLIB_PATH)/src/external -L $(RAYLIB_PATH)/src -Wall -lraylib -lopengl32 -lgdi32 -lwinmm

all:
	gcc main.c -o Main $(FLAGS) $(RAYLIB_LINK)
