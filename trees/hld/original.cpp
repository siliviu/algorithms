#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int n, inv[1 << 18];
ll st[1 << 19], lazy[1 << 19];

void Build(int node = 1, int left = 1, int right = n) {
	if (left == right) {
		st[node] = inv[left];
		return;
	}
	int m = (left + right) / 2;
	Build(2 * node, left, m), Build(2 * node + 1, m + 1, right);
	st[node] = st[2 * node] + st[2 * node + 1];
}

void Propagate(int node, int left, int right) {
	if (lazy[node] != 0) {
		st[node] = (right - left + 1LL) * lazy[node];
		if (left != right)
			lazy[2 * node] = lazy[2 * node + 1] = lazy[node];
	}
	lazy[node] = 0;
}

void Update(int posleft, int posright, int val, int node = 1, int left = 1, int right = n) {
	if (posleft <= left && right <= posright) {
		lazy[node] = val;
		Propagate(node, left, right);
		return;
	}
	Propagate(node, left, right);
	int m = (left + right) / 2;
	if (posleft <= m)
		Update(posleft, posright, val, 2 * node, left, m), Propagate(2 * node + 1, m + 1, right);
	if (posright > m)
		Update(posleft, posright, val, 2 * node + 1, m + 1, right), Propagate(2 * node, left, m);
	st[node] = st[2 * node] + st[2 * node + 1];
}

ll Query(int posleft, int posright, int node = 1, int left = 1, int right = n) {
	Propagate(node, left, right);
	if (posleft <= left && right <= posright)
		return st[node];
	int m = (left + right) / 2;
	ll ans = 0;
	if (posleft <= m)
		ans += Query(posleft, posright, 2 * node, left, m);
	if (posright > m)
		ans += Query(posleft, posright, 2 * node + 1, m + 1, right);
	return ans;
}

int main() {
	int m, t = 0;
	cin >> n >> m;
	cin.tie(0)->sync_with_stdio(0);
	vector<int> a(n + 1), p(n + 1), tin(n + 1), d(n + 1), s(n + 1, 1), heavy(n + 1), r(n + 1);
	vector<vector<int>> G(n + 1);
	for (int i = 1; i <= n; ++i)
		cin >> a[i];
	for (int i = 1; i < n; ++i) {
		int x, y;
		cin >> x >> y;
		G[x].emplace_back(y);
		G[y].emplace_back(x);
	}
	function<void(int, int)> dfs0 = [&](int node, int last) {
		p[node] = last, d[node] = d[last] + 1;
		for (auto x : G[node])
			if (x != last) {
				dfs0(x, node);
				s[node] += s[x];
			}
		if (last)
			G[node].erase(find(G[node].begin(), G[node].end(), last));
		if (!G[node].empty()) {
			sort(G[node].begin(), G[node].end(), [&](int x, int y) {return s[x] > s[y]; });
			heavy[node] = G[node][0];
		}
	};
	dfs0(1, 0);
	function<void(int, int)> dfsh = [&](int node, int last) {
		tin[node] = ++t;
		inv[t] = a[node];
		for (auto x : G[node])
			if (x != last) {
				r[x] = x == G[node][0] ? r[node] : x;
				dfsh(x, node);
			}
	};
	dfsh(1, 0);
	auto ProcessPath = [&](int x, int y, function<void(int, int)> Op) {
		for (; r[x] != r[y]; y = p[r[y]]) {
			if (d[r[x]] > d[r[y]])
				swap(x, y);
			Op(tin[r[y]], tin[y]);
		}
		if (d[x] > d[y])
			swap(x, y);
		Op(tin[x], tin[y]);
	};
	Build();
	while (m--) {
		char t;
		int tt, x, y, z;
		cin >> t >> tt >> x;
		if (tt == 3)
			cin >> y;
		if (t == 'U')
			cin >> z;
		if (tt == 1) {
			if (t == 'U')
				Update(tin[x], tin[x], z);
			else
				cout << Query(tin[x], tin[x]) << '\n';
		}
		else if (tt == 2) {
			if (t == 'U')
				Update(tin[x], tin[x] + s[x] - 1, z);
			else
				cout << Query(tin[x], tin[x] + s[x] - 1) << '\n';
		}
		else if (tt == 3) {
			if (t == 'U') {
				ProcessPath(x, y, [&](int x, int y) {Update(x, y, z); });
			}
			else {
				ll ans = 0;
				ProcessPath(x, y, [&](int x, int y) {ans += Query(x, y); });
				cout << ans << '\n';
			}
		}
	}
}