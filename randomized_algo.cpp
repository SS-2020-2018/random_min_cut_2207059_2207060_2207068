#include <iostream>
#include <cstdio>
#include <random>
#include <time.h>
static std::mt19937 rng((unsigned)time(NULL));
struct Edge
{
    int src, dest;
};

struct Graph
{
    int V, E;
    Edge *edge;
};
struct subset
{
    int parent;
    int rank;
};

int find(struct subset subsets[], int i);
void Union(struct subset subsets[], int x, int y);

int kargerMinCut(struct Graph *graph)
{
    int V = graph->V, E = graph->E;
    Edge *edge = graph->edge;

    struct subset *subsets = new subset[V];
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    int vertices = V;
    while (vertices > 2)
    {

        std::uniform_int_distribution<int> dist(0, E - 1);
        int i = dist(rng);
        int subset1 = find(subsets, edge[i].src);
        int subset2 = find(subsets, edge[i].dest);
        if (subset1 == subset2)
            continue;
        else
        {
            printf("Contracting edge %d-%d\n",
                   edge[i].src, edge[i].dest);
            vertices--;
            Union(subsets, subset1, subset2);
        }
    }
    int cutedges = 0;
    for (int i = 0; i < E; i++)
    {
        int subset1 = find(subsets, edge[i].src);
        int subset2 = find(subsets, edge[i].dest);
        if (subset1 != subset2)
            cutedges++;
    }

    delete[] subsets;
    return cutedges;
}

int find(struct subset subsets[], int i)
{
    if (subsets[i].parent != i)
        subsets[i].parent =
            find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

void Union(struct subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

struct Graph *createGraph(int V, int E)
{
    Graph *graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    return graph;
}

int main()
{
    int V, E;
    if (!(std::cin >> V >> E))
        return 0;
    struct Graph *graph = createGraph(V, E);

    for (int i = 0; i < E; i++)
    {
        int u, v;
        std::cin >> u >> v;
        graph->edge[i].src = u;
        graph->edge[i].dest = v;
    }

    printf("\nCut found by Karger's randomized algo is %d\n",
           kargerMinCut(graph));

    delete[] graph->edge;
    delete graph;

    return 0;
}