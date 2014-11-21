//@ Heavy-Light Decomposition

#include <cstdio>
#include <algorithm>
using namespace std;

const int N = 250050;

namespace CLR {
	int tID[N], chain[N], ccnt;
	int head[N];
}

namespace segTree {
	struct node {
		int l, r, m, rs;
		node *lc, *rc;
	} tbf[N << 2], *tc = tbf, *rt[N];
	inline node* newNode (int l, int r) {
		*tc = (node){l, r, (l + r) >> 1, r - l + 1, 0, 0};
		return tc ++;
	}
	void build (node *x)
	{
		if (x->l == x->r) return;
		build(x->lc = newNode(x->l, x->m));
		build(x->rc = newNode(x->m + 1, x->r));
	}
	int query (node *x, int l, int r)
	{
		if (l <= x->l && r >= x->r) return x->rs;
		int res(0);
		if (l <= x->m) res += query(x->lc, l, r);
		if (r > x->m) res += query(x->rc, l, r);
		return res;
	}
	void modify (node *x, int l, int r)
	{
		if (l <= x->l && r >= x->r) { x->rs = 0; return; }
		if (l <= x->m) modify(x->lc, l, r);
		if (r > x->m) modify(x->rc, l, r);
		x->rs = x->lc->rs + x->rc->rs;
	}

	using namespace CLR;

	inline int queryTree (int u, int v)
	{
		node *r = rt[chain[u]];
		if (tID[u] > tID[v]) swap(u, v);
		return query(r, tID[u], tID[v]);
	}
	inline void modifyTree (int u, int v)
	{
		node *r = rt[chain[u]];
		if (tID[u] > tID[v]) swap(u, v);
		modify(r, tID[u], tID[v]);
	}

	inline void buildChain (int *a, int cnt)
	{
		build(rt[++ ccnt] = newNode(1, cnt));
		for (int i = 1; i <= cnt; ++i)
			chain[a[i]] = ccnt, tID[a[i]] = i;
	}
}

namespace tree {
	using namespace CLR;

	struct edge { int t; edge *n; }
	ebf[N << 1], *ecr = ebf, *e[N];
	inline void link (int s, int t) {
		*ecr = (edge){t, e[s]}; e[s] = ecr++;
	}
#define forEdges(it,u) for(edge*it=e[u];it;it=it->n)

	int dep[N], pre[N], next[N], size[N], status[N];

	void init (int n)
	{
		static int que[N];
		int l(0), r(0), x;
		que[++r] = 1; dep[1] = 1;
		while (l < r)
			forEdges(it, x = que[++l])
				if (it->t != pre[x])
					pre[que[++r] = it->t] = x,
						dep[it->t] = dep[x] + 1;
		for (int i = r; i > 0; --i)
			x = que[i], size[pre[x]] += ++size[x];
		for (int i = 1; i <= r; ++i)
		{
			int maxSize(0), id(0);
			forEdges(it, x = que[i]) if (it->t != pre[x])
				if (size[it->t] > maxSize)
					maxSize = size[id = it->t];
			next[x] = id;
		}
		for (int i = 1; i <= n; ++i) if (next[pre[i]] != i)
		{
			int tc(0);
			for (int j = i; j; j = next[j])
				que[++tc] = j, head[j] = i;
			segTree::buildChain(que, tc);
		}
	}

	int query (int u)
	{
		int res(0);
		while (chain[u] != chain[1])
			res += segTree::queryTree(u, head[u]),
				u = pre[head[u]];
		if (u != 1)
			res += segTree::queryTree(next[1], u);
		return res;
	}

	void modify (int u, int v)
	{
		while (chain[u] != chain[v])
			if (dep[pre[head[u]]] < dep[pre[head[v]]])
				segTree::modifyTree(v, head[v]),
					v = pre[head[v]];
			else
				segTree::modifyTree(u, head[u]),
					u = pre[head[u]];
		if (dep[u] != dep[v])
		{
			if (dep[u] > dep[v]) swap(u, v);
			segTree::modifyTree(next[u], v);
		}
	}
}

int main ()
{
	using namespace tree;
	//freopen("in", "r", stdin);
	int n, a, b;
	scanf("%d", &n);
	for (int i = 1; i < n; ++i)
	{
		scanf("%d%d", &a, &b);
		link(a, b); link(b, a);
	}
	init(n);
	int m; scanf("%d", &m);
	m += n - 1;
	while (m--)
	{
		char op[20]; scanf("%s", op);
		if (op[0] == 'W')
		{
			scanf("%d", &a);
			printf("%d\n", query(a));
		}
		else
		{
			scanf("%d%d", &a, &b);
			modify(a, b);
		}
	}
	return 0;
}
