#include "MatrixGraph.h"
#include <cassert>

MatrixGraph::MatrixGraph(int size): adjacencyMatrix(size, std::vector<bool>(size, false)) {}

MatrixGraph::MatrixGraph(const IGraph &graph): adjacencyMatrix(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false)) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        for (int to : graph.GetNextVertices(i)) {
            adjacencyMatrix[i][to] = true;
        }
    }
}

MatrixGraph& MatrixGraph::operator=(const MatrixGraph &graph) {
    adjacencyMatrix = graph.adjacencyMatrix;
    return *this;
}

void MatrixGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < VerticesCount());
    assert(0 <= to && to < VerticesCount());
    adjacencyMatrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return adjacencyMatrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    std::vector<int> nextVertices;
    for (int i = 0; i < VerticesCount(); ++i) {
        if (adjacencyMatrix[vertex][i])
            nextVertices.push_back(i);
    }
    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    std::vector<int> prevVertices;
    for (int i = 0; i < VerticesCount(); ++i) {
        if (adjacencyMatrix[i][vertex])
            prevVertices.push_back(i);
    }
    return prevVertices;
}
