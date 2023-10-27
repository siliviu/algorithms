#include <bits/stdc++.h>

using namespace std;
using ll = long long;

class LiChao {
    int n;
    struct Line {
        ll m, b;
        Line(ll _m = 0, ll _b = 1e18) : m(_m), b(_b) {}
        ll Calc(ll x) const {
            return 1LL * m * x + b;
        }
    };
    vector<Line> st;
    void Add(Line line, int node, int left, int right) {
        int m = (left + right) / 2;
        bool sl = line.Calc(left) < st[node].Calc(left), sm = line.Calc(m) < st[node].Calc(m);
        if (sm)
            swap(st[node], line);
        if (left == right)
            return;
        if (sl ^ sm)
            Add(line, 2 * node, left, m);
        else
            Add(line, 2 * node + 1, m + 1, right);
    };
    ll Get(int point, int node, int left, int right) {
        if (left == right)
            return st[node].Calc(point);
        int m = (left + right) / 2;
        ll ans = st[node].Calc(point);
        if (point <= m)
            ans = min(ans, Get(point, 2 * node, left, m));
        else
            ans = min(ans, Get(point, 2 * node + 1, m + 1, right));
        return ans;
    }
public:
    LiChao(int _n) : n(_n) {
        int s = 1;
        while (s < 2 * n)
            s *= 2;
        st.resize(s);
    }
    void Add(const Line& line) {
        Add(line, 1, 1, n);
    }
    ll Get(int x) {
        return Get(x, 1, 1, n);
    }
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n;
    cin >> n;
    vector<int> s(n + 1), f(n + 1);
    vector<ll> dp(n + 1);
    cin >> f[0];
    for (int i = 1; i <= n; ++i)
        cin >> s[i];
    for (int i = 1; i <= n; ++i)
        cin >> f[i];
    LiChao t(1e6);
    t.Add({ f[0],0 });
    for (int i = 1; i <= n; ++i) {
        dp[i] = t.Get(s[i]);
        t.Add({ f[i], dp[i] });
    }
    cout << dp[n];
}