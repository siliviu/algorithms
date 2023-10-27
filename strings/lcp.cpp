#include <bits/stdc++.h>
 
using namespace std;
 
int main() {
    cin.tie(0)->sync_with_stdio(0);
    string s;
    cin >> s;
    int n = s.length();
    vector<int> pi(n);
    for (int i = 1, j = 0; i < n;)
        if (s[i] == s[j])
            pi[i++] = ++j;
        else if (j)
            j = pi[j - 1];
        else
            pi[i++] = 0;
    for (int i = 0; i < n; ++i)
        cout << pi[i] << ' ';
}