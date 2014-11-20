//@ MinCostMaxflow (SSP Approach)
/*
 * Naive Successive Shortest Path Algorithm for cost flow.
 * Last Edit : before 2013
 * */

namespace mcFlow {
	const int N = 6050, M = 8000000, inf = ~0U >> 1;
	struct edge { int s, t, fl, co; edge *n, *d; }
		ebf[M], *ec = ebf, *e[N], *pre[N];
	int n, s, t, cost, flow, dist[N], inque[N];
	inline void addEdge (int s, int t, int f, int c)
	{
		*ec = (edge){s, t, f, c, e[s], ec + 1}; e[s] = ec++;
		*ec = (edge){t, s, 0,-c, e[t], ec - 1}; e[t] = ec++;
	}
	inline int addNode () { return ++n; }
	inline int addSrc () { return s = ++n; }
	inline int addSnk () { return t = ++n; }
	void augument ()
	{
		int co(0), fl(inf);
		for (int cr = t; cr != s; cr = pre[cr]->s)
			co += pre[cr]->co, fl = min(fl, pre[cr]->fl);
		cost += co * fl, flow += fl;
		for (int cr = t; cr != s; cr = pre[cr]->s)
			pre[cr]->fl -= fl, pre[cr]->d->fl += fl;
	}
	bool modlabel ()
	{
		fill(dist + 1, dist + n + 1, -inf);
		fill(inque + 1, inque + n + 1, 0);
		queue<int> que;
		que.push(s); dist[s] = 0; inque[s] = true;
		while (!que.empty())
		{
			int x = que.front(), y;
			que.pop(); inque[x] = false;
			for (edge *i = e[x]; i; i = i->n)
				if (i->fl && dist[x] + i->co > dist[y = i->t])
				{
					dist[y] = dist[x] + i->co, pre[y] = i;
					if (!inque[y]) inque[y] = true, que.push(y);
				}
		}
		return dist[t] > -inf;
	}
	void solve () { while (modlabel()) augument(); }
} 
