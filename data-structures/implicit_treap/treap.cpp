#include <bits/stdc++.h>

using namespace std;



class Treap {
private:
	struct TreapNode {
		int w, s = 1;
		long long v;
		TreapNode* l = 0, * r = 0;

		int lazy2 = 0, rev = 0;
		long long sum = 0, lazy = 0;

		TreapNode(int _v) : v(_v), w(rand()), sum(_v) {};

		friend void update(TreapNode* node) {
			if (node)
				node->s = (node->l ? node->l->s : 0) + (node->r ? node->r->s : 0) + 1,
				node->sum = (node->lazy2 ? 1LL * node->lazy2 * node->s : (node->l ? node->l->sum : 0) + (node->r ? node->r->sum : 0) + node->v) + node->lazy * node->s;
		}

		friend void propagate(TreapNode* node) {
			if (!node) return;
			if (node->rev) {
				swap(node->l, node->r);
				if (node->l)
					node->l->rev ^= 1;
				if (node->r)
					node->r->rev ^= 1;
				node->rev = 0;
			}
			if (node->lazy2) {
				node->v = node->lazy2;
				if (node->l)
					node->l->lazy2 = node->lazy2, node->l->lazy = 0, node->l->sum = 1LL * node->lazy2 * node->l->s;
				if (node->r)
					node->r->lazy2 = node->lazy2, node->r->lazy = 0, node->r->sum = 1LL * node->lazy2 * node->r->s;
				node->lazy2 = 0;
			}
			if (node->lazy) {
				node->v += node->lazy;
				if (node->l)
					node->l->lazy += node->lazy, node->l->sum += node->lazy * node->l->s;
				if (node->r)
					node->r->lazy += node->lazy, node->r->sum += node->lazy * node->r->s;
				node->lazy = 0;
			}
		}

		friend void dfs(TreapNode* node) {
			if (!node) return;
			propagate(node);
			dfs(node->l);
			cout << node->v << ' ';
			dfs(node->r);
		}
	}*root = 0;

	static TreapNode* join(TreapNode* nl, TreapNode* nr) {
		if (!nl)
			return nr;
		if (!nr)
			return nl;
		propagate(nl), propagate(nr);
		if (nl->w > nr->w) {
			nl->r = join(nl->r, nr);
			update(nl);
			return nl;
		}
		nr->l = join(nl, nr->l);
		update(nr);
		return nr;
	}
	static pair<TreapNode*, TreapNode*> split(TreapNode* node, int pos) {
		if (!node)
			return { 0,0 };
		propagate(node);
		pair<TreapNode*, TreapNode*> ans;
		if (pos <= (node->l ? node->l->s : 0)) {
			ans.second = node;
			auto [ll, rr] = split(node->l, pos);
			ans.first = ll;
			ans.second->l = rr;
			update(ans.second);
		}
		else {
			ans.first = node;
			auto [ll, rr] = split(node->r, pos - (node->l ? node->l->s : 0) - 1);
			ans.second = rr;
			ans.first->r = ll;
			update(ans.first);
		}
		return ans;
	}
public:
	Treap(const vector<int>& a) {
		for (auto x : a)
			root = join(root, new TreapNode(x));
	}

	void Reverse(int left, int right) {
		auto [l, r] = split(root, right);
		auto [ll, rr] = split(l, left - 1);
		rr->rev ^= 1;
		propagate(rr);
		update(rr);
		root = join(join(ll, rr), r);
	}

	void Add(int pos, int val) {
		Add(pos, pos, val);
	}

	void Add(int left, int right, int val) {
		auto [l, r] = split(root, right);
		auto [ll, rr] = split(l, left - 1);
		rr->lazy += val;
		propagate(rr);
		update(rr);
		root = join(join(ll, rr), r);
	}

	void Set(int pos, int val) {
		Set(pos, pos, val);
	}

	void Set(int left, int right, int val) {
		auto [l, r] = split(root, right);
		auto [ll, rr] = split(l, left - 1);
		rr->lazy2 = val;
		rr->lazy = 0;
		propagate(rr);
		update(rr);
		root = join(join(ll, rr), r);
	}

	void Delete(int pos) {
		Delete(pos, pos);
	}

	void Delete(int left, int right) {
		auto [l, r] = split(root, right);
		auto [ll, rr] = split(l, left - 1);
		root = join(ll, r);
	}

	void Insert(int pos, int val) {
		auto [l, r] = split(root, pos - 1);
		root = join(join(l, new TreapNode(val)), r);
	}

	long long Query(int pos) {
		return Query(pos, pos);
	}

	long long Query(int left, int right) {
		auto [l, r] = split(root, right);
		auto [ll, rr] = split(l, left - 1);
		auto ans = rr->sum;
		root = join(join(ll, rr), r);
		return ans;
	}

	void QueryAll() {
		dfs(root);
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0);
	srand(time(0));
	char t;
	int n, m, x, y, z;
	cin >> n >> m;
	vector<int> a(n);
	for (auto& x : a)
		cin >> x;
	Treap treap(a);
	while (m--) {
		cin >> t >> x;
		if (t == '+') {
			cin >> y >> z;
			treap.Add(x, y, z);
		}
		else if (t == '=') {
			cin >> y >> z;
			treap.Set(x, y, z);
		}
		else if (t == 'R') {
			cin >> y;
			treap.Reverse(x, y);
		}
		else if (t == 'D')
			treap.Delete(x);
		else if (t == 'I') {
			cin >> z;
			treap.Insert(x, z);
		}
		else {
			cin >> y;
			cout << treap.Query(x, y) << '\n';
		}
	}
	treap.QueryAll();
}