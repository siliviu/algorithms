#include <bits/stdc++.h>
 
using namespace std;
 
int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n, m, x, y;
    cin >> n >> m;
    vector<int> f(n + 1), s(n + 1), d(n + 1, 0x3F3F3F3F), seen(n + 1);
    vector<unordered_map<int, int>> dd(n + 1);
    vector<vector<int>> G(n + 1);
    for (int i = 1; i < n; ++i) {
        cin >> x >> y;
        G[x].emplace_back(y);
        G[y].emplace_back(x);
    }
    function<void(int, int)> dfs = [&](int node, int last) {
        s[node] = 1;
        for (auto x : G[node])
            if (x != last && !seen[x]) {
                dfs(x, node);
                s[node] += s[x];
            }
    };
    function<int(int, int, int)> centroid = [&](int node, int last, int n) {
        for (auto x : G[node])
            if (x != last && !seen[x] && s[x] > n / 2)
                return centroid(x, node, n);
        return node;
    };
    function<void(int, int, int)> dfs2 = [&](int c, int node, int last) {
        for (auto x : G[node])
            if (x != last && !seen[x]) {
                dd[c][x] = dd[c][node] + 1;
                dfs2(c, x, node);
            }
    };
    function<void(int, int)> Build = [&](int node, int lastc) {
        dfs(node, 0);
        int c = centroid(node, 0, s[node]);
        seen[c] = 1;
        f[c] = lastc;
        dfs2(c, c, 0);
        for (auto x : G[c])
            if (!seen[x])
                Build(x, c);
    };
    auto Paint = [&](int node) {
        for (int i = node; i; i = f[i])
            d[i] = min(d[i], dd[i][node]);
    };
    auto Query = [&](int node) {
        int ans = INT_MAX;
        for (int i = node; i; i = f[i])
            ans = min(ans, d[i] + dd[i][node]);
        return ans;
    };
    Build(1, 0);
    Paint(1);
    while (m--) {
        cin >> x >> y;
        if (x == 1)
            Paint(y);
        else
            cout << Query(y) << '\n';
    }
}