cfour : target/main.o target/cfour.o
	gcc  target/main.o target/cfour.o -o target/cfour -lm

target/main.o : src/main.c src/cfour.h
	gcc -c src/main.c -o target/main.o

target/cfour.o : src/cfour.c src/cfour.h
	gcc -c src/cfour.c -o target/cfour.o
