// O(VE*EVlog)
#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using pi = pair<int, int>;

int main() {
	int n, m, s, t;
	cin >> n >> m;
	s = 1, t = n;
	vector<vector<int>> G(n + 1);
	vector<tuple<int, int, int>> E(2 * m + 2);
	for (int i = 1; i <= m; ++i) {
		int x, y, z, t;
		cin >> x >> y >> z >> t;
		G[x].emplace_back(2 * i);
		G[y].emplace_back(2 * i + 1);
		E[2 * i] = { y,z,t };
		E[2 * i + 1] = { x,0,-t };
	}
	int flow = 0, cost = 0;
	while (1) {
		vector<int> dp(n + 1, INT_MAX);
		vector<pair<int, int>> last(n + 1);
		auto comp = [&](pi& a, pi& b) { return a.second > b.second; };
		priority_queue<pi, vector<pi>, decltype(comp)> PQ(comp);
		PQ.emplace(s, 0);
		dp[s] = 0;
		while (!PQ.empty()) {
			auto [node, cost] = PQ.top();
			PQ.pop();
			if (dp[node] == cost)
				for (auto nr : G[node]) {
					auto& [x, c, w] = E[nr];
					if (cost + w < dp[x] && c) {
						dp[x] = cost + w;
						last[x] = { nr, node };
						PQ.emplace(x, cost + w);
					}
				}
		}
		if (!last[t].second)
			break;
		int curflow = INT_MAX, curcost = 0;
		for (int i = t; i != s; i = last[i].second)
			curflow = min(curflow, get<1>(E[last[i].first]));
		for (int i = t; i != s; i = last[i].second) {
			curcost += curflow * get<2>(E[last[i].first]);
			get<1>(E[last[i].first]) -= curflow;
			get<1>(E[last[i].first ^ 1]) += curflow;;
		}
		flow += curflow, cost += curcost;
	}
	cout << flow << ' ' << cost;
}