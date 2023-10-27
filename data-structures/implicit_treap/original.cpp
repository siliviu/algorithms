#include <bits/stdc++.h>

using namespace std;

struct TreapNode {
	int w, s = 1, lazy2 = 0, rev = 0;
	long long v, sum = 0, lazy = 0;
	TreapNode* l = 0, * r = 0;
	TreapNode(int _v) : v(_v), w(rand()), sum(_v) {};
} *root;

void update(TreapNode* nod) {
	if (nod)
		nod->s = (nod->l ? nod->l->s : 0) + (nod->r ? nod->r->s : 0) + 1,
		nod->sum = (nod->lazy2 ? 1LL * nod->lazy2 * nod->s : (nod->l ? nod->l->sum : 0) + (nod->r ? nod->r->sum : 0) + nod->v) + nod->lazy * nod->s;
}

void propagate(TreapNode* nod) {
	if (!nod) return;
	if (nod->rev) {
		swap(nod->l, nod->r);
		if (nod->l)
			nod->l->rev ^= 1;
		if (nod->r)
			nod->r->rev ^= 1;
		nod->rev = 0;
	}
	if (nod->lazy2) {
		nod->v = nod->lazy2;
		if (nod->l)
			nod->l->lazy2 = nod->lazy2, nod->l->lazy = 0, nod->l->sum = 1LL * nod->lazy2 * nod->l->s;
		if (nod->r)
			nod->r->lazy2 = nod->lazy2, nod->r->lazy = 0, nod->r->sum = 1LL * nod->lazy2 * nod->r->s;
		nod->lazy2 = 0;
	}
	if (nod->lazy) {
		nod->v += nod->lazy;
		if (nod->l)
			nod->l->lazy += nod->lazy, nod->l->sum += nod->lazy * nod->l->s;
		if (nod->r)
			nod->r->lazy += nod->lazy, nod->r->sum += nod->lazy * nod->r->s;
		nod->lazy = 0;
	}
}

pair<TreapNode*, TreapNode*> split(TreapNode* n, int pos) {
	if (!n)
		return { 0,0 };
	propagate(n);
	pair<TreapNode*, TreapNode*> ans;
	if (pos <= (n->l ? n->l->s : 0)) {
		ans.second = n;
		auto [ll, rr] = split(n->l, pos);
		ans.first = ll;
		ans.second->l = rr;
		update(ans.second);
	}
	else {
		ans.first = n;
		auto [ll, rr] = split(n->r, pos - (n->l ? n->l->s : 0) - 1);
		ans.second = rr;
		ans.first->r = ll;
		update(ans.first);
	}
	return ans;
}

TreapNode* join(TreapNode* nl, TreapNode* nr) {
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

void dfs(TreapNode* n) {
	if (!n) return;
	propagate(n);
	dfs(n->l);
	cout << n->v << ' ';
	dfs(n->r);
}

int32_t main() {
	cin.tie(0)->sync_with_stdio(0);
	srand(time(0));
	char t;
	int n, m, x, y, z;
	cin >> n >> m;
	for (int i = 1; i <= n; ++i) {
		cin >> x;
		root = join(root, new TreapNode(x));
	}
	while (m--) {
		cin >> t >> x;
		if (t == '+' || t == '=' || t == 'R') {
			cin >> y;
			if (t != 'R') cin >> z;
			auto [l, r] = split(root, y);
			auto [ll, rr] = split(l, x - 1);
			if (t == '+') {
				rr->lazy += z;
			}
			else if (t == '=') {
				rr->lazy2 = z;
				rr->lazy = 0;
			}
			else
				rr->rev ^= 1;
			propagate(rr);
			update(rr);
			root = join(join(ll, rr), r);
		}
		else if (t == 'D') {
			auto [l, r] = split(root, x);
			auto [ll, rr] = split(l, x - 1);
			root = join(ll, r);
		}
		else if (t == 'I') {
			cin >> z;
			auto [l, r] = split(root, x - 1);
			root = join(join(l, new TreapNode(z)), r);
		}
		else {
			cin >> y;
			auto [l, r] = split(root, y);
			auto [ll, rr] = split(l, x - 1);
			cout << rr->sum << '\n';
			root = join(join(ll, rr), r);
		}
	}
	dfs(root);
}