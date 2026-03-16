all: build run_main

main.exe: main.o table.o
	gcc main.o table.o -o main.exe

table.o: table.c table.h
	gcc -c table.c -o table.o

main.o: main.c table.h
	gcc -c main.c -o main.o

run_main: main.exe
	./main.exe

build: main.exe
	@echo "The assembly is completed"

clean:
	del /Q *.o main.exe
