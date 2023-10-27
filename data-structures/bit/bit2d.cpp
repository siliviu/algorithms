#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class BIT2D {
    int n, m;
    vector<vector<ll>> bit;
public:
    BIT2D(int _n, int _m) : n(_n), m(_m), bit(n + 1, vector<ll>(m + 1)) { }
    BIT2D(int _n, int _m, vector<vector<int>>& a) : n(_n), m(_m), bit(n + 1, vector<ll>(m + 1)) {
        for (int i = 1;i <= n;++i)
            for (int j = 1;j <= m;++j) {
                bit[i][j] += a[i][j];
                if (j + (j & -j) <= m)
                    bit[i][j + (j & -j)] += bit[i][j];
            }
        for (int i = 1;i <= n;++i)
            for (int j = 1;j <= m;++j)
                if (i + (i & -i) <= n)
                    bit[i + (i & -i)][j] += bit[i][j];
    }
    void Update(int y, int x, int val) {
        for (int i = y; i <= n; i += i & -i)
            for (int j = x; j <= n; j += j & -j)
                bit[i][j] += val;
    }
    ll Query(int y, int x) {
        ll ans = 0;
        for (int i = y; i; i -= i & -i)
            for (int j = x; j; j -= j & -j)
                ans += bit[i][j];
        return ans;
    }
    ll Query(int y1, int x1, int y2, int x2) {
        return Query(y2, x2) - Query(y2, x1 - 1) - Query(y1 - 1, x2) + Query(y1 - 1, x1 - 1);
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int n, q;
    cin >> n >> q;
    vector<vector<int>> a(n + 1, vector<int>(n + 1));
    for (int i = 1;i <= n;++i)
        for (int j = 1;j <= n;++j) {
            char x;
            cin >> x;
            if (x == '*')
                a[i][j] = 1;
        }
    BIT2D ft(n, n, a);
    while (q--) {
        int t, y1, x1, y2, x2;
        cin >> t >> y1 >> x1;
        if (t == 1) {
            ft.Update(y1, x1, a[y1][x1] ? -1 : 1);
            a[y1][x1] ^= 1;
        }
        else {
            cin >> y2 >> x2;
            cout << ft.Query(y1, x1, y2, x2) << '\n';
        }
    }
}