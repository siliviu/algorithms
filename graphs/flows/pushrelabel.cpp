// O(V^2*sqrt(E)
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<pair<int, int>>> G(n + 1);
    vector<vector<int>> hlist(2 * n);
    vector<ll> c(2 * m + 2), e(n + 1);
    vector<int> h(n + 1), pos(n + 1);
    for (int i = 1; i <= m; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        if (x == y)
            continue;
        G[x].emplace_back(y, 2 * i);
        G[y].emplace_back(x, 2 * i + 1);
        c[2 * i] = z;
    }
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
            if (!curh) {
                cout << e[t];
                return 0;
            }
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