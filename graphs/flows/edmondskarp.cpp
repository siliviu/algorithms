// O(V*E^2)
#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<int>> G(n + 1);
    vector c(n + 1, vector<ll>(n + 1));
    while (m--) {
        int x, y, z;
        cin >> x >> y >> z;
        G[x].emplace_back(y);
        G[y].emplace_back(x);
        c[x][y] += z;
    }
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
    cout << flow;
}