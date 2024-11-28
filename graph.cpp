#include <iostream>
#include <vector>
#include <queue>
#include <set>

class Graph {
public:
    Graph(int size); 
    void addEdge(int v1, int v2); 
    void findPairsWithinThreeHandshakes(); 

private:
    int size; 
    std::vector<std::vector<int>> adj;
};

Graph::Graph(int size) : size(size) {
    adj.resize(size); 
}

void Graph::addEdge(int v1, int v2) {
    adj[v1].push_back(v2);
    adj[v2].push_back(v1); 
}

// найти все пары, которые могут быть связаны через 3 рукопожатия
void GraphGraph::findPairsWithinThreeHandshakes() {
    for (int i = 0; i < size; ++i) {
        std::set<int> visited;
        std::queue<std::pair<int, int>> q;
        q.push({i, 0});
        visited.insert(i);
        while (!q.empty()) {
            auto [current, level] = q.front();
            q.pop();

            if (level == 3) continue;
            
            for (int neighbor : adj[current]) {
                if (visited.find(neighbor) == visited.end()) { 
                    visited.insert(neighbor);
                    q.push({neighbor, level + 1}); 
                }
            }
        }
        for (int j = 0; j < size; ++j) {
            if (visited.find(j) != visited.end() && j != i) {
                std::cout << "Пара: " << i << " и " << j << " через три рукопожатия." << std::endl;
            }
        }
    }
}

int main() {
    Graph g(6); п
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(3, 5);
    g.addEdge(4, 5);
    g.findPairsWithinThreeHandshakes();
    
    return 0;
}