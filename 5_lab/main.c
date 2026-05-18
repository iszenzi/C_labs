#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void print_menu()
{
    printf("Menu:\n");
    printf("1. Load graph from file\n");
    printf("2. Calculate MST (Prim)\n");
    printf("3. Save MST to file\n");
    printf("4. Save Original Graph to file\n");
    printf("5. Exit\n");
    printf("Choice: ");
}

int main(int argc, char *argv[])
{
    int choice;
    char filename[256];
    Graph *graph = NULL;
    Graph *mst = NULL;

    if (argc >= 3)
    {
        graph = load_graph_from_file(argv[1]);
        if (graph)
        {
            mst = prim_mst(graph);
            if (mst)
            {
                save_graph_to_file(mst, argv[2]);
                printf("MST successfully saved to %s\n", argv[2]);
                free_graph(mst);
            }
            else
            {
                printf("Failed to build MST\n");
            }
            free_graph(graph);
            return 0;
        }
        else
        {
            return 1;
        }
    }

    while (1)
    {
        print_menu();
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            printf("Enter filename to load graph: ");
            scanf("%255s", filename);
            if (graph)
                free_graph(graph);
            graph = load_graph_from_file(filename);
            if (graph)
            {
                printf("Graph loaded! Vertices: %d, Edges: %d\n", graph->num_vertices, graph->num_edges);
            }
            else
            {
                printf("Failed to load graph from %s.\n", filename);
            }
            break;
        case 2:
            if (!graph)
            {
                printf("Please load a graph first\n");
                break;
            }
            if (mst)
                free_graph(mst);
            mst = prim_mst(graph);
            if (mst && mst->num_edges == graph->num_vertices - 1)
                printf("MST calculated successfully\n");
            else
                printf("Failed to calculate full MST (possibly disconnected graph)\n");
            break;
        case 3:
            if (!mst)
            {
                printf("No MST available. Calculate it first\n");
                break;
            }
            printf("Enter filename to save MST: ");
            scanf("%255s", filename);
            save_graph_to_file(mst, filename);
            printf("MST saved to %s\n", filename);
            break;
        case 4:
            if (!graph)
            {
                printf("No original graph available\n");
                break;
            }
            printf("Enter filename to save original graph: ");
            scanf("%255s", filename);
            save_graph_to_file(graph, filename);
            printf("Graph saved to %s\n", filename);
            break;
        case 5:
            if (graph)
                free_graph(graph);
            if (mst)
                free_graph(mst);
            printf("Exiting\n");
            return 0;
        default:
            printf("Invalid choice. Try again\n");
        }
    }

    return 0;
}
