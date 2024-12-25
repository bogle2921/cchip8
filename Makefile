INCLUDES= -I ./include
FLAGS= -g

OBJECTS=./build/c8memory.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/emu-chip8

./build/c8memory.o:src/c8memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/c8memory.c -c -o ./build/c8memory.o

clean:
	del build\*