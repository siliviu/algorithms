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
    SegTree() {}
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

class Tree {
private:
    int n, root, t = 0;
    vector<int> v, p, tin, d, s, heavy, r, inv;
    vector<vector<int>> G;
    SegTree T;

    void dfs0(int node, int last) {
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

    void dfsh(int node, int last) {
        tin[node] = ++t;
        inv[t] = v[node];
        for (auto x : G[node])
            if (x != last) {
                r[x] = x == G[node][0] ? r[node] : x;
                dfsh(x, node);
            }
    };

    void ProcessPath(int x, int y, function<void(int, int)> Op) {
        for (; r[x] != r[y]; y = p[r[y]]) {
            if (d[r[x]] > d[r[y]])
                swap(x, y);
            Op(tin[r[y]], tin[y]);
        }
        if (d[x] > d[y])
            swap(x, y);
        Op(tin[x], tin[y]);
    };
public:
    Tree(int _n, vector<int>& _v, vector<vector<int>>& _G, int _root = 1) : n(_n), v(_v), G(_G), root(_root) {
        p.resize(n + 1), tin.resize(n + 1), d.resize(n + 1), s.assign(n + 1, 1), heavy.resize(n + 1), r.resize(n + 1), inv.resize(n + 1);
        dfs0(root, 0);
        dfsh(root, 0);
        T = SegTree(n, inv);
    }

    void UpdateNode(int node, int val) {
        T.Set(tin[node], val);
    }
    ll QueryNode(int node) {
        return T.Query(tin[node]);
    }

    void UpdateSubtree(int node, int val) {
        T.Set(tin[node], tin[node] + s[node] - 1, val);
    }
    ll QuerySubtree(int node) {
        return T.Query(tin[node], tin[node] + s[node] - 1);
    }

    void UpdatePath(int node1, int node2, int val) {
        ProcessPath(node1, node2, [&](int x, int y) {T.Set(x, y, val); });
    }
    ll QueryPath(int node1, int node2) {
        ll ans = 0;
        ProcessPath(node1, node2, [&](int x, int y) {ans += T.Query(x, y); });
        return ans;
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    cin.tie(0)->sync_with_stdio(0);
    vector<int> a(n + 1);
    vector<vector<int>> G(n + 1);
    for (int i = 1; i <= n; ++i)
        cin >> a[i];
    for (int i = 1; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        G[x].emplace_back(y);
        G[y].emplace_back(x);
    }
    Tree T(n, a, G);
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
                T.UpdateNode(x, z);
            else
                cout << T.QueryNode(x) << '\n';
        }
        else if (tt == 2) {
            if (t == 'U')
                T.UpdateSubtree(x, z);
            else
                cout << T.QuerySubtree(x) << '\n';
        }
        else if (tt == 3) {
            if (t == 'U')
                T.UpdatePath(x, y, z);
            else
                cout << T.QueryPath(x, y) << '\n';
        }
    }
}