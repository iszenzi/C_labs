all: build run_main

main.exe: main.o tree.o
	gcc main.o tree.o -o main.exe

tree.o: tree.c tree.h
	gcc -c tree.c -o tree.o

main.o: main.c tree.h
	gcc -c main.c -o main.o

test_tree.exe: test_tree.o tree.o
	gcc test_tree.o tree.o -o test_tree.exe

test_tree.o: test_tree.c tree.h
	gcc -c test_tree.c -o test_tree.o

run_main: main.exe
	./main.exe

run_test: test_tree.exe
	./test_tree.exe

build: main.exe
	@echo "The assembly is completed"

clean:
	del /Q *.o main.exe test_tree.exe