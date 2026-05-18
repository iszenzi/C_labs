#include "graph.h"
#include <limits.h>

Graph *create_graph(int num_vertices, int num_edges)
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    if (!g)
        return NULL;

    g->num_vertices = num_vertices;
    g->num_edges = num_edges;

    g->weights = (int *)calloc(num_edges, sizeof(int));
    g->matrix = (int **)malloc(num_vertices * sizeof(int *));

    for (int i = 0; i < num_vertices; i++)
    {
        g->matrix[i] = (int *)calloc(num_edges, sizeof(int));
    }

    return g;
}

void free_graph(Graph *g)
{
    if (!g)
        return;
    for (int i = 0; i < g->num_vertices; i++)
    {
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g->weights);
    free(g);
}

void add_edge(Graph *g, int edge_idx, int u, int v, int weight)
{
    if (!g || edge_idx < 0 || edge_idx >= g->num_edges)
        return;
    if (u < 0 || u >= g->num_vertices || v < 0 || v >= g->num_vertices)
        return;

    g->matrix[u][edge_idx] = 1;
    g->matrix[v][edge_idx] = 1;
    g->weights[edge_idx] = weight;
}

Graph *load_graph_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Could not open file %s for reading\n", filename);
        return NULL;
    }

    int v, e;
    if (fscanf(file, "%d %d", &v, &e) != 2)
    {
        fclose(file);
        return NULL;
    }

    Graph *g = create_graph(v, e);
    for (int i = 0; i < e; i++)
    {
        int u, w, weight;
        fscanf(file, "%d %d %d", &u, &w, &weight);
        add_edge(g, i, u, w, weight);
    }

    fclose(file);
    return g;
}

void save_graph_to_file(Graph *g, const char *filename)
{
    if (!g)
        return;
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "%d %d\n", g->num_vertices, g->num_edges);
    for (int i = 0; i < g->num_edges; i++)
    {
        int u = -1, v = -1;
        for (int j = 0; j < g->num_vertices; j++)
        {
            if (g->matrix[j][i] == 1)
            {
                if (u == -1)
                    u = j;
                else
                    v = j;
            }
        }
        if (u != -1 && v != -1)
        {
            fprintf(file, "%d %d %d\n", u, v, g->weights[i]);
        }
    }

    fclose(file);
}

Graph *prim_mst(Graph *g)
{
    if (!g || g->num_vertices <= 0)
        return NULL;

    Graph *mst = create_graph(g->num_vertices, g->num_vertices - 1);
    if (!mst)
        return NULL;

    int *visited = (int *)calloc(g->num_vertices, sizeof(int));
    visited[0] = 1;

    int mst_edge_count = 0;

    while (mst_edge_count < g->num_vertices - 1)
    {
        int min_weight = INT_MAX;
        int best_edge = -1;
        int best_u = -1;
        int best_v = -1;

        for (int e = 0; e < g->num_edges; e++)
        {
            int u = -1, v = -1;
            for (int i = 0; i < g->num_vertices; i++)
            {
                if (g->matrix[i][e] == 1)
                {
                    if (u == -1)
                        u = i;
                    else
                        v = i;
                }
            }

            if (u != -1 && v != -1)
            {
                if ((visited[u] && !visited[v]) || (!visited[u] && visited[v]))
                {
                    if (g->weights[e] < min_weight)
                    {
                        min_weight = g->weights[e];
                        best_edge = e;
                        best_u = u;
                        best_v = v;
                    }
                }
            }
        }

        if (best_edge != -1)
        {
            add_edge(mst, mst_edge_count, best_u, best_v, min_weight);
            visited[best_u] = 1;
            visited[best_v] = 1;
            mst_edge_count++;
        }
        else
        {
            break;
        }
    }

    free(visited);
    return mst;
}
