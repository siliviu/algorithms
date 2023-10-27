// O(V*E^2)
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

struct EdmondsKarp {
    int n;
    vector<vector<int>> G;
    vector<vector<ll>> c;
    EdmondsKarp(int _n) : n(_n), G(n + 1), c(n + 1, vector<ll>(n + 1)) {};
    void AddEdge(int from, int to, int cap) {
        if (from == to)
            return;
        G[from].emplace_back(to);
        G[to].emplace_back(from);
        c[from][to] += cap;
    }
    ll MaxFlow(int s, int t) {
        ll flow = 0;
        while (1) {
            queue<int> Q;
            vector<int> p(n + 1);
            Q.emplace(s);
            while (!Q.empty()) {
                int node = Q.front();
                Q.pop();
                for (auto x : G[node])
                    if (!p[x] && c[node][x]) {
                        p[x] = node;
                        Q.emplace(x);
                    }
            }
            ll cur_flow = LLONG_MAX;
            if (!p[t])
                break;
            for (int cur = t; cur != s; cur = p[cur])
                cur_flow = min(cur_flow, c[p[cur]][cur]);
            for (int cur = t; cur != s; cur = p[cur]) {
                c[p[cur]][cur] -= cur_flow;
                c[cur][p[cur]] += cur_flow;
            }
            flow += cur_flow;
        }
        return flow;
    }
};

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    EdmondsKarp G(n);
    for (int i = 1; i <= m; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        G.AddEdge(x, y, z);
    }
    cout << G.MaxFlow(s, t);
}