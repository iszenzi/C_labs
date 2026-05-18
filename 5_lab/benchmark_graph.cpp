#include <benchmark/benchmark.h>
#include <random>
#include <cstdio>
#include <string>

extern "C"
{
#include "graph.h"
}

static Graph *create_random_graph(int vertices, int edges)
{
    Graph *graph = create_graph(vertices, edges);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> vertex_dist(0, vertices - 1);
    std::uniform_int_distribution<> weight_dist(1, 100);

    for (int i = 0; i < edges; i++)
    {
        int u = vertex_dist(gen);
        int v = vertex_dist(gen);
        while (u == v)
        {
            v = vertex_dist(gen);
        }
        int weight = weight_dist(gen);
        add_edge(graph, i, u, v, weight);
    }
    return graph;
}

static void BM_Prim_SmallGraph(benchmark::State &state)
{
    Graph *graph = create_random_graph(50, 100);

    for (auto _ : state)
    {
        Graph *mst = prim_mst(graph);
        benchmark::DoNotOptimize(mst);
        benchmark::ClobberMemory();
        free_graph(mst);
    }

    free_graph(graph);
}
BENCHMARK(BM_Prim_SmallGraph);

static void BM_Prim_MediumGraph(benchmark::State &state)
{
    Graph *graph = create_random_graph(150, 500);

    for (auto _ : state)
    {
        Graph *mst = prim_mst(graph);
        benchmark::DoNotOptimize(mst);
        benchmark::ClobberMemory();
        free_graph(mst);
    }

    free_graph(graph);
}
BENCHMARK(BM_Prim_MediumGraph);

static void BM_Prim_DenseGraph(benchmark::State &state)
{
    int vertices = state.range(0);
    int edges = vertices * (vertices - 1) / 4;
    Graph *graph = create_random_graph(vertices, edges);

    for (auto _ : state)
    {
        Graph *mst = prim_mst(graph);
        benchmark::DoNotOptimize(mst);
        benchmark::ClobberMemory();
        free_graph(mst);
    }

    free_graph(graph);
}
BENCHMARK(BM_Prim_DenseGraph)->Range(50, 200);

static void BM_GraphLoadFromFile(benchmark::State &state)
{
    Graph *temp_graph = create_random_graph(100, 300);
    save_graph_to_file(temp_graph, "benchmark_temp.txt");
    free_graph(temp_graph);

    for (auto _ : state)
    {
        Graph *graph = load_graph_from_file("benchmark_temp.txt");
        benchmark::DoNotOptimize(graph);
        benchmark::ClobberMemory();
        free_graph(graph);
    }

    remove("benchmark_temp.txt");
}
BENCHMARK(BM_GraphLoadFromFile);

BENCHMARK_MAIN();
