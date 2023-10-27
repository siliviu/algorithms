#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n, q, x, y, t = 0;
    cin >> n >> q;
    vector<int> tin(n + 1), tout(n + 1);
    vector<vector<int>> dp(18, vector<int>(n + 1)), G(n + 1);
    for (int i = 2; i <= n; ++i) {
        cin >> x;
        dp[0][i] = x;
        G[x].emplace_back(i);
        G[i].emplace_back(x);
    }
    function<void(int, int)> dfs = [&](int node, int last) {
        tin[node] = ++t;
        for (auto x : G[node])
            if (x != last)
                dfs(x, node);
        tout[node] = t;
        };
    auto check = [&](int a, int b) { return (tin[a] <= tin[b] && tout[a] >= tout[b]) || (tin[b] <= tin[a] && tout[b] >= tout[a]); };
    dfs(1, 0);
    tout[0] = n;
    for (int i = 2; i <= n; ++i)
        for (int j = 1; j <= 17; ++j)
            dp[j][i] = dp[j - 1][dp[j - 1][i]];
    while (q--) {
        cin >> x >> y;
        if (check(x, y)) {
            cout << (tin[x] < tin[y] ? x : y) << '\n';
            continue;
        }
        for (int i = 17; i >= 0; --i)
            if (!check(dp[i][x], y))
                x = dp[i][x];
        cout << dp[0][x] << '\n';
    }
}