// O(V^2*sqrt(E)
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
	cin.tie(0)->sync_with_stdio(0);
	int n, m, s, t;
	cin >> n >> m >> s >> t;
	vector<tuple<int, int, int, int>> E(m);
	for (auto& [x, y, l, u] : E)
		cin >> x >> y >> l >> u;
	E.emplace_back(t, s, 0, INT_MAX);
	Dinic Flow(n + 2);
	vector<ll> off(n + 1);
	for (auto& [x, y, l, u] : E) {
		Flow.AddEdge(x + 1, y + 1, u - l);
		off[y] += l, off[x] -= l;
	}
	for (int i = 1; i <= n; ++i) {
		if (off[i] > 0)
			Flow.AddEdge(1, i + 1, off[i]);
		if (off[i] < 0)
			Flow.AddEdge(i + 1, n + 2, -off[i]);
	}
	Flow.MaxFlow(1, n + 2);
	for (auto [x, edge] : Flow.G[1])
		if (Flow.c[edge]) {
			cout << "please go home to sleep";
			return 0;
		}
	int ans = Flow.c[2 * (m + 1) ^ 1];
	Flow.c[2 * (m + 1)] = Flow.c[2 * (m + 1) ^ 1] = 0;
	cout << ans + Flow.MaxFlow(s + 1, t + 1);
}