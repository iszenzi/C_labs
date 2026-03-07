all: build run_main

main.exe: main.o tree.o
	gcc main.o tree.o -o main.exe

tree.o: tree.c tree.h
	gcc -c tree.c -o tree.o

main.o: main.c tree.h
	gcc -c main.c -o main.o

run_main: main.exe
	./main.exe

build: main.exe
	@echo "The assembly is completed"

clean:
	del /Q *.o main.exe