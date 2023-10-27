#include <bits/stdc++.h>
 
using namespace std;
 
int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    string s;
    cin >> s;
    int n = s.length(), ans = 0;
    s += s;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (ans + j == i || s[ans + j] < s[i + j]) {
                i += max(0, j - 1);
                break;
            }
            else if (s[ans + j] > s[i + j]) {
                ans = i;
                break;
            }
    cout << s.substr(ans, n);
}
