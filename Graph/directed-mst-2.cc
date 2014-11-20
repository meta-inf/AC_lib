//@ MST (Directed Graph)
/*
 * Minimal Spanning Tree on Direted Graph : 
 * Improved version of Chu-liu/Edmonds Algorithm
 * Contract all circles in each iteration.
 * The time complexity is O(ElogV)
 * Tested; Last Edit Oct 2012
 * */
#include <cstdio>
#include <cstdlib>

#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

const int N = 100050, M = 100050;

struct info;
vector<info*> infoList;

struct info {
	int c, ty, utime; info *pos, *neg;
	void inc () { ++utime; }
	void push () {
		if (!ty)
		{
			pos->utime += utime;
			neg->utime -= utime;
			utime = 0;
		}
	}
	info (int cost) : c(cost), ty(1), pos(0), neg(0), utime(0) {}
	info (info *p, info *n) : pos(p), neg(n),
		c(p->c - n->c), ty(0), utime(0) { infoList.push_back(this); }
	info () {}
};

struct edge {
	int u, v; info *i;
	edge (int x, int y, int w) : u(x), v(y) {
		i = new info(w);
	}
	edge () {}
} e[M];
info* origList[M];

edge* predge[N];
info* preInfo[N];
int vis[N], tmp[N], n, m;

void dMST ()
{
#define pre(i) predge[i]->u
	int res(0), root(1), n(::n);
	while (1)
	{
		fill(predge, predge + n + 1, reinterpret_cast<edge*>(0x0));
		fill(vis, vis + n + 1, 0);
		fill(tmp, tmp + n + 1, 0);
		for (int i = 1; i <= m; ++i) if (e[i].u != e[i].v)
			if (!predge[e[i].v] || predge[e[i].v]->i->c > e[i].i->c)
				predge[e[i].v] = e + i;
		int n_vert = 0;
		for (int i = 1, j; i <= n; ++i) if (i != root)
		{
			if (!predge[i])
			{
				res = -1;
				break;
			}
			predge[i]->i->inc();
			res += predge[i]->i->c;
			for (j = i; !vis[j] && tmp[j] != i && j != root; j = pre(j))
				tmp[j] = i;
			if (vis[j] || j == root)
				continue;
			++n_vert;
			for (; !vis[j]; j = predge[j]->u) vis[j] = n_vert;
		}
		if (!n_vert) break;
		for (int i = 1; i <= n; ++i)
			if (!vis[i]) vis[i] = ++n_vert;
		for (int i = 1; i <= n; ++i)
			preInfo[i] = predge[i] ? predge[i]->i : 0x0;
		for (int i = 1; i <= m; ++i)
		{
			int v = e[i].v;
			e[i].u = vis[e[i].u]; e[i].v = vis[e[i].v];
			if (e[i].u != e[i].v)
				e[i].i = new info(e[i].i, preInfo[v]);
		}
		n = n_vert;
		root = vis[root];
	}
	if (res == -1) printf("-1\n");
	else
	{
		printf("%d\n", res);
		for (auto it = infoList.rbegin(); it != infoList.rend(); ++it)
			(*it)->push();
		for (int i = 1; i <= m; ++i)
		{
			if (origList[i]->utime < 0) throw;
			if (origList[i]->c && origList[i]->utime)
				printf("%d ", i);
		}
		puts("");
	}
}

int main ()
{
#ifdef LOCAL
	freopen("in", "r", stdin);
#else
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif
	scanf("%d%d", &n, &m);
	for (int i = 1, a, b, c; i <= m; ++i)
	{
		scanf("%d%d%d", &a, &b, &c);
		e[i] = edge(a, b, c);
		origList[i] = e[i].i;
	}
	dMST();
	return 0;
}
