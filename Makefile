INCLUDES= -I ./include
FLAGS= -g

OBJECTS=./build/c8memory.o ./build/c8stack.o ./build/c8keyboard.o ./build/chip8.o ./build/c8screen.o
all: ${OBJECTS}
	gcc ${FLAGS} ${INCLUDES} ./src/main.c ${OBJECTS} -L ./lib -lmingw32 -lSDL2main -lSDL2 -o ./bin/emu-chip8

./build/c8memory.o:src/c8memory.c
	gcc ${FLAGS} ${INCLUDES} ./src/c8memory.c -c -o ./build/c8memory.o

./build/c8stack.o:src/c8stack.c
	gcc ${FLAGS} ${INCLUDES} ./src/c8stack.c -c -o ./build/c8stack.o

./build/c8keyboard.o:src/c8keyboard.c
	gcc ${FLAGS} ${INCLUDES} ./src/c8keyboard.c -c -o ./build/c8keyboard.o

./build/chip8.o:src/chip8.c
	gcc ${FLAGS} ${INCLUDES} ./src/chip8.c -c -o ./build/chip8.o

./build/c8screen.o:src/c8screen.c
	gcc ${FLAGS} ${INCLUDES} ./src/c8screen.c -c -o ./build/c8screen.o

clean:
	del build\*