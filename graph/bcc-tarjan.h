
//finding cut points and bridges

int dfn[N], low[N], ts, n;
long long f[N];

void dfs (int x)
{
	int y, cc(0), ch(0);
	dfn[x] = low[x] = ++ts;
	for (edge *i = e[x]; i; i = i->n) if (i->v)
	{
		i->v = i->d->v = 0;
		if (!dfn[y = i->t])
		{
			++ch;
			dfs(y);
			// low[y] == dfn[x] : dumplicated edge -> x is a cut-point, i is not a bridge
			if (x != 1 && low[y] >= dfn[x]) ;//x is a cut point
			if (low[y] > dfn[x]) ;//i is a bridge
			low[x] = min(low[x], low[y]);
		} else low[x] = min(low[x], dfn[y]);
	}
	if (x == 1 && ch >= 2) ; // x is a cut point
}
int solve () { for (int i = 1; i <= n; ++i) if (!dfn[i]) dfs(i); }

//in order to find edge biconnected components, just remove the bridges and do floodfill
//in order to get vertex bcc, use the code below (the time complexity may degenerate to O(N^2))

struct graph_t
{
	struct edge { int t, i; edge *n; } eb[M << 1], *ec, *e[N];
	int valid[M], n, mcnt;
	inline void link (int u, int v)
	{ valid[++mcnt] = 1;
		*ec = (edge){v, mcnt, e[u]}; e[u] = ec++;
		*ec = (edge){u, mcnt, e[v]}; e[v] = ec++;
	}
	int dfn[N], low[N], id[N], st[N], color[N], top, icnt, cid, pvalid[N];
	void pop (int x, int cid, int ad = 0)
	{
		int cnt(0); static int buff[N], vis[N], color[N];
		while (st[top] != x) id[buff[++cnt] = st[top--]] = cid;
		id[buff[++cnt] = st[top--]] = cid;
		if (ad)
		{
			int add(0);
			for (edge *i = e[ad]; i; i = i->n) add += id[i->t] == cid && i->t != ad; //~
			if (add > 1) buff[++cnt] = ad;
		}
	}
	void dfs (int x, int ax)
	{
		static int tstamp = 0;
		dfn[x] = low[x] = ++tstamp; st[++top] = x;
		for (edge *i = e[x]; i; i = i->n) if (valid[i->i])
		{
			valid[i->i] = 0;
			if (!dfn[i->t])
			{
				dfs(i->t, x); low[x] = min(low[x], low[i->t]);
				(dfn[i->t] == low[i->t]) ? pop(i->t, id[x] = ++icnt, x) : void();
			}
			else low[x] = min(low[x], dfn[i->t]);
		}
		if (!ax) pop(x, ++icnt);
	}
	void solve ()
	{
		for (int i = 1; i <= n; ++i) if (!dfn[i]) dfs(i, 0);
	}
	graph_t () { ec = eb; mcnt = 0; }
} G;

