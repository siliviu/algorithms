#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class SegTree {
private:
    struct node {
        ll v, lazya = 0;
        int lazys = 0;
    };
    int n;
    vector<node> st;
    void Propagate(int node, int left, int right) {
        if (st[node].lazys) {
            st[node].v = (right - left + 1LL) * st[node].lazys;
            if (left != right) {
                st[2 * node].lazya = st[2 * node + 1].lazya = 0;
                st[2 * node].lazys = st[2 * node + 1].lazys = st[node].lazys;
            }
            st[node].lazys = 0;
        }
        if (st[node].lazya) {
            st[node].v += (right - left + 1LL) * st[node].lazya;
            if (left != right) {
                st[2 * node].lazya += st[node].lazya;
                st[2 * node + 1].lazya += st[node].lazya;
            }
            st[node].lazya = 0;
        }
    }
    void Build(vector<int>& a, int node, int left, int right) {
        if (left == right) {
            st[node].v = a[left];
            return;
        }
        int m = (left + right) / 2;
        Build(a, 2 * node, left, m), Build(a, 2 * node + 1, m + 1, right);
        st[node].v = st[2 * node].v + st[2 * node + 1].v;
    }
    void _Update(int posleft, int posright, int mode, int val, int node, int left, int right) {
        if (posleft <= left && right <= posright) {
            if (!mode)
                st[node].lazya += val;
            else
                st[node].lazya = 0, st[node].lazys = val;
            Propagate(node, left, right);
            return;
        }
        Propagate(node, left, right);
        int m = (left + right) / 2;
        if (posleft <= m)
            Propagate(2 * node + 1, m + 1, right), _Update(posleft, posright, mode, val, 2 * node, left, m);
        if (posright > m)
            Propagate(2 * node, left, m), _Update(posleft, posright, mode, val, 2 * node + 1, m + 1, right);
        st[node].v = st[2 * node].v + st[2 * node + 1].v;
    }
    ll _Query(int posleft, int posright, int node, int left, int right) {
        Propagate(node, left, right);
        if (posleft <= left && right <= posright)
            return st[node].v;
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
        while (s < 2 * n)
            s *= 2;
        st.resize(s);
        Build(a, 1, 1, n);
    }
    void Set(int pos, int val) {
        Set(pos, pos, val);
    }
    void Set(int l, int r, int val) {
        _Update(l, r, 1, val, 1, 1, n);
    }
    void Add(int pos, int val) {
        Add(pos, pos, val);
    }
    void Add(int l, int r, int val) {
        _Update(l, r, 0, val, 1, 1, n);
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
    int n, q, t, x, y, z;
    cin >> n >> q;
    vector<int> a(n + 1);
    for (int i = 1;i <= n;++i)
        cin >> a[i];
    SegTree st(n, a);
    while (q--) {
        cin >> t >> x >> y;
        if (t == 3)
            cout << st.Query(x, y) << '\n';
        else {
            cin >> z;
            if (t == 1)
                st.Add(x, y, z);
            else
                st.Set(x, y, z);
        }
    }
}


