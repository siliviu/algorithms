#include <bits/stdc++.h>
 
using namespace std;
using ll = long long;
using pi = pair<int, int>;
 
int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    int n;
    ll ans = LLONG_MAX;
    cin >> n;
    vector<pi> a(n), aux(n);
    for (auto& x : a)
        cin >> x.first >> x.second;
    sort(a.begin(), a.end());
    auto comp = [&](pi a, pi b) {return a.second < b.second; };
    function<void(int, int)> solve = [&](int left, int right) {
        if (right - left <= 3) {
            for (int i = left; i <= right; ++i)
                for (int j = i + 1; j <= right; ++j)
                    ans = min(ans, 1LL * (a[i].first - a[j].first) * (a[i].first - a[j].first) + 1LL * (a[i].second - a[j].second) * (a[i].second - a[j].second));
            sort(a.begin() + left, a.begin() + right + 1, comp);
            return;
        }
        int m = (left + right) / 2, mx = a[m].first;
        solve(left, m);
        solve(m + 1, right);
        inplace_merge(a.begin() + left, a.begin() + m + 1, a.begin() + right + 1, comp);
        for (int i = left, k = 0; i <= right; ++i)
            if (1LL * (a[i].first - mx) * (a[i].first - mx) < ans) {
                for (int j = k; j && 1LL * (a[i].second - aux[j].second) * (a[i].second - aux[j].second) < ans; --j)
                    ans = min(ans, 1LL * (a[i].first - aux[j].first) * (a[i].first - aux[j].first) + 1LL * (a[i].second - aux[j].second) * (a[i].second - aux[j].second));
                aux[++k] = a[i];
            }
    };
    solve(0, n - 1);
    cout << ans;
}