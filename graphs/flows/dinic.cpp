// O(V^2*E)
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<pair<int, int>>> G(n + 1);
    vector<ll> c(2 * m + 2);
    for (int i = 1; i <= m; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        G[x].emplace_back(y, 2 * i);
        G[y].emplace_back(x, 2 * i + 1);
        c[2 * i] = z;
    }
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
    cout << flow;
}