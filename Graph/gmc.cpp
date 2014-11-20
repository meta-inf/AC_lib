//@ Global Min-Cut
/*
 * O(N^3) Algorithm.
 * Last Edit: Feb 2011
 * */

#include <cstdio>
#include <cstring>
using namespace std;

#define N 505
const int inf = 100000000;
int n, g[N][N];

int min_cut ()
{
	int s, t, cut = inf, mx, u;
	static bool v[N], ex[N];
	static int w[N];
	memset(ex, 0, sizeof (int) * (n + 1));
	for (int l = 1; l < n; ++l)
	{
		memset(v, 0, sizeof (int) * (n + 1));
		memset(w, 0, sizeof (int) * (n + 1));
		for (int j = 1; j <= n; ++j)
		{
			mx = -inf, u = -1;
			for (int i = 1; i <= n; ++i)
				if (!v[i] && !ex[i] && w[i] > mx)
				{
					mx = w[i];
					u = i;
				}
			if (u == -1) break;
			s = t, t = u, v[u] = true;
			for (int i = 1; i <= n; ++i)
				if (!v[i]) w[i] += g[u][i];
		}
		if (w[t] < cut) cut = w[t];
		ex[s] = ex[t] = true;
		for (int i = 1; i <= n; ++i)
			if (!ex[i]) g[i][s] += g[i][t], g[s][i] += g[t][i];
		ex[s] = false;
	}
	return (cut < inf) ? cut : 0;
}

int v[N], tc;
void dfs (int x)
{
	v[x] = true;
	++tc;
	for (int i = 1; i <= n; ++i)
		if (!v[i] && g[x][i] > 0)
			dfs(i);
}
bool connect ()
{
	if (n == 1) return false;
	memset(v, 0, sizeof v);
	tc = 0;
	dfs(1);
	return (tc == n);
}

int main ()
{
	int m, a, b, c;
	while (scanf("%d%d", &n, &m) == 2)
	{
		memset(g, 0, sizeof g);
		for (int i = 0; i < m; ++i)
		{
			scanf("%d%d%d", &a, &b, &c);
			++a;
			++b;
			g[a][b] += c;
			g[b][a] += c;
		}
		if (!connect()) printf("0\n");
		else printf("%d\n", min_cut());
	}
	return 0;
}
