//@ MST (Directed Graph, V^3 Approach)
/*
 * Minimal Spanning Tree on Direted Graph : Chu-liu/Edmonds Algorithm
 * Time Complexity O(V^3)
 * Code checked
 * Last Edit : Before 2013
 * */
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const int N = 256;
const double inf = 1e50, eps = 1e-6;

double G[N][N];
int vis[N], n, pre[N], removed[N];

void dfs (int x) {
	vis[x] = 1;
	for (int i = 1; i <= n; ++i) if (G[x][i] < inf && !vis[i]) dfs(i);
}

void dmst ()
{
	double res(0.0);
	fill(removed, removed + n + 1, 0);
	while (1)
	{
	// For each node, let its precursor be the node with minimum weigh to it
		for (int i = 2; i <= n; ++i) if (!removed[i])
		{
			G[pre[i] = i][i] = inf;
			for (int j = 1; j <= n; ++j) if (!removed[j])
				if (G[j][i] < G[pre[i]][i]) pre[i] = j;
		}
		bool found = false;
		// Find a circuit and reduce it
		for (int i = 2; i <= n; ++i) if (!removed[i])
		{
			int j, k; fill(vis, vis + n + 1, 0);
			for (j = i; j != 1 && !vis[j]; j = pre[j]) vis[j] = 1;
			if (j == 1) continue;
			found = true; i = j;
			res += G[pre[i]][i];
			/*
			 * Contract the circle : 
			 * (Temporarily) include all the edges on the circle,
			 * and adjust the cost of edges adjacent to the circle,
			 * so that to select one of them ((u, v) for instance) 
			 * is equivalent to remove the edge (pre<v>, v) on the circle.
			 * Easy to figure out that in the last iteration, 
			 * when selecting all remaining |V'|-1 edges, 
			 * the subgraph selected will expand to a rooted tree.
			 * */
			for (j = pre[i]; j != i; j = pre[j]) res += G[pre[j]][j], removed[j] = 1;
			for (k = 1; k <= n; ++k) if (!removed[k])
				if (G[k][i] < inf) G[k][i] -= G[pre[i]][i];
			// Adjust the cost of edges
			for (j = pre[i]; j != i; j = pre[j])
				for (k = 1; k <= n; ++k) if (!removed[k])
				{
					if (G[k][j] < inf)
						G[k][i] = min(G[k][i], G[k][j] - G[pre[j]][j]);
					G[i][k] = min(G[i][k], G[j][k]);
					// as i represents the the whole circle now.
				}
			break;
		}
		if (!found)
		{
			// select all |V'|-1 edges
			for (int i = 2; i <= n; ++i) if (!removed[i]) res += G[pre[i]][i];
			break;
		}
	}
	printf("%.2lf\n", res);
}

int x[N], y[N];

inline double sqr (double x) { return x * x; }
inline double edist (int a, int b) { return sqrt(sqr(double(x[a] - x[b])) + sqr(double(y[a] - y[b]))); }

void solve ()
{
	int m; scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i)
		scanf("%d%d", x + i, y + i);
	for (int i = 0; i <= n; ++i)
		for (int j = 0; j <= n; ++j)
			G[i][j] = inf * 2.;
	while (m--)
	{
		int s, t; scanf("%d%d", &s, &t);
		G[s][t] = edist(s, t);
	}
	fill(vis, vis + n + 1, 0); dfs(1);
	bool able(true);
	for (int i = 1; i <= n; ++i) if (!vis[i]) { able = false; break; }
	if (!able) puts("poor snoopy"); else dmst();
}

int main ()
{
#ifdef LOCAL
	freopen("in", "r", stdin);
#else
	freopen("network.in", "r", stdin);
	freopen("network.out", "w", stdout);
#endif
	int _; scanf("%d", &_); while (_--) solve();
	return 0;
}



