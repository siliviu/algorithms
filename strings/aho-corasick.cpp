#include <bits/stdc++.h>
 
using namespace std;
 
struct node {
	node* c[26], * back;
	vector<node*> invback;
	int cnt;
}*root;
 
vector<node*> ans;
 
void Insert(const string& a) {
	node* cur = root;
	for (auto x : a) {
		if (!cur->c[x - 'a'])
			cur->c[x - 'a'] = new node();
		cur = cur->c[x - 'a'];
	}
	ans.emplace_back(cur);
}
 
void Build() {
	queue<node*> Q;
	root->back = root;
	Q.emplace(root);
	while (!Q.empty()) {
		auto top = Q.front();
		Q.pop();
		for (int i = 0; i < 26; ++i)
			if (top->c[i]) {
				node* cur = top->back;
				while (cur != root && !cur->c[i])
					cur = cur->back;
				if (cur->c[i] && cur->c[i] != top->c[i]) {
					top->c[i]->back = cur->c[i];
					cur->c[i]->invback.emplace_back(top->c[i]);
				}
				else {
					top->c[i]->back = root;
					root->invback.emplace_back(top->c[i]);
				}
				Q.emplace(top->c[i]);
			}
	}
}
 
void dfs(node* node) {
	for (auto x : node->invback) {
		dfs(x);
		node->cnt += x->cnt;
	}
}
void Query(const string& a) {
	node* cur = root;
	for (auto x : a) {
		while (cur != root && !cur->c[x - 'a'])
			cur = cur->back;
		if (cur->c[x - 'a']) {
			cur = cur->c[x - 'a'];
			++cur->cnt;
		}
	}
	dfs(root);
}
 
int main() {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	root = new node();
	string s, x;
	int m;
	cin >> s >> m;
	for (int i = 0; i < m; ++i) {
		cin >> x;
		Insert(x);
	}
	Build();
	Query(s);
	for (auto x : ans)
		cout << x->cnt << '\n';
}