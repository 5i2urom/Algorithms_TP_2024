#include "ArcGraph.h"
#include <cassert>

ArcGraph::ArcGraph(int size) : verticesCount(size) {}

ArcGraph::ArcGraph(const IGraph& graph) : verticesCount(graph.VerticesCount()) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (int to: graph.GetNextVertices(i)) {
            adjacencyArcs.push_back({i, to});
        }
    }
}

ArcGraph& ArcGraph::operator=(const ArcGraph &graph) {
    verticesCount = graph.verticesCount;
    adjacencyArcs = graph.adjacencyArcs;
    return *this;
}


void ArcGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < VerticesCount());
    assert(0 <= to && to < VerticesCount());
    adjacencyArcs.push_back({from, to});
}

int ArcGraph::VerticesCount() const {
    return verticesCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    std::vector<int> nextVertices;
    for (const auto& arc: adjacencyArcs) {
        if (arc.first == vertex)
            nextVertices.push_back(arc.second);
    }
    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    std::vector<int> prevVertices;
    for (const auto& arc: adjacencyArcs) {
        if (arc.second == vertex)
            prevVertices.push_back(arc.first);
    }
    return prevVertices;
}
