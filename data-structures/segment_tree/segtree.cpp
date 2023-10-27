#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class SegTree {
    int n;
    vector<ll> st;
private:
    void Build(vector<int>& a, int node, int left, int right) {
        if (left == right) {
            st[node] = a[left];
            return;
        }
        int m = (left + right) / 2;
        Build(a, 2 * node, left, m), Build(a, 2 * node + 1, m + 1, right);
        st[node] = st[2 * node] + st[2 * node + 1];
    }
    void _Update(int pos, int val, int node, int left, int right) {
        if (left == right) {
            st[node] = val;
            return;
        }
        int m = (left + right) / 2;
        if (pos <= m)
            _Update(pos, val, 2 * node, left, m);
        else
            _Update(pos, val, 2 * node + 1, m + 1, right);
        st[node] = st[2 * node] + st[2 * node + 1];
    }
    ll _Query(int posleft, int posright, int node, int left, int right) {
        if (posleft <= left && right <= posright)
            return st[node];
        int m = (left + right) / 2;
        ll ans = 0;
        if (posleft <= m)
            ans += _Query(posleft, posright, 2 * node, left, m);
        if (posright > m)
            ans += _Query(posleft, posright, 2 * node + 1, m + 1, right);
        return ans;
    }
public:
    SegTree(int _n, vector<int>& a) : n(_n) {
        int s = 1;
        while (s < 2* n)
            s *= 2;
        st.resize(s);
        Build(a, 1, 1, n);
    }
    void Update(int pos, int val) {
        _Update(pos, val, 1, 1, n);
    }
    ll Query(int pos) {
        return Query(pos, pos);
    }
    ll Query(int left, int right) {
        return _Query(left, right, 1, 1, n);
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int n, q, t, x, y;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1;i <= n;++i)
        cin >> a[i];
    SegTree st(n, a);
    while (q--) {
        cin >> t >> x >> y;
        if (t == 1)
            st.Update(x, y);
        else
            cout << st.Query(x, y) << '\n';
    }
}

