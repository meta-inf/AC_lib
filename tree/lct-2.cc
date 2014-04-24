//unrooted tree
#include <cstdio>
#include <cassert>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;

typedef pair<int, int> pi;
#define rep(I,L,R) for(__typeof(R)I(L);I<=R;++I)
#define reprv(I,L) for(int I(L);I;--I)
#define for_(IT,C) for(__typeof(C.begin())IT(C.begin());IT!=C.end();++IT)

namespace LCT {
	const int N = 100050;
	struct node {
		static node *nil;
		node *f, *c[2], *maxp;
		int rev, w, maxw;
		void reverse ()
		  { if (this != nil) rev ^= 1, swap(c[0], c[1]); }
		void push ()
		  { if (rev) c[0]->reverse(), c[1]->reverse(), rev = 0; }
		void update () {
			if (c[0]->maxw > c[1]->maxw) maxw = (maxp = c[0]->maxp)->w;
			else maxw = (maxp = c[1]->maxp)->w;
			if (w > maxw) maxw = (maxp = this)->w;
		}
		void setc (node *x, int t) { c[t] = x, x->f = this; }
		int ty () { return this == f->c[1]; }
		int root () { return this == f->c[0] || this == f->c[1]; }
	} buff[N << 3], *node::nil = buff, *cur = buff + 1;
	
	node* newNode (int w) {
		cur->f = cur->c[0] = cur->c[1] = node::nil;
		cur->w = cur->maxw = w; cur->maxp = cur; cur->rev = 0;
		return cur++;
	}
	inline void zig (node *x)
	{
		node *y = x; y->push(); x->push();
		int w = x->ty();
		y->setc(x->c[!w], w);
		x->f = y->f; if (!y->root()) y->f->c[y->ty()] = x;
		x->setc(y, !w); y->update();
	}
	void splay (node *x) {
		for (x->push(); !x->root(); zig(x)) ; x->update();
	}
	void access (node *x) {
		for (node *y(x), *z(node::nil); y != node::nil; z = y, y = y->f)
			splay(y), y->setc(z, 1), y->update();
		splay(x);
	}
	pair<int, node*> _query (node *x, node *y)
	{
		int res(0); node *rp(node::nil);
		access(x);
		for (node *z(node::nil); y != node::nil; z = y, y = y->f)
		{
			splay(y);
			if (y->f == node::nil) // Node y, Tree y->c[1] && z
			{
				if (y->w > z->maxw) res = (rp = y)->w;
				else res = (rp = z->maxp)->w;
				if (y->c[1]->maxw > res) res = (rp = y->c[1]->maxp)->w;
			}
			y->setc(z, 1), y->update();
		}
		return make_pair(res, rp);
	}
	
	inline void evert (node *x) { access(x); x->reverse(); }
	inline void relink (node *x, node *y) { evert(x); x->f = y; access(x); }
	inline void cut (node *x) // x is a weight-node. equals cut(pre(x), next(x)) on the graph
	{
		splay(x); // pre(x) && next(x) have been visited and are now on the same tree in the context.
		assert(x->c[0] != node::nil && x->c[1] != node::nil);
		x->c[0]->f = x->c[1]->f = node::nil;
		x->c[0] = x->c[1] = node::nil;
	}

	node* treeNode[N];
	inline void updateGraph (int u, int v, int nw)
	{
		node* r = _query(treeNode[u], treeNode[v]).second;
		if (r->w <= nw) return;
		cut(r);
		node* t = newNode(nw);
		relink(treeNode[u], t);
		relink(treeNode[v], t);
	}
	int query (int u, int v) { return _query(treeNode[u], treeNode[v]).first; }

	struct edge { int t, w; edge *n; } ebf[N << 1], *ec = ebf, *e[N];
	inline void addEdge (int u, int v, int w)
	{
		*ec = (edge){v, w, e[u]}; e[u] = ec++;
		*ec = (edge){u, w, e[v]}; e[v] = ec++;
	}
	void dfs (int x, int ax)
	{
		treeNode[x] = newNode(0);
		for (edge *i = e[x]; i; i = i->n) if (i->t != ax)
			dfs(i->t, x),
			(treeNode[i->t]->f = newNode(i->w))->f = treeNode[x];
	}
	inline void init () { dfs(1, 0); }
	
	void printNode (node *t, int de)
	{
		if (t == node::nil) return;
		fputc('\t', stderr);
		reprv(i, de) fputc(' ', stderr);
		fprintf(stderr, "Node %d : c(%d, %d) f %d w %d maxw %d maxp %d rev %d type %d\n",
			t - buff, t->c[0] - buff, t->c[1] - buff, t->f - buff, 
			t->w, t->maxw, t->maxp - buff, t->rev, t->ty());
		printNode(t->c[0], de + 1);
		printNode(t->c[1], de + 1);
	}
	void printTree () {
		rep(it, buff, cur - 1)
			if (it->root()) printNode(it, 0);
	}
}

namespace parser {
	const int N = 100050;
	map<int, int> e[N], er[N];
	pair<int, pi> qr[N], edges[N];
	int n, f[N], qCnt;
	
	int nextInt ()
	{
		char ch = getchar();
		for ( ; ch > '9' || ch < '0'; ch = getchar());
		int tmp = 0;
		for ( ; '0' <= ch && ch <= '9'; ch = getchar())
			tmp = tmp * 10 + int(ch) - 48;
		return tmp;
	}
	void readData ()
	{
		n = nextInt();
		int m = nextInt(), q = nextInt();
		while (m--)
		{
			int a = nextInt(), b = nextInt(), c = nextInt();
			e[a][b] = c; e[b][a] = c;
		}
		reprv(i, q)
		{
			int a = nextInt(), b = nextInt(), c = nextInt();
			qr[i] = make_pair(a, pi(b, c));
			if (a == 2)
				er[b][c] = er[c][b] = e[b][c],
				e[b].erase(c), e[c].erase(b);
		} qCnt = q;
	}
	
	int find (int x) { return f[x] == x ? f[x] : (f[x] = find(f[x])); }
	void getMST ()
	{
		int m(0), k(0);
		rep(i, 1, n) f[i] = i;
		rep(i, 1, n) for_(it, e[i])
			edges[m++] = make_pair(it->second, pi(i, it->first));
		sort(edges, edges + m);
		rep(i, 0, m - 1)
		{
			int a = edges[i].second.first, b = edges[i].second.second, c = edges[i].first;
			if (find(a) != find(b))
			{
				f[f[a]] = f[b];
				LCT::addEdge(a, b, c);
				if (++k == n - 1) break;
			}
		}
	}
	
	int res[N], rCnt;
	void solve ()
	{
		readData();
		getMST();
		LCT::init();
		reprv(i, qCnt) {
			int a = qr[i].first, b = qr[i].second.first, c = qr[i].second.second;
			if (a == 1) res[++rCnt] = LCT::query(b, c);
			else LCT::updateGraph(b, c, er[b][c]);
		}
		reprv(i, rCnt) printf("%d\n", res[i]);
	}
}

int main ()
{
#ifndef ONLINE_JUDGE
	freopen("in", "r", stdin);
#endif
	parser::solve();
	return 0;
}

