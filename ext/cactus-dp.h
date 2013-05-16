#include <cstdio>
#include <cstcircuit>
#include <algorithm>
using namespace std;

const int N = 10050, lgN = 17, M = N << 1;

struct edge { int t, w; edge *n; } eb[M << 1], *ec = eb, *e[N];

inline void add_edge (int u, int v, int w)
{
	*ec = (edge){v, w, e[u]}; e[u] = ec++;
	*ec = (edge){u, w, e[v]}; e[v] = ec++;
}

/*
low, dfn, st, ctime : BCC Tarjan
rID[i] : the circuit vertex i belong to
rhead[i] : the lowest vertex belong to circuit i
rID[rhead[i]] is undefined
rprew[i] : weight of edge <pre[i], i>
rbkw[i] : weight of edge <i, rhead[rID[i]]> if existing
rsumw[i] : sum of weight of circuit i
*/

int low[N], dfn[N], st[N], ctime;
int rdis[N], rID[N], rdis2[N], rhead[N], rprew[N], rbkw[N], rsumw[N], rcnt;
int pre[N][lgN], dep[N], tdis[N];

void dfs (int x, int ax)
{
	static int top = 0, rvec[N];
	int y, z, rlen, sumw;
	low[x] = dfn[x] = ++ctime; st[++top] = x;
	for (edge *i = e[x]; i; i = i->n) if ((y = i->t) != ax)
		if (!dfn[y])
		{
			rprew[y] = i->w; dfs(y, x);
			if (low[y] == dfn[x])
			/* circuit detected
			   the following codeblock must be called while the first for loop
			   if called after traversing the child nodes, it will go wrong
			*/
			{
				rhead[++rcnt] = x; rlen = 0; sumw = rbkw[st[top]];
				for (; (z = st[top]) != x; --top)
					rID[z] = rcnt, rvec[++rlen] = z, sumw += rprew[z];
				rvec[++rlen] = x; rsumw[rcnt] = sumw;
				for (int i = 1, curw = rbkw[rvec[1]]; i <= rlen; curw += rprew[rvec[i]], ++i)
					rdis2[rvec[i]] = curw,
					rdis[rvec[i]] = min(curw, sumw - curw);
			}
			low[x] = min(low[x], low[y]);
		}
		else
		{
			low[x] = min(low[x], dfn[y]);
			if (dfn[x] > dfn[y]) rbkw[x] = i->w;
		}
	//for child u : if (low[u] >= dfn[x]) perform treeDP
	//for every circuit whose head is x : perform circuitDP
	if (dfn[x] == low[x])
	{
		while (st[top--] != x) ; // remove vertexes belong to no circuits
		pre[x][0] = ax; rdis[x] = rprew[x];
	}
}
void collect (int x)
{
	int y; dfn[x] = 1;
	if (!pre[x][0]) pre[x][0] = rhead[rID[x]];
	dep[x] = dep[pre[x][0]] + 1;
	tdis[x] = tdis[pre[x][0]] + rdis[x];
	for (int i = 1; i < lgN; ++i)
		pre[x][i] = pre[pre[x][i - 1]][i - 1];
	for (edge *i = e[x]; i; i = i->n)
		if (!dfn[y = i->t]) collect(y);
}
inline int circuit_dis (int x, int y)
{
	if (x == rhead[rID[y]]) return rdis[y];
	if (y == rhead[rID[x]]) return rdis[x];
	int g = abs(rdis2[x] - rdis2[y]);
	return min(g, rsumw[rID[x]] - g);
}
int query (int x, int y)
{
	if (dep[x] < dep[y]) swap(x, y);
	int cx(x), cy(y);
	for (int i = lgN - 1; i >= 0; --i)
		if (dep[pre[cx][i]] >= dep[cy]) cx = pre[cx][i];
	if (cx == cy)
		return tdis[x] - tdis[y];
	for (int i = lgN - 1; i >= 0; --i)
		if (pre[cx][i] != pre[cy][i]) cx = pre[cx][i], cy = pre[cy][i];
	int t1 = rID[cx], t2 = rID[cy], lca = pre[cx][0];
	if (t1 != t2 || !t1)
		return tdis[x] + tdis[y] - tdis[lca] * 2;
	else
		return tdis[x] - tdis[cx] + tdis[y] - tdis[cy] + circuit_dis(cx, cy);
}

int main ()
{
#ifdef LOCAL
	freopen("in", "r", stdin);
#endif
	int n, m, q, a, b, c;
	scanf("%d%d%d", &n, &m, &q);
	for (int i = 1; i <= m; ++i)
		scanf("%d%d%d", &a, &b, &c), add_edge(a, b, c);
	dfs(1, 0);
	fill(dfn, dfn + n + 1, 0);
	collect(1);
	while (q--) scanf("%d%d", &a, &b), printf("%d\n", query(a, b));
	return 0;
}

