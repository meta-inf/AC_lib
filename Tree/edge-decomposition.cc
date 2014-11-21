//@ Edge Decomposition

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MXN = 300500, MXD = 20, inf = 100000000;
struct edge { int u, v, w, rev; edge *next, *dual; }
	ebuff[MXN + MXN], *cur = ebuff, *e[MXN];
inline void add_edge (int u, int v, int w)
{ 
	*cur = (edge){u, v, w, 0, e[u], cur + 1}; e[u] = cur++;
	*cur = (edge){v, u, w, 0, e[v], cur - 1}; e[v] = cur++; 
}

int diff, dis[MXN][MXD], root[MXN][MXD], stot, crt;
int log2[MXN];
int rid[MXN * 5], mxdis[MXN * 5], cwhite[MXN * 5], BITBuff[MXN * MXD];
edge *midedge[MXN * 5], *me;
bool white[MXN];
int *BIT[MXN * 5], *Bcur = BITBuff;
int tstamp[MXN], curstamp;

int det_edge (int u)
{
	int size(1), tm; tstamp[u] = curstamp;
	for (edge *i = e[u]; i; i = i->next)
		if (!i->rev && tstamp[i->v] != curstamp)
		{
			tm = det_edge(i->v);
			size += tm;
			if (abs(stot - tm - tm) < diff)
				diff = abs(stot - tm - tm), me = i;
		}
	return size;
}

int maxdis;
inline int update (int u, int au, int di, int de)
{
	int size(1); maxdis = max(maxdis, di);
	tstamp[u] = curstamp;
	dis[u][de] = di; root[u][de] = crt;
	for (edge *i = e[u]; i; i = i->next)
		if (!i->rev && tstamp[i->v] != curstamp)
			size += update(i->v, u, di + i->w, de);
	return size;
}
void divide (int u, int d)
{
	if (stot == 1) return;
	edge *tm = new edge, *be;
	int s1, s2; 
	diff = inf; ++curstamp; det_edge(u);
	me->rev = me->dual->rev = 1;
	be = me;
	
	*tm = (edge){crt + 1, crt + 2, me->w, 0, 0x0, 0x0};
	rid[++crt] = be->u; maxdis = -inf; curstamp++;
	s1 = update(be->u, -1, 1, d); mxdis[crt] = maxdis,
	BIT[crt] = Bcur; Bcur += maxdis + 1; midedge[crt] = tm;
	rid[++crt] = be->v; maxdis = -inf; 
	s2 = update(be->v, -1, 1, d); mxdis[crt] = maxdis,
	BIT[crt] = Bcur; Bcur += maxdis + 1; midedge[crt] = tm;
	
	stot = s1; divide(be->u, d + 1);
	stot = s2; divide(be->v, d + 1);
}

inline void print_edge (int u)
{
	for (edge *i = e[u]; i; i = i->next)
		fprintf(stderr, "%d %d %d %d\n", i->u, i->v, i->w, i->rev);
}

int Q[MXN], w[MXN], cv, vis[MXN];
inline void adjust (int u, int au)
{ vis[u] = true;
	for (edge *i = e[u]; i; i = i->next)
		if (i->v != au) adjust(i->v, u);
	int l(1), r(1);
	for (edge *i = e[u]; i; i = i->next)
		if (i->v != au) { ++r; Q[r] = i->v, w[r] = 1; }
	vis[u] = false;
	if (r - l <= 2) return;
	for (edge *i = e[u]; i; i = i->next)
		if (!vis[i->v]) i->rev = i->dual->rev = 1;
	while (r - l > 2)
	{ 
		++l; add_edge(cv, Q[l], w[l]);
		++l; add_edge(cv, Q[l], w[l]);
		++r; Q[r] = cv++, w[r] = 0;
	}
	while (l < r) { ++l; add_edge(u, Q[l], w[l]); }
	//print_edge(u);
}

#define low(X) (X&(-X))
inline int getKth (int *B, int N, int K)
{
	int p(0), re(0);
	for (int i = log2[N - 1]; i >= 0; --i)
	{
		p += 1 << i;
		if (p > N || re + B[p] >= K)
			p -= 1 << i;
		else re += B[p];
	}
	return p + 1;
}
inline void modify (int *B, int N, int i, int delta)
{ for (; i <= N; i += low(i)) B[i] += delta; }

inline int query (int u)
{
	if (white[u]) return 0;
	int res(inf), a, b, tmp;
	for (int i = 0; i < 20; ++i)
	{
		if ((b = root[u][i]) == 0) break;
		a = (b == midedge[b]->u) ? midedge[b]->v : midedge[b]->u;
		if (!cwhite[a]) continue;
		tmp = midedge[b]->w + dis[u][i] + getKth(BIT[a], mxdis[a], 1) - 2;
		if (tmp < res) res = tmp;
	}
	return res == inf ? -1 : res;
}

inline void blacken (int u)
{
	int b; white[u] = false;
	for (int i = 0; i < 20; ++i)
	{
		if ((b = root[u][i]) == 0) break;
		--cwhite[b];
		modify(BIT[b], mxdis[b], dis[u][i], -1);
	}
}
inline void whiten (int u)
{
	int b; white[u] = true;
	for (int i = 0; i < 20; ++i)
	{
		if ((b = root[u][i]) == 0) break;
		++cwhite[b];
		modify(BIT[b], mxdis[b], dis[u][i], +1);
	}
}
inline void flip (int u)
  { if (white[u]) blacken(u); else whiten(u); }

int main ()
{
	int N, tm(0);
#ifdef LOCAL
	freopen("qtree5.in", "r", stdin);
#endif
	scanf("%d", &N);
	for (int i = 1; i <= 300000; ++i)
	{
		if ((1 << tm) < i) ++tm;
		log2[i] = tm;
	}
	for (int i = 1; i < N; ++i)
	{
		int a, b; scanf("%d%d", &a, &b);
		add_edge(a, b, 1);
	}
	cv = N + 1; adjust(1, -1);
	stot = cv - 1; divide(1, 0);
	scanf("%d", &tm); while (tm--)
	{
		int a, b; scanf("%d%d", &a, &b);
		if (!a) flip(b);
		else printf("%d\n", query(b));
	} 
	return 0;
}



