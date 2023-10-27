#include <bits/stdc++.h>
 
using namespace std;
using ll = long long;
 
struct Line {
    ll m, b;
    Line(ll _m = 0, ll _b = 1e18) : m(_m), b(_b) {}
    ll Calc(ll x) {
        return 1LL * m * x + b;
    }
} ST[4000000];
 
void Add(Line line, int node = 1, int left = 1, int right = 1000000) {
    int m = (left + right) / 2;
    bool sl = line.Calc(left) < ST[node].Calc(left), sm = line.Calc(m) < ST[node].Calc(m);
    if (sm)
        swap(ST[node], line);
    if (left == right)
        return;
    if (sl ^ sm)
        Add(line, 2 * node, left, m);
    else
        Add(line, 2 * node + 1, m + 1, right);
};
 
ll Get(int point, int node = 1, int left = 1, int right = 1000000) {
    if (left == right)
        return ST[node].Calc(point);
    int m = (left + right) / 2;
    ll ans = ST[node].Calc(point);
    if (point <= m)
        ans = min(ans, Get(point, 2 * node, left, m));
    else
        ans = min(ans, Get(point, 2 * node + 1, m + 1, right));
    return ans;
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
    Add({f[0],0});
    for (int i = 1; i <= n; ++i) {
        dp[i] = Get(s[i]);
        Add({f[i], dp[i]});
    }
    cout << dp[n];
}