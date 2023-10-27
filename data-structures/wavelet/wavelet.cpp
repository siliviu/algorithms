#include <bits/stdc++.h>

using namespace std;

class WaveletTree {
    int nr;
    vector<vector<int>> t;
    void Build(auto start, auto end, int left, int right, int node) {
        if (left == right)
            return;
        int m = (left + right) / 2;
        t.reserve(end - start + 1);
        for (auto it = start;it != end;++it)
            t[node].emplace_back(t[node].back() + (*it <= m));
        auto pivot = stable_partition(start, end, [m](int x) {return x <= m;});
        Build(start, pivot, left, m, 2 * node);
        Build(pivot, end, m + 1, right, 2 * node + 1);
    }
    int Kth(int posl, int posr, int k, int left, int right, int node) {
        if (left == right)
            return left;
        int m = (left + right) / 2, ll = t[node][posl - 1], rr = t[node][posr], cnt = rr - ll;
        if (k <= cnt)
            return Kth(ll + 1, rr, k, left, m, 2 * node);
        return Kth(posl - ll, posr - rr, k - cnt, m + 1, right, 2 * node + 1);
    }
    int LTE(int posl, int posr, int k, int left, int right, int node) {
        if (left == right)
            return posr - posl + 1;
        int m = (left + right) / 2, ll = t[node][posl - 1], rr = t[node][posr], cnt = rr - ll;
        if (k <= m)
            return cnt ? LTE(ll + 1, rr, k, left, m, 2 * node) : 0;
        return cnt + (cnt != posr - posl + 1 ? LTE(posl - ll, posr - rr, k, m + 1, right, 2 * node + 1) : 0);
    }
    int Count(int posl, int posr, int k, int left, int right, int node) {
        if (left == right)
            return posr - posl + 1;
        int m = (left + right) / 2, ll = t[node][posl - 1], rr = t[node][posr], cnt = rr - ll;
        if (k <= m)
            return cnt ? Count(ll + 1, rr, k, left, m, 2 * node) : 0;
        return cnt != posr - posl + 1 ? Count(posl - ll, posr - rr, k, m + 1, right, 2 * node + 1) : 0;
    }
public:
    WaveletTree(int _nr, vector<int> a) : nr(_nr) {
        int s = 1;
        while (s < 2 * nr)
            s *= 2;
        t.assign(s, vector<int>(1));
        Build(a.begin(), a.end(), 1, nr, 1);
    }
    int Kth(int left, int right, int k) {
        return Kth(left, right, k, 1, nr, 1);
    }
    int LTE(int left, int right, int k) {
        if (k < 1)
            return 0;
        return LTE(left, right, k, 1, nr, 1);
    }
    int Count(int left, int right, int k) {
        if (k < 1 || k > nr)
            return 0;
        return Count(left, right, k, 1, nr, 1);
    }
};

int main() {
    cin.tie(0)->sync_with_stdio(0);
    int n;
    cin >> n;
    vector<int> a(n), inv(n + 1);
    for (int i = 0;i < n;++i) {
        cin >> a[i];
        inv[i + 1] = a[i];
    }
    sort(inv.begin() + 1, inv.end());
    inv.erase(unique(inv.begin() + 1, inv.end()), inv.end());
    int t = inv.size() - 1;
    for (auto& x : a)
        x = lower_bound(inv.begin() + 1, inv.end(), x) - inv.begin();
    WaveletTree wt(t, a);
    int m;
    cin >> m;
    while (m--) {
        int x, y, z;
        cin >> x >> y >> z;
        // KTH
        // cout << inv[wt.Kth(x, y, z)] << '\n';
        // RANK
        int k = upper_bound(inv.begin() + 1, inv.end(), z) - (inv.begin() + 1);
        cout << (y - x + 1) - wt.LTE(x, y, k) << '\n';
    }
}