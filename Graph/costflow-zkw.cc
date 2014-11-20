//@ MinCostMaxflow (Zkw Algorithm)
/*
 * Zkw's Algorithm for min-cost-max-flow, inspired by SAP and KM.
 * Keep in mind that when there are negative-weighed edges along with zero-
 * weighed edges, the algorithm will misbehave. It will try to augument along 
 * zero-weighed edges first.
 * The fix is to force to augument along all negative edges before starting, 
 * using the idea of excess flow.
 * Last Edit : before 2013
 * */

namespace Zkw {

	const int M = int(2e5) + 50, N = int(1e3) + 50, inf = ~0u >> 2;

	struct edge { int t, f, c; edge *n, *d; }
	eb[M << 1], *ec = eb, *e[N];

	inline void add_edge (int s, int t, int c, int f)
	{
		*ec = (edge){t, f, c, e[s], ec + 1}; e[s] = ec++;
		*ec = (edge){s, 0,-c, e[t], ec - 1}; e[t] = ec++;
	}

	int vis[N], pi[N], slack[N], n, s, t, flow, cost, arr[N];

	bool modlabel ()
	{
		int d = inf;
		for (int i = 1; i <= n; ++i)
			if (!vis[i] && slack[i] < d) d = slack[i];
		if (d == inf) return false;
		for (int i = 1; i <= n; ++i) {
			if (vis[i]) pi[i] += d; slack[i] = inf;
		}
		return true;
	}
	int augument (int x, int f)
	{
		if (x == t) {
			arr[flow + 1] = pi[s]; flow += f; cost += f * pi[s]; return f;
		}
		vis[x] = true; int r(f), y, d;
		for (edge *i = e[x]; i; i = i->n) if (i->f && !vis[y = i->t])
			if (pi[x] == i->c + pi[y])
			{
				d = augument(y, min(r, i->f));
				if (!(i->f -= d, i->d->f += d, r -= d)) break;
			}
			else slack[y] = min(slack[y], -pi[x] + pi[y] + i->c);
		return f - r;
	}

	void costflow ()
	{
		fill(slack + 1, slack + n + 1, inf);
		do
			do memset(vis, 0, sizeof(int) * (n + 1));
			while (augument(s, inf));
		while (modlabel());
	}

}

