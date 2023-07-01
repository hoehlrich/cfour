connect-four : src/main.c
	gcc src/main.c -o target/connect-four -lm

target/main.o : src/main.c
	gcc -c src/main.c -o target/main.o
