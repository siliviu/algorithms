// O(V^2*sqrt(E)
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct PushRelabel {
    int n;
    vector<vector<pair<int, int>>> G;
    vector<vector<int>> hlist;
    vector<ll> c, e;
    vector<int> h, pos;
    PushRelabel(int _n) : n(_n), G(n + 1), hlist(2 * n), c(2), e(n + 1), h(n + 1), pos(n + 1) {};
    void AddEdge(int from, int to, int cap) {
        if (from == to)
            return;
        G[from].emplace_back(to, c.size());
        G[to].emplace_back(from, c.size() + 1);
        c.emplace_back(cap), c.emplace_back(0);
    }
    ll MaxFlow(int s, int t) {
        h[s] = n;
        queue<int> Q;
        Q.emplace(t);
        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            for (auto [x, edge] : G[node])
                if (!h[x] && c[edge ^ 1]) {
                    h[x] = h[node] + 1;
                    hlist[h[x]].emplace_back(x);
                    Q.emplace(x);
                }
        }
        int curh = n - 1;
        auto Push = [&](int node, int x, int edge, ll flow) {
            if (!e[x] && x != t)
                hlist[h[x]].emplace_back(x);
            e[x] += flow, e[node] -= flow;
            c[edge] -= flow, c[edge ^ 1] += flow;
        };
        auto Relabel = [&](int node) {
            int minh = INT_MAX, cur = 0, z = 0;
            for (const auto [x, edge] : G[node]) {
                if (c[edge] && minh > h[x] + 1)
                    minh = h[x] + 1, z = cur;
                ++cur;
            }
            pos[node] = z;
            h[node] = curh = minh;
        };
        for (const auto [x, edge] : G[s])
            Push(s, x, edge, c[edge]);
        while (1) {
            for (; hlist[curh].empty(); --curh)
                if (!curh)
                    return e[t];
            int node = hlist[curh].back();
            hlist[curh].pop_back();
            while (e[node]) {
                if (pos[node] == G[node].size()) {
                    Relabel(node);
                    continue;
                }
                const auto [x, edge] = G[node][pos[node]];
                if (c[edge] && h[node] == h[x] + 1)
                    Push(node, x, edge, min(c[edge], e[node]));
                ++pos[node];
            }
        }
    }
};

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    PushRelabel G(n);
    for (int i = 1; i <= m; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        G.AddEdge(x, y, z);
    }
    cout << G.MaxFlow(s, t);
}