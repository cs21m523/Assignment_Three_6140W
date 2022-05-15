#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })

bool *visited;
int *parent;
int *discovered;
int *lowest;
bool *cut_vertices;
int visit_number = 0;
unsigned int **predef_graph(unsigned int **graph)
{
    unsigned int pregraph[6][6] = {
        {0, 1, 1, 0, 0, 0},
        {1, 0, 1, 0, 0, 0},
        {1, 1, 0, 1, 1, 1},
        {0, 0, 1, 0, 0, 1},
        {0, 0, 1, 0, 0, 1},
        {0, 0, 1, 1, 1, 0}};
    int i = 0, j = 0;
    graph = malloc(6 * sizeof(unsigned int *));
    for (i = 0; i < 6; i++)
    {
        graph[i] = malloc(6 * sizeof(unsigned int));
        for (j = 0; j < 6; j++)
        {
            graph[i][j] = pregraph[i][j];
        }
    }
    return graph;
}

unsigned int **input_graph(unsigned int **graph, int vertices)
{
    int i = 0, j = 0;
    unsigned int weight;
    printf("Enter weight data for %d nodes[0 if nodes are not connected]\n", vertices);
    graph = malloc(vertices * sizeof(unsigned int *));
    for (i = 0; i < vertices; i++)
    {
        graph[i] = malloc(vertices * sizeof(unsigned int));
        graph[i][i] = 0;
    }
    for (i = 0; i < vertices; i++)
    {
        for (j = i + 1; j < vertices; j++)
        {
            printf("Enter 1 if node %c is connected to %c> ", 'A' + i, 'A' + j);
            scanf("%d", &weight);
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
    }
    return graph;
}

void print_graph(unsigned int **graph, int vertices)
{
    int i = 0, j = 0;
    printf("  ");
    for (j = 0; j < vertices; j++)
    {
        printf("%*c ", 3, 'A' + j);
    }
    printf("\n");
    for (i = 0; i < vertices; i++)
    {
        printf("%c ", 'A' + i);
        for (j = 0; j < vertices; j++)
        {
            printf("%*d ", 3, graph[i][j]);
        }
        printf("\n");
    }
}

void _dfs(unsigned int **graph, int vertices, int vertex, int level)
{
    int i = 0;
    int children = 0;
    visit_number++;
    visited[vertex] = true;
    lowest[vertex] = visit_number;
    discovered[vertex] = visit_number;
    for (i = 0; i < level; i++)
        printf("  ");
    printf("Vertex %c\n", 'A' + vertex);
    for (i = 0; i < vertices; i++)
    {
        if (!graph[vertex][i] || i == vertex)
            continue;
        if (!visited[i])
        {
            children++;
            parent[i] = vertex;
            _dfs(graph, vertices, i, level + 1);
            lowest[vertex] = MIN(lowest[vertex], lowest[i]);
            if ((parent[vertex] != -1) && lowest[i] >= discovered[vertex])
            {
                cut_vertices[vertex] = true;
            }
            else if ((parent[vertex] == -1) && children > 1)
            {
                cut_vertices[vertex] = true;
            }
        }
        else if (i != parent[vertex])
        {
            lowest[vertex] = MIN(lowest[vertex], discovered[i]);
        }
    }
}

void dfs(unsigned int **graph, int vertices)
{
    int i = 0;
    visited = calloc(vertices, sizeof(bool));
    cut_vertices = calloc(vertices, sizeof(bool));
    parent = calloc(vertices, sizeof(int));
    discovered = calloc(vertices, sizeof(int));
    lowest = calloc(vertices, sizeof(int));
    for (i = 0; i < vertices; i++)
        parent[i] = -1;
    printf("The graph traversal tree in depth-first order is: \n");
    printf("===================================================\n");
    for (i = 0; i < vertices; i++)
    {
        if (!visited[i])
            _dfs(graph, vertices, i, 0);
    }
    printf("===================================================\n");
    printf("The cut vertices for this graph are: \n");
    for (i = 0; i < vertices; i++)
    {
        if (cut_vertices[i])
            printf("%c  ", 'A' + i);
    }
    printf("\n");
}

void main(int argc, char *argv[])
{
    unsigned int **graph;
    char choice;
    int i = 0, j = 0, weight;
    int vertices;
    printf("Use pre-defined graph? [Y/y]> ");
    scanf("%c", &choice);
    printf("%c", choice);
    if (choice != 'y' && choice != 'Y' && choice != '\n')
    {
        printf("Enter number of vertices> ");
        scanf("%d", &vertices);
        graph = input_graph(graph, vertices);
    }
    else
    {
        vertices = 6;
        graph = predef_graph(graph);
    }
    print_graph(graph, vertices);
    dfs(graph, vertices);
}