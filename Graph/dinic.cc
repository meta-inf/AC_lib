//@ Maxflow (Dinic)
/*
 * Dinic Algorithm for maximum flow.
 * Last Edit : Apr 2013
 * */

typedef long long LL;

namespace Dinic {
	const int N = 16384, M = N << 4;
	struct edge { int t; LL c; edge *op, *n; }
		ebf[M], *ec = ebf, *e[N], *er[N];
	int n, s, t, d[N];
	inline void add_edge (int s, int t, LL c)
	{
		*ec = (edge){t, c, ec + 1, er[s]}; er[s] = ec++;
		*ec = (edge){s, 0, ec - 1, er[t]}; er[t] = ec++;
	}
	inline void reset ()
	{
		memset(er, 0, sizeof(er[0]) * (n + 1));
		memset(d, 0, sizeof(d[0]) * (n + 1));
		n = 0; ec = ebf;
	}
	bool modlabel ()
	{
		static int que[M];
		int l(0), r(0), x, y;
		memcpy(e, er, sizeof(er[0]) * (n + 1));
		memset(d, 0xff, sizeof(d[0]) * (n + 1));
		d[que[++r] = s] = 0;
		while (l != r)
			for (edge *i = er[x = que[++l]]; i; i = i->n)
				if (i->c && d[y = i->t] < 0) d[que[++r] = y] = d[x] + 1;
		return d[t] >= 0;
	}
	LL augument (int x, LL c)
	{
		if (x == t) return c;
		LL r(c), de; int y; edge *i;
		for (i = e[x]; i; i = i->n)
			if (i->c && d[x] + 1 == d[y = i->t])
			{
				de = augument(y, min(r, i->c));
				i->c -= de, i->op->c += de, r -= de;
				if (!r) break;
			}
		e[x] = i;
		if (r) d[x] = -1; // CRUCIAL OPTIMIZATION
		return c - r;
	}
	LL solve (int _n, int _s, int _t)
	{
		LL res = 0, de; n = _n; s = _s; t = _t;
		while (modlabel()) while (de = augument(s, ~0ULL >> 4)) res += de;
		return res;
	}
}

