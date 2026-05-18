#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Структура для представления графа в виде матрицы инцидентности
typedef struct {
    int num_vertices; // Количество вершин (V)
    int num_edges;    // Количество ребер (E)
    int **matrix;     // Матрица инцидентности V x E (1 - инцидентна, 0 - нет)
    int *weights;     // Массив весов для каждого ребра размера E
} Graph;

// Создание графа (выделение памяти)
Graph* create_graph(int num_vertices, int num_edges);

// Освобождение памяти, занятой графом
void free_graph(Graph* g);

// Добавление ребра в граф
// edge_idx - индекс ребра (от 0 до E-1)
// u, v - индексы соединяемых вершин
// weight - вес ребра
void add_edge(Graph* g, int edge_idx, int u, int v, int weight);

// Чтение графа из файла
// Формат файла: 
// [количество_вершин] [количество_ребер]
// [u1] [v1] [weight1]
// [u2] [v2] [weight2]
// ...
Graph* load_graph_from_file(const char* filename);

// Сохранение графа в файл на диск
void save_graph_to_file(Graph* g, const char* filename);

// Алгоритм Прима (минимальное остовное дерево)
// Возвращает новый граф, представляющий МОД
Graph* prim_mst(Graph* g);

#endif // GRAPH_H
