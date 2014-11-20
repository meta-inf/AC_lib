//@ Tarjan (BCC)
/*
 * Tarjan algorithm on undirected graph
 * Finding cut points, bridges, biconnected components &
 * 2-edge-connected components
 * (biconnected components refers to vertex-bcc;
 * 2-edge-connected components refers to maximal bridgeless subgraphs.)
 * Last Edit : 15-5-2013
 * */

int dfn[N], low[N], ts, n;
long long f[N];

void dfs (int x, int ax = 0) {
	int y, cc(0), ch(0);
	dfn[x] = low[x] = ++ts;
	for (edge *i = e[x]; i; i = i->n) if (i->v) {
		i->v = i->d->v = 0;
		if (!dfn[y = i->t]) {
			++ch;
			dfs(y, x);
			// low[y] == dfn[x]: 
			// dumplicated edge -> x is a cut-point, i is not a bridge
			if (ax != 0 && low[y] >= dfn[x]) ;//x is a cut point
			if (low[y] > dfn[x]) ;//i is a bridge
			low[x] = min(low[x], low[y]);
		} else low[x] = min(low[x], dfn[y]);
	}
	if (ax == 0 && ch >= 2) ; // x is a cut point
}
int solve () { for (int i = 1; i <= n; ++i) if (!dfn[i]) dfs(i); }

//in order to find edge biconnected components, 
// just remove the bridges and do floodfill
//in order to get vertex bcc, use the code below

/*
 * UPDATE@20130515 : The previous version is wrong. Code fixed. 
 * The time complexity is now linear.
 * further test may be needed.
 * */

struct edge { int s, t, i; edge *n; } ebf[M * 2], *ec = ebf, *e[N];
inline void add_edge (int u, int v, int i) {
	*ec = (edge){u, v, i, e[u]}; e[u] = ec++;
	*ec = (edge){v, u, i, e[v]}; e[v] = ec++;
}

bool ans[N];
int top, dfn[N], low[N], times, scnt, evis[M], sID[M];
edge* S[M];

void tarjan (int x, int ax = 0) {
#define edges(x) ebf[x*2-2]
	dfn[x] = low[x] = ++times;
	for (edge *i = e[x]; i; i = i->n) {
		int y = i->t;
		if (evis[i->i]) continue;
		S[++top] = i;
		evis[i->i] = 1;
		if (dfn[y]) {
			low[x] = min(low[x], dfn[y]);
			continue;
		}
		tarjan(y, x);
		if (low[y] == dfn[x]) { // a Vertex-BCC with cutpoint x
			for (++scnt; S[top]->s != x; sID[S[top--]->i] = scnt) ;
			//assert(S[top]->s == x && S[top]->t == y);
			sID[S[top--]->i] = scnt;
		}
		low[x] = min(low[x], low[y]);
	}
	if (dfn[x] == low[x]) {
		// there should be only one edge connecting its ancestor
		if (ax == 0) return;
		//assert(S[top]->s == ax && S[top]->t == x);
		sID[S[top--]->i] = ++scnt;
	}
}

