all: build run_test run_bench

build: main.exe test_graph.exe benchmark_graph.exe

main.exe: main.o graph.o
	gcc main.o graph.o -o main.exe

graph.o: graph.c graph.h
	gcc -c graph.c -o graph.o

main.o: main.c graph.h
	gcc -c main.c -o main.o

test_graph.exe: test_graph.o graph.o
	g++ test_graph.o graph.o -o test_graph.exe -lgtest -lgtest_main

test_graph.o: test_graph.cpp graph.h
	g++ -c test_graph.cpp -o test_graph.o

benchmark_graph.exe: benchmark_graph.o graph.o
	g++ benchmark_graph.o graph.o -o benchmark_graph.exe -lbenchmark -lbenchmark_main

benchmark_graph.o: benchmark_graph.cpp graph.h
	g++ -c benchmark_graph.cpp -o benchmark_graph.o

run_main: main.exe
	./main.exe

run_test: test_graph.exe
	./test_graph.exe

run_bench: benchmark_graph.exe
	./benchmark_graph.exe

clean:
	del /Q *.o main.exe test_graph.exe benchmark_graph.exe
