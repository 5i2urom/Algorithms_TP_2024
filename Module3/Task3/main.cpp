// Требуется отыскать самый короткий маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город. 

#include <iostream>
#include <vector>
#include <cassert>
#include <set>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;
    virtual int VerticesCount() const  = 0;
    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const = 0;
};

struct ListGraph: public IGraph
{
public:
    ListGraph(int size);
    ListGraph(const IGraph &graph);
    ListGraph& operator=(const ListGraph &graph);

    ~ListGraph() {}

    void AddEdge(int from, int to, int weight) override;
    int VerticesCount() const override;
    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override;
    virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists; // vertex, weight
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

void ListGraph::AddEdge(int from, int to, int weight) {
    assert(0 <= from && from < VerticesCount());
    assert(0 <= to && to < VerticesCount());
    adjacencyLists[from].push_back({to, weight});
}

int ListGraph::VerticesCount() const {
    return adjacencyLists.size();
}

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    return adjacencyLists[vertex];
}

std::vector<std::pair<int, int>> ListGraph::GetPrevVertices(int vertex) const {
    assert(0 <= vertex && vertex < VerticesCount());
    std::vector<std::pair<int, int>> prevVertices;
    for (int from = 0; from < VerticesCount(); ++from) {
        for (const auto& to : adjacencyLists[from]) {
            if (to.first == vertex) {
                prevVertices.push_back({from, to.second});
            }
        }
    }
    return prevVertices;
}

int DijkstraShortestPath(const IGraph& graph, int startVertex, int endVertex) {
    std::vector<int> dist(graph.VerticesCount(), __INT_MAX__);
    dist[startVertex] = 0;

    std::set<std::pair<int, int>> pq;
    pq.insert({0, startVertex});

    while (!pq.empty()) {
        int currentDist = pq.begin()->first;
        int currentVertex = pq.begin()->second;
        pq.erase(pq.begin());

        for (const auto& next : graph.GetNextVertices(currentVertex)) {
            int nextVertex = next.first;
            int weight = next.second;

            if (dist[currentVertex] + weight < dist[nextVertex]) {
                pq.erase({dist[nextVertex], nextVertex});
                dist[nextVertex] = dist[currentVertex] + weight;
                pq.insert({dist[nextVertex], nextVertex});
            }
        }
    }
    return dist[endVertex];
}

int main() {
    int n, m, from, to, w, startVertex, endVertex = 0;
    std::cin >> n >> m;
    ListGraph graph(n);

    for (int i = 0; i < m; ++i) {
        std::cin >> from >> to >> w;
        graph.AddEdge(from, to, w);
        graph.AddEdge(to, from, w);
    }
    std::cin >> startVertex >> endVertex;
    std::cout << DijkstraShortestPath(graph, startVertex, endVertex);
}