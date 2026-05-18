#include <benchmark/benchmark.h>
#include <cstdlib>
#include <string>

extern "C"
{
#include "graph.h"
}

Graph *generate_random_graph(int num_vertices)
{
    int num_edges = (num_vertices * (num_vertices - 1)) / 2;
    Graph *g = create_graph(num_vertices, num_edges);
    int edge_idx = 0;
    for (int i = 0; i < num_vertices; i++)
    {
        for (int j = i + 1; j < num_vertices; j++)
        {
            add_edge(g, edge_idx++, i, j, rand() % 100 + 1);
        }
    }
    return g;
}

// Бенчмаркинг алгоритма Прима (MST) - Маленький граф (10 вершин)
static void BM_PrimMST_Small(benchmark::State &state)
{
    Graph *g = generate_random_graph(10);
    for (auto _ : state)
    {
        Graph *mst = prim_mst(g);
        benchmark::DoNotOptimize(mst);
        free_graph(mst);
    }
    free_graph(g);
}
BENCHMARK(BM_PrimMST_Small)->Unit(benchmark::kMillisecond);

// Бенчмаркинг алгоритма Прима (MST) - Средний граф (50 вершин)
static void BM_PrimMST_Medium(benchmark::State &state)
{
    Graph *g = generate_random_graph(50);
    for (auto _ : state)
    {
        Graph *mst = prim_mst(g);
        benchmark::DoNotOptimize(mst);
        free_graph(mst);
    }
    free_graph(g);
}
BENCHMARK(BM_PrimMST_Medium)->Unit(benchmark::kMillisecond);

// Бенчмаркинг алгоритма Прима (MST) - Большой граф (150 вершин)
static void BM_PrimMST_Large(benchmark::State &state)
{
    Graph *g = generate_random_graph(150);
    for (auto _ : state)
    {
        Graph *mst = prim_mst(g);
        benchmark::DoNotOptimize(mst);
        free_graph(mst);
    }
    free_graph(g);
}
BENCHMARK(BM_PrimMST_Large)->Unit(benchmark::kMillisecond);

static void ensure_file_exists(int num_vertices, const std::string &filename)
{
    Graph *g = generate_random_graph(num_vertices);
    save_graph_to_file(g, filename.c_str());
    free_graph(g);
}

// Загрузка - Маленький
static void BM_LoadGraphFromFile_Small(benchmark::State &state)
{
    std::string filename = "bench_graph_10.txt";
    if (state.thread_index() == 0)
    {
        ensure_file_exists(10, filename);
    }

    for (auto _ : state)
    {
        Graph *g = load_graph_from_file(filename.c_str());
        benchmark::DoNotOptimize(g);
        free_graph(g);
    }
}
BENCHMARK(BM_LoadGraphFromFile_Small)->Unit(benchmark::kMillisecond);

// Загрузка - Средний
static void BM_LoadGraphFromFile_Medium(benchmark::State &state)
{
    std::string filename = "bench_graph_50.txt";
    if (state.thread_index() == 0)
    {
        ensure_file_exists(50, filename);
    }

    for (auto _ : state)
    {
        Graph *g = load_graph_from_file(filename.c_str());
        benchmark::DoNotOptimize(g);
        free_graph(g);
    }
}
BENCHMARK(BM_LoadGraphFromFile_Medium)->Unit(benchmark::kMillisecond);

// Загрузка - Большой
static void BM_LoadGraphFromFile_Large(benchmark::State &state)
{
    std::string filename = "bench_graph_150.txt";
    if (state.thread_index() == 0)
    {
        ensure_file_exists(150, filename);
    }

    for (auto _ : state)
    {
        Graph *g = load_graph_from_file(filename.c_str());
        benchmark::DoNotOptimize(g);
        free_graph(g);
    }
}
BENCHMARK(BM_LoadGraphFromFile_Large)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

BENCHMARK_MAIN();
