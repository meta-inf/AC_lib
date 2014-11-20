//@ MST with Degree Limit
/*
 * Minimum Spanning Tree with degree limit on undirected graphs.
 * O(N^3) due to poor implementation.
 * Last Edit : Before 2013
 * */
#include <cstdio>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

#define F first
#define S second

typedef pair<int, int> pi;
const int N = 500;

struct sset {
	int f[N];
	sset () { for (int i = 0; i < N; ++i) f[i] = i; }
	int gf (int x) { return f[x] == x ? x : (f[x] = gf(f[x])); }
	void merge (int x, int y) { f[gf(x)] = gf(y); }
} st;
struct enode {
	int x, y, w;
	bool operator< (const enode &b) const
		{ return w < b.w; }
} e[N * N];

int g[N][N], orig[N][N], n, m, res, deglimit;

inline void addEdge (int x, int y, int w) { res += w; g[x][y] = g[y][x] = w; }
inline void delEdge (int x, int y) { res -= g[x][y]; g[x][y] = g[y][x] = 0; }

void mst ()
{
	sort(e + 1, e + m + 1);
	for (int i = 1; i <= m; ++i)
		if (e[i].x != 1 && e[i].y != 1 &&
			st.gf(e[i].x) != st.gf(e[i].y))
		{
			st.merge(e[i].x, e[i].y);
			addEdge(e[i].x, e[i].y, e[i].w);
		}
}

enode dist[N];
bool direct[N];
void trav (int x, int ax, enode maxdist)
{
	dist[x] = maxdist;
	for (int y = 1; y <= n; ++y) if (g[x][y] && y != ax)
		trav(y, x, max(maxdist, (enode){x, y, g[x][y]}));
}

void dlmst ()
{
	map<int, pi> mindist;
	mst();
	for (int i = 2; i <= n; ++i)
		if (orig[i][1])
		{
			pi cur = pi(orig[i][1], i);
			if (!mindist.count(st.gf(i)) || mindist[st.gf(i)] > cur)
				mindist[st.gf(i)] = cur;
		}
	for (__typeof(mindist.begin()) it = mindist.begin(); it != mindist.end(); ++it)
		addEdge(it->S.S, 1, it->S.F), direct[it->S.S] = 1;
	int cdeg = mindist.size();
	for (; cdeg < deglimit; ++cdeg)
	{
		trav(1, 0, (enode){0, 0, 0});
		int bv = 0, ch = -1;
		for (int i = 2; i <= n; ++i)
			if (orig[1][i] && !direct[i])
				if (orig[1][i] - dist[i].w < bv)
					bv = orig[1][i] - dist[i].w,
					ch = i;
		if (bv == 0) return;
		delEdge(dist[ch].x, dist[ch].y);
		addEdge(1, ch, orig[1][ch]);
	}
}

map<string, int> idMap;
int get_id (string cur) {
	return idMap[cur] ? idMap[cur] : (idMap[cur] = ++n);
}

void init ()
{
	cin >> m;
	get_id("Park");
	for (int i = 1; i <= m; ++i)
	{
		string x, y;
		int w, a, b;
		cin >> x >> y >> w;
		a = get_id(x); b = get_id(y);
		e[i] = (enode){a, b, w};
		orig[a][b] = orig[b][a] = w;
	}
	cin >> deglimit;
}

int main ()
{
	freopen("in", "r", stdin);
	init ();
	dlmst();
	cout << "Total miles driven: " << res << endl;
	return 0;
}


