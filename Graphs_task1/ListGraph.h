#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "IGraph.h"
#include <vector>

struct ListGraph: public IGraph
{
public:
    ListGraph(int size);
    ListGraph(const IGraph &graph);
    ListGraph& operator=(const ListGraph &graph);

    ~ListGraph() {}

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacencyLists;
};

#endif // LISTGRAPH_H
