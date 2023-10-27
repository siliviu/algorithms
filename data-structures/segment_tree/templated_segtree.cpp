#include <bits/stdc++.h>

using namespace std;
using ll = long long;

template <class T, T E, auto F>
class SegTree {
    int n;
    vector<T> st;
private:
    void Build(vector<T>& a, int node, int left, int right) {
        if (left == right) {
            st[node] = a[left];
            return;
        }
        int m = (left + right) / 2;
        Build(a, 2 * node, left, m), Build(a, 2 * node + 1, m + 1, right);
        st[node] = F(st[2 * node], st[2 * node + 1]);
    }
    void _Update(int pos, T val, int node, int left, int right) {
        if (left == right) {
            st[node] = val;
            return;
        }
        int m = (left + right) / 2;
        if (pos <= m)
            _Update(pos, val, 2 * node, left, m);
        else
            _Update(pos, val, 2 * node + 1, m + 1, right);
        st[node] = F(st[2 * node], st[2 * node + 1]);
    }
    T _Query(int posleft, int posright, int node, int left, int right) {
        if (posleft <= left && right <= posright)
            return st[node];
        int m = (left + right) / 2;
        T ans = E;
        if (posleft <= m)
            ans = F(_Query(posleft, posright, 2 * node, left, m), ans);
        if (posright > m)
            ans = F(ans, _Query(posleft, posright, 2 * node + 1, m + 1, right));
        return ans;
    }
public:
    SegTree(int _n, vector<T>& a) : n(_n) {
        int s = 1;
        while (s < 2 * n)
            s *= 2;
        st.resize(s);
        Build(a, 1, 1, n);
    }
    void Update(int pos, T val) {
        _Update(pos, val, 1, 1, n);
    }
    T Query(int pos) {
        return Query(pos, pos);
    }
    T Query(int left, int right) {
        return _Query(left, right, 1, 1, n);
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int n, q, t, x, y;
    cin >> n >> q;
    vector<ll> a(n + 1);
    for (int i = 1;i <= n;++i)
        cin >> a[i];
    SegTree<ll, 0, [&](ll a, ll b) {return a + b;} > st{ n, a };
    while (q--) {
        cin >> t >> x >> y;
        if (t == 1)
            st.Update(x, y);
        else
            cout << st.Query(x, y) << '\n';
    }
}

