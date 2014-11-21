//@ Node Decomposition

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

typedef long long lint;
const int N = 100050, M = 2000500;

struct edge { int t, w; edge *n; }
ebf[N << 2], *ec = ebf, *e[N];
#define forEdges(I,U) for(edge*I=e[U];I;I=I->n)
inline void addEdge (int u, int v, int w)
{
	*ec = (edge){v, w, e[u]}; e[u] = ec++;
	*ec = (edge){u, w, e[v]}; e[v] = ec++;
}

int root[N], size[N], que[M], dep[N], fa[N], n;
lint dis[N];
vector<lint> maxDis[M];
vector<pair<int, int> > child[M];
int start[N], L, U, f, r;
int gravity, cid;

void getGravity (int rt, int ID)
{
	int x, y, z;
	for (int i = r; i > 0; --i) size[que[i]] = 1;
	for (int i = r; i > 0; --i)
		size[fa[que[i]]] += size[que[i]];
	int mn(N), cur, id(-1);
	for (int i = r; i > 0; --i)
	{
		cur = 0;
		forEdges(it, x = que[i])
			if (root[y = it->t] == rt && y != fa[x])
				cur = max(cur, size[y]);
		cur = max(cur, size[rt] - size[x]);
		if (cur < mn) mn = cur, id = x;
	}
	if ((gravity = id) == -1) return;
	forEdges(it, gravity) if (root[it->t] == rt)
		child[ID].push_back(make_pair(it->t, it->w));
	root[gravity] = gravity;
	forEdges(it, gravity) if (root[it->t] == rt)
	{
		root[x = it->t] = x;
		f = r = 0; que[++r] = x; fa[x] = 0;
		while (f < r)
			forEdges(i, y = que[++f])
				if (root[z = i->t] == rt && z != fa[y])
					fa[z] = y, root[z] = x, que[++r] = z;
	}
}

void calc (int rt, vector<lint> &res)
{
	int x, y; f = r = 0;
	que[++r] = rt; dep[rt] = 1, dis[rt] = fa[rt] = 0; 
	while (f < r)
		forEdges(it, x = que[++f])
			if (root[y = it->t] == rt && y != fa[x])
			{
				dep[y] = dep[x] + 1, dis[y] = dis[x] + it->w;
				que[++r] = y; fa[y] = x;
			}
	x = 0;
	for (int i = r; i > 0; --i) if (dep[que[i]] > x) x = dep[que[i]];
	res.resize(x + 1);
	for (int i = r; i > 0; --i)
		res[dep[que[i]]] = max(res[dep[que[i]]], dis[que[i]]);
}

int solve (int x)
{
	int r = ++cid;
	calc(x, maxDis[r]);
	getGravity(x, r);
	if (gravity == -1) return r;
	for (int i = 0; i < child[r].size(); ++i)
	{	
		int id = (child[r][i].first = solve(child[r][i].first));
		for (int j = 0; j < maxDis[id].size(); ++j)
			maxDis[id][j] += child[r][i].second;
	}
	return r;
}

bool getMax (vector<lint> &x,
		vector<lint> &y, double delt)
{
	int i;
	for (f = 1, r = 1, i = 1; i < y.size(); ++i)
	{
		if (L - i >= 1 && L - i < x.size())
		{
			for (; f < r; --r)
				if (x[L - i] - delt * (L - i) < x[que[r]] - delt * que[r]) break;
			que[++r] = L - i;
		}
		while (f < r && que[f + 1] + i > U) ++f;
		if (f < r && x[que[f + 1]] + y[i] >= delt * (que[f + 1] + i)) return true;
	}
	return false;
}

vector<lint> tmp;
bool check (double delt)
{
	for (int i = 1; i <= cid; ++i)
		for (int j = 1; j < maxDis[i].size(); ++j)
			if ((j - 1) >= L && (j - 1) <= U && (maxDis[i][j] - maxDis[i][0]) >= (j - 1) * delt)
				return true;
	for (int i = 1; i <= cid; ++i)
	{
		if (child[i].empty()) continue;
		tmp = maxDis[child[i][0].first];
		for (int j = 1; j < child[i].size(); ++j)
		{
			__typeof(tmp) &cur = maxDis[child[i][j].first];
			if (getMax(tmp, cur, delt) || getMax(cur, tmp, delt)) return true;
			for (int t = 0; t < tmp.size() && t < cur.size(); ++t)
				tmp[t] = max(tmp[t], cur[t]);
			for (int t = tmp.size(); t < cur.size(); ++t) tmp.push_back(cur[t]);
		}
	}
	return 0;
}

int main ()
{
	scanf("%d%d%d", &n, &L, &U);
	double s(0.0), e(0.0);
	for (int i = 1; i < n; ++i)
	{
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);
		addEdge(a, b, c);
		if (c > e) e = c;
	}
	fill(root + 1, root + n + 1, 1);
	solve(1);
	while (e - s > 1e-5)
	{
		double m = (e + s) / 2;
		if (check(m)) s = m; else e = m;
	}
	printf("%.3lf\n", s);
	return 0;
}

