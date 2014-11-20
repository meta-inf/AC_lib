//@ ChairTree
/*
 * Persistent segment tree set
 * Example: Kth value in range supporting modification
 * */

#include <cstdio>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

const int N = 10050;

struct node {
	int s;
	node *c[2];
	node (int _s = 0) : s(_s) { c[0] = c[1] = 0; }
};

inline int size (node *X) { return (X?(X->s):0); }

node *T[N], *que[N];
int ft[N], maxval, rear, n;
map<int, int> compr;
vector<int> revcmp;

node* insTree (node *t, int l, int r, int p, int de)
{
	node *re = new node();
	if (t) *re = *t; re->s += de;
	if (l != r)
	{
		int m = (l + r) >> 1;
		if (p <= m) re->l = insTree(lc(re), l, m, p, de);
		else re->r = insTree(rc(re), m + 1, r, p, de);
	}
	return re;
}

int leftsize ()
{
	int res(0);
	for (int i = 0; i < rear; ++i)
		if (que[i] && que[i]->c[0]) res += que[i]->c[0]->s * ft[i];
	return res;
}
void turn (int p) {
	for (int i = 0; i < rear; ++i)
		if (que[i]) que[i] = que[i]->c[p];
}
void expand_pre (int p, int f) {
	for (; p; p -= p & -p)
		ft[rear] = f, que[rear] = T[p], rear++;
}
int query (int l, int r, int k)
{
	rear = 0;
	expand_pre(r, +1), expand_pre(l - 1, -1);
	int tl = 1, tr = maxval;
	while (tl != tr)
	{
		int tm = (tl + tr) >> 1, ts = leftsize();
		if (ts >= k) turn(0), tr = tm;
		else turn(1), tl = tm + 1, k -= ts;
	}
	return revcmp[tl];
}
int dt[N], tdt[N];
void modify (int p, int de)
{
	int pd = tdt[p]; tdt[p] = de;
	for (; p <= n; p += p & -p)
	{
		T[p] = insTree(T[p], 1, maxval, pd, -1);
		T[p] = insTree(T[p], 1, maxval, de, +1);
	}
}

struct event { int ty, a, b, c; } ev[N];

int main ()
{
#ifdef LOCAL
	freopen("in", "r", stdin);
#endif
	int m;
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i)
	{
		scanf("%d", dt + i);
		compr[dt[i]] = 1;
	}
	for (int i = 1; i <= m; ++i)
	{
		char op[20];
		scanf("%s", op);
		if (op[0] == 'Q')
		{
			scanf("%d%d%d", &ev[i].a, &ev[i].b, &ev[i].c);
			ev[i].ty = 0;
		}
		else
		{
			scanf("%d%d", &ev[i].a, &ev[i].b);
			ev[i].ty = 1; compr[ev[i].b] = 1;
		}
	}
	maxval = 0;
	revcmp.resize(1 + compr.size());
	for (__typeof(compr.begin()) it = compr.begin(); it != compr.end(); ++it)
		revcmp[it->second = ++maxval] = it->first;
	for (int i = 1; i <= n; ++i)
		modify(i, compr[dt[i]]);
	for (int i = 1; i <= m; ++i)
	{
		if (ev[i].ty == 0)
			printf("%d\n", query(ev[i].a, ev[i].b, ev[i].c));
		else
			modify(ev[i].a, compr[ev[i].b]);
	}
	return 0;
}

