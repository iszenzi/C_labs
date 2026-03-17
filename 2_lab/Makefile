all: build run_main

main.exe: main.o table.o
	gcc main.o table.o -o main.exe

table.o: table.c table.h
	gcc -c table.c -o table.o

main.o: main.c table.h
	gcc -c main.c -o main.o

run_main: main.exe
	./main.exe
run_test: test_table.exe
	./test_table.exe
test_table.exe: test_table.o table.o
	gcc test_table.o table.o -o test_table.exe

test_table.o: test_table.c table.h
	gcc -c test_table.c -o test_table.o

build: main.exe
	@echo "The assembly is completed"

clean:
	del /Q *.o main.exe test_table.exe
