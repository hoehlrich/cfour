objects := $(patsubst src%, target%, $(patsubst %.c,%.o,$(wildcard src/*.c)))
headers := $(wildcard src/*.h)

cfour : $(objects) $(headers)
	gcc $(objects) $(headers) -o target/cfour -lSDL2_image $$(sdl2-config --libs) -lm

target/main.o : src/main.c src/cfour.h
	gcc -c src/main.c -o target/main.o

target/cfour.o : src/cfour.c src/cfour.h
	gcc -c src/cfour.c -o target/cfour.o

target/gui.o : src/gui.c src/cfour.h
	gcc -c src/gui.c $$(sdl2-config --cflags) -o target/gui.o

clean :
	rm target/*.o
