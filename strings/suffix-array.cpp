#include <bits/stdc++.h>
 
using namespace std;
using ll = long long;
 
int main() {
	string s;
	cin >> s;
	struct state {
		int len, link;
		map<char, int> next;
	};
	vector<state> st = {{0,-1}};
	int last = 0;
	auto add = [&](char c) {
		int cur = st.size();
		st.emplace_back();
		st[cur].len = st[last].len + 1;
		int p = last;
		for (; p != -1 && !st[p].next.count(c); p = st[p].link)
			st[p].next[c] = cur;
		if (p == -1)
			st[cur].link = 0;
		else {
			int q = st[p].next[c];
			if (st[p].len + 1 == st[q].len)
				st[cur].link = q;
			else {
				int clone = st.size();
				st.emplace_back(st[q]);
				st[clone].len = st[p].len + 1;
				for (; p != -1 && st[p].next[c] == q; p = st[p].link)
					st[p].next[c] = clone;
				st[q].link = st[cur].link = clone;
			}
		}
		last = cur;
	};
	for (auto x : s)
		add(x);
	int n = s.size(), m = st.size();
	vector<ll> cnt(n + 2);
	for (int i = 1; i < m; ++i) {
		++cnt[st[st[i].link].len + 1];
		--cnt[st[i].len + 1];
	}
	for (int i = 1; i <= n; ++i) 
		cout << (cnt[i] += cnt[i - 1]) << ' ';;
}