#include <gtest/gtest.h>

extern "C"
{
#include "graph.h"
}

TEST(GraphTest, CreateAndFreeGraph)
{
    Graph *g = create_graph(4, 3);
    ASSERT_NE(g, nullptr);
    EXPECT_EQ(g->num_vertices, 4);
    EXPECT_EQ(g->num_edges, 3);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(g->matrix[i][j], 0);
        }
    }

    free_graph(g);
}

TEST(GraphTest, AddEdge)
{
    Graph *g = create_graph(3, 1);
    add_edge(g, 0, 0, 1, 10);

    EXPECT_EQ(g->matrix[0][0], 1);
    EXPECT_EQ(g->matrix[1][0], 1);
    EXPECT_EQ(g->matrix[2][0], 0);
    EXPECT_EQ(g->weights[0], 10);

    free_graph(g);
}

TEST(PrimTest, SimpleMST)
{
    Graph *g = create_graph(3, 3);
    add_edge(g, 0, 0, 1, 10);
    add_edge(g, 1, 1, 2, 20);
    add_edge(g, 2, 0, 2, 30);

    Graph *mst = prim_mst(g);
    ASSERT_NE(mst, nullptr);
    EXPECT_EQ(mst->num_vertices, 3);
    EXPECT_EQ(mst->num_edges, 2);

    int total_weight = 0;
    for (int i = 0; i < mst->num_edges; i++)
    {
        total_weight += mst->weights[i];
    }
    EXPECT_EQ(total_weight, 30);

    free_graph(mst);
    free_graph(g);
}

TEST(PrimTest, DisconnectedGraph)
{
    Graph *g = create_graph(4, 1);
    add_edge(g, 0, 0, 1, 5);

    Graph *mst = prim_mst(g);

    // Поскольку граф несвязный, алгоритм Прима не может найти остовное дерево, содержащее V-1 ребер,
    // и должен обработать этот случай (в нашей реализации мы прерываем цикл).
    ASSERT_NE(mst, nullptr);

    free_graph(mst);
    free_graph(g);
}

TEST(GraphTest, NullGraphHandling)
{
    Graph *mst = prim_mst(nullptr);
    EXPECT_EQ(mst, nullptr);

    // Проверка, что функции не падают при передаче NULL
    add_edge(nullptr, 0, 0, 1, 10);
    free_graph(nullptr);
}
