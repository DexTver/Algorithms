#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int n, m, u, v;
    cin >> n >> m;
    vector<vector<int>> g(n);
    for (int i = 0; i < m; ++i) {
        cin >> u >> v;
        u--;
        v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> color(n, 0);
    int cnt = 0;
    vector<vector<pair<int, int>>> spanning_edges;
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (!color[i]) {
            spanning_edges.emplace_back();
            color[i] = 1;
            q.push(i);
            while (!q.empty()) {
                u = q.front();
                q.pop();
                for (auto w: g[u]) {
                    if (!color[w]) {
                        color[w] = color[u] == 1 ? 2 : 1;
                        spanning_edges[cnt].emplace_back(u, w);
                        q.push(w);
                    } else if (color[w] != color[u]) {
                        spanning_edges[cnt].emplace_back(u, w);
                    }
                }
            }
            ++cnt;
        }
    }

    cout << "Count of connected components: " << cnt << "\n";
    for (int i = 0; i < cnt; ++i) {
        cout << "\nComponent " << i + 1 << ":\n";
        for (auto x: spanning_edges[i]) {
            cout << x.first + 1 << "-" << x.second + 1 << "\n";
        }
    }

    return 0;
}
