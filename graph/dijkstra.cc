/*
 * SSSP - Dijkstra
 * Not able to handle negative-weighed edges.
 * */
namespace G
{
	using namespace CLR;
	const int N = 20050, M = 200050;
	struct edge { int t, w; edge *n; } ebf[M * 2], *ec = ebf, *e[N];
	int dis[N]; // 1-based
 
	inline void link (int u, int v, int w)
	{
		*ec = (edge){v, w, e[u]}; e[u] = ec++;
		*ec = (edge){u, w, e[v]}; e[v] = ec++;
	}
	inline void Dijkstra (int s)
	{
		priority_queue<pi> pque;
		fill(dis, dis + n + 1, inf);
		pque.push(pi(-(dis[s] = 0), s));
		while (!pque.empty())
		{
			int x = pque.top().second, w = -pque.top().first; pque.pop();
			if (dis[x] != w) continue;
			for (edge *i = e[x]; i; i = i->n)
				if (dis[x] + i->w < dis[i->t])
					pque.push(pi(-(dis[i->t] = dis[x] + i->w), i->t));
		}
	}
}
