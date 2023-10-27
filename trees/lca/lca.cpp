#include <bits/stdc++.h>

using namespace std;

class Tree {
    int n;
    vector<int> tin, tout;
    vector<vector<int>> G, dp;
    void dfs(int node, int last) {
        static int t = 0;
        tin[node] = ++t;
        for (auto x : G[node])
            if (x != last) {
                dp[0][x] = node;
                dfs(x, node);
            }
        tout[node] = t;
    };
public:
    Tree(int _n, vector<vector<int>>& _G) : n(_n), G(_G) {
        tin.resize(n + 1), tout.resize(n + 1), G.resize(n + 1), dp.resize(18, vector<int>(n + 1));
        dfs(1, 0);
        tout[0] = n;
        for (int i = 2; i <= n; ++i)
            for (int j = 1; j <= 17; ++j)
                dp[j][i] = dp[j - 1][dp[j - 1][i]];
    }
    bool Check(int a, int b) {
        return (tin[a] <= tin[b] && tout[a] >= tout[b]) || (tin[b] <= tin[a] && tout[b] >= tout[a]);
    };
    int LCA(int x, int y) {
        if (Check(x, y))
            return tin[x] < tin[y] ? x : y;
        for (int i = 17; i >= 0; --i)
            if (!Check(dp[i][x], y))
                x = dp[i][x];
        return dp[0][x];
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n, q, x, y;
    cin >> n >> q;
    vector<vector<int>> G(n + 1);
    for (int i = 2; i <= n; ++i) {
        cin >> x;
        G[x].emplace_back(i);
        G[i].emplace_back(x);
    }
    Tree t(n, G);
    while (q--) {
        cin >> x >> y;
        cout << t.LCA(x, y) << '\n';
    }
}