#ifndef GRAPH_H
#define GRAPH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

    typedef struct
    {
        int num_vertices;
        int num_edges;
        int **matrix;
        int *weights;
    } Graph;

    Graph *create_graph(int num_vertices, int num_edges);

    void free_graph(Graph *g);
    void add_edge(Graph *g, int edge_idx, int u, int v, int weight);
    Graph *load_graph_from_file(const char *filename);
    void save_graph_to_file(Graph *g, const char *filename);
    Graph *prim_mst(Graph *g);

#ifdef __cplusplus
}
#endif

#endif
