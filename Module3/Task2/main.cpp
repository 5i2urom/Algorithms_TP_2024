// Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
// Найдите количество различных кратчайших путей между заданными вершинами.

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;
    virtual int VerticesCount() const  = 0;
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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

ListGraph::ListGraph(int size) : adjacencyLists(size) {}

ListGraph::ListGraph(const IGraph& graph) : adjacencyLists(graph.VerticesCount()) {
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        adjacencyLists[i] = graph.GetNextVertices(i);
    }
}

ListGraph& ListGraph::operator=(const ListGraph &graph) {
    adjacencyLists = graph.adjacencyLists;
    return *this;
}

void ListGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < VerticesCount());
    assert(0 <= to && to < VerticesCount());
    adjacencyLists[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return adjacencyLists.size();
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    std::vector<int> prevVertices;
    for (int from = 0; from < VerticesCount(); ++from) {
        for (int to : adjacencyLists[from]) {
            if (to == vertex) {
                prevVertices.push_back(from);
            }
        }
    }
    return prevVertices;
}

int GetNumberOfShortestPaths(const IGraph& graph, int startVertex, int endVertex) {
    std::vector<int> dist(graph.VerticesCount(), __INT_MAX__);
    std::vector<int> paths(graph.VerticesCount(), 0);
    dist[startVertex] = 0;
    paths[startVertex] = 1;

    std::vector<bool> visited (graph.VerticesCount(), false);
    std::queue<int> q;
    q.push(startVertex);
    visited[startVertex] = true;

    while (!q.empty()) {
        int currentVertex = q.front();
        q.pop();

        for (int to: graph.GetNextVertices(currentVertex)) {
            if (dist[to] > dist[currentVertex] + 1) {
                dist[to] = dist[currentVertex] + 1;
                paths[to] = paths[currentVertex];
            }
            else if (dist[to] == dist[currentVertex] + 1) {
                paths[to] += paths[currentVertex];                
            }

            if (!visited[to])
            {
                visited[to] = true;
                q.push(to);
            }
        }
    }
    return paths[endVertex];    
}

int main() {
    int verticesNumber, edgeNumber, startVertex, endVertex = 0;    
    std::pair<int, int> verticesPair {0, 0};
    std::cin >> verticesNumber >> edgeNumber; 

    ListGraph graph(verticesNumber);
    
    for (int i = 0; i < edgeNumber; ++i)
    {
        std::cin >> verticesPair.first >> verticesPair.second;
        graph.AddEdge(verticesPair.first, verticesPair.second);
        graph.AddEdge(verticesPair.second, verticesPair.first);
    }

    std::cin >> startVertex >> endVertex; 
    std::cout << GetNumberOfShortestPaths(graph, startVertex, endVertex);      
}