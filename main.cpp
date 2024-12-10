#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int n;
    int m;
    cin >> n >> m;

    vector<vector<int>> g(n);
    vector<pair<int, int>> edges(m);
    vector<int> color(n, 0);
    queue<int> q;

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;
        g[u].push_back(v);
        g[v].push_back(u);
        edges[i] = {u, v};
    }

    for (int i = 0; i < n; ++i) {
        if (!color[i]) {
            color[i] = 1;
            q.push(i);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int w: g[u]) {
                    if (!color[w]) {
                        color[w] = color[u] == 1 ? 2 : 1;
                        q.push(w);
                    }
                }
            }
        }
    }

    for (auto x: edges) {
        if (color[x.first] != color[x.second]) {
            cout << x.first + 1 << "-" << x.second + 1 << "\n";
        }
    }

    return 0;
}
