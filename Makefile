OBJECTS= ./build/compiler.o ./build/cprocess.o ./build/helpers/buffer.o ./build/helpers/vector.o
INCLUDES= -I./

all: ${OBJECTS}
	gcc main.c ${INCLUDES} ${OBJECTS} -g -o ./main

./build/compiler.o: ./compiler.c
	mkdir -p ./build
	gcc ./compiler.c ${INCLUDES} -o ./build/compiler.o -g -c

./build/cprocess.o: ./cprocess.c
	mkdir -p ./build
	gcc ./cprocess.c ${INCLUDES} -o ./build/cprocess.o -g -c

./build/helpers/buffer.o: ./helpers/buffer.c
	mkdir -p ./build/helpers
	gcc ./helpers/buffer.c ${INCLUDES} -o ./build/helpers/buffer.o -g -c

./build/helpers/vector.o: ./helpers/vector.c
	mkdir -p ./build/helpers
	gcc ./helpers/vector.c ${INCLUDES} -o ./build/helpers/vector.o -g -c

clean:
	rm ./main
	rm -rf ./build
