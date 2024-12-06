#include <iostream>
#include <vector>
#include <queue>
#include <ctime>

using namespace std;

class Graph {
private:
    int n;
    vector<vector<int>> adjMatrix;

    bool isBipartiteComponent(int start, vector<int>& colors) {
        queue<int> q;
        q.push(start);
        colors[start] = 0;

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int u = 0; u < n; ++u) {
                if (adjMatrix[v][u]) {
                    if (colors[u] == -1) {
                        colors[u] = 1 - colors[v];
                        q.push(u);
                    } else if (colors[u] == colors[v]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

public:
    Graph(int vertices) : n(vertices), adjMatrix(vertices, vector<int>(vertices, 0)) {}

    void generateRandomGraph(double density) {
        srand(time(nullptr));
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if ((rand() % 100) < (density * 100)) {
                    adjMatrix[i][j] = adjMatrix[j][i] = 1;
                }
            }
        }
    }

    void findBipartiteComponents() {
        vector<int> colors(n, -1);
        vector<bool> visited(n, false);

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                if (isBipartiteComponent(i, colors)) {
                    cout << "Bipartite component found: ";
                    for (int j = 0; j < n; ++j) {
                        if (colors[j] != -1) {
                            cout << j << " ";
                            visited[j] = true;
                        }
                    }
                    cout << endl;
                } else {
                    cout << "Component starting at vertex " << i << " is not bipartite." << endl;
                    for (int j = 0; j < n; ++j) {
                        if (colors[j] != -1) {
                            visited[j] = true;
                        }
                    }
                }
                fill(colors.begin(), colors.end(), -1);
            }
        }
    }

    void printAdjMatrix() const {
        cout << "Adjacency matrix of the graph:\n";
        for (const auto& row : adjMatrix) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int vertices = 10;
    double density = 0.3;

    Graph g(vertices);
    g.generateRandomGraph(density);

    cout << "Generated graph:\n";
    g.printAdjMatrix();

    cout << "\nFinding bipartite components:\n";
    g.findBipartiteComponents();

    return 0;
}
