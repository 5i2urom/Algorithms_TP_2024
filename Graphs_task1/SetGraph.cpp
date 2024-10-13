#include "SetGraph.h"
#include <cassert>

SetGraph::SetGraph(int size) : adjacencySets(size) {}

SetGraph::SetGraph(const IGraph& graph) : adjacencySets(graph.VerticesCount()) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (int to: graph.GetNextVertices(i)) {
            adjacencySets[i].insert(to);
        }
    }
}

SetGraph& SetGraph::operator=(const SetGraph &graph) {
    adjacencySets = graph.adjacencySets;
    return *this;
}
void SetGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < VerticesCount());
    assert(0 <= to && to < VerticesCount());
    adjacencySets[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return adjacencySets.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    return std::vector<int>(adjacencySets[vertex].begin(), adjacencySets[vertex].end());
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    std::vector<int> prevVertices;
    for (int from = 0; from < VerticesCount(); ++from) {
        for (int to : adjacencySets[from]) {
            if (to == vertex) {
                prevVertices.push_back(from);
            }
        }
    }
    return prevVertices;
}
