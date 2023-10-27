// O(V^2*E)
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct Dinic {
    int n;
    vector<vector<pair<int, int>>> G;
    vector<ll> c;
    Dinic(int _n) : n(_n), G(n + 1), c(2) {};
    void AddEdge(int from, int to, int cap) {
        if (from == to)
            return;
        G[from].emplace_back(to, c.size());
        G[to].emplace_back(from, c.size() + 1);
        c.emplace_back(cap), c.emplace_back(0);
    }
    ll MaxFlow(int s, int t) {
        ll flow = 0;
        while (1) {
            vector<int> h(n + 1), pos(n + 1);
            queue<int> Q;
            Q.emplace(s);
            h[s] = 1;
            while (!Q.empty()) {
                int node = Q.front();
                Q.pop();
                for (auto [x, edge] : G[node])
                    if (!h[x] && c[edge]) {
                        h[x] = h[node] + 1;
                        Q.emplace(x);
                    }
            }
            if (!h[t])
                break;
            function<ll(int, ll)> dfs = [&](int node, ll cur)->ll {
                if (node == t) {
                    flow += cur;
                    return cur;
                }
                for (; pos[node] < G[node].size(); ++pos[node]) {
                    auto [x, edge] = G[node][pos[node]];
                    if (h[x] == h[node] + 1 && c[edge]) {
                        ll act = dfs(x, min(cur, c[edge]));
                        if (act) {
                            c[edge] -= act, c[edge ^ 1] += act;
                            return act;
                        }
                    }
                }
                return 0;
            };
            while (dfs(s, LLONG_MAX));
        }
        return flow;
    }
};

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    Dinic G(n);
    for (int i = 1; i <= m; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        G.AddEdge(x, y, z);
    }
    cout << G.MaxFlow(s, t);
}