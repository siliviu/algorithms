#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class BIT {
    int n;
    vector<ll> bit;
public:
    BIT(int _n) : n(_n), bit(n + 1) { }
    BIT(int _n, vector<int>& a) : n(_n), bit(n + 1) {
        for (int i = 1;i <= n;++i) {
            bit[i] += a[i];
            if (i + (i & -i) <= n)
                bit[i + (i & -i)] += bit[i];
        }
    }
    void Update(int pos, int val) {
        for (int i = pos;i <= n;i += i & -i)
            bit[i] += val;
    }
    ll Query(int pos) {
        ll ans = 0;
        for (int i = pos;i;i -= i & -i)
            ans += bit[i];
        return ans;
    }
    ll Query(int left, int right) {
        return Query(right) - Query(left - 1);
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int n, q, t, x, y;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1;i <= n;++i)
        cin >> a[i];
    BIT ft(n, a);
    while (q--) {
        cin >> t >> x >> y;
        if (t == 1) {
            ft.Update(x, y - a[x]);
            a[x] = y;
        }
        else
            cout << ft.Query(x, y) << '\n';
    }
}

