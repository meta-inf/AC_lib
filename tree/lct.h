// rooted tree

int c[N][2], f[N], key[N], sum[N], lsum[N], rsum[N], csum[N];
int size[N], same[N];

struct edge { int t; edge *n; }
	ebf[N << 1], *e[N], *ec = ebf;
inline void addEdge (int a, int b)
{
	*ec = (edge){b, e[a]}; e[a] = ec++;
	*ec = (edge){a, e[b]}; e[b] = ec++;
}

inline void initNode (int x)
{
	size[x] = 1; sum[x] = key[x];
	lsum[x] = rsum[x] = csum[x] = max(sum[x], 0);
}

inline int nodeType (int t) { return t == c[f[t]][1]; }
inline bool root (int t)
  { return t != c[f[t]][0] && t != c[f[t]][1]; }
inline void setc (int x, int y, int t) { c[x][t] = y; f[y] = x; }
inline void update (int x)
{
	if (!x) return;
	// update X
}
inline void makesame (int x, int val)
{
	same[x] = 1; key[x] = val;
	sum[x] = size[x] * val;
	lsum[x] = rsum[x] = csum[x] = max(0, sum[x]);
}
inline void pushDown (int x)
{
	if (x && same[x])
	{
		makesame(c[x][0], key[x]);
		makesame(c[x][1], key[x]);
		same[x] = 0;
	}
}

inline void zig (int x)
{
	int w = nodeType(x), y = f[x]; //-1
	pushDown(y); pushDown(x);
	setc(y, c[x][!w], w);
	if (root(y)) f[x] = f[y]; else setc(f[y], x, nodeType(y));
	setc(x, y, !w);
	update(y);
}
int tmp[N];
inline void splay (int x)
{
	for (pushDown(x); !root(x); zig(x)) if (!root(f[x]))
		if (nodeType(x) == nodeType(f[x])) zig(f[x]); else zig(x);
	update(x);
}

inline void access (int x)
{
	int _x(x);
	for (int y = nil; x; y = x, x = f[x])
		splay(x), setc(x, y, 1), update(x);
	splay(_x);
}

inline void change (int x, int y, int nkey)
{
	access(x);
	for (int z = nil; y; z = y, y = f[y])
	{
		splay(y);
		if (!f[y])
		{
			makesame(c[y][1], nkey);
			makesame(z, nkey);
			key[y] = nkey;
		}
		setc(y, z, 1);
		update(y);
	}
}

inline int query (int x, int y)
{
	int res(0);
	access(x);
	for (int z = nil; y; z = y, y = f[y])
	{
		splay(y);
		if (!f[y])
			res = max(max(lsum[c[y][1]], 0) + max(lsum[z], 0) + key[y],
				max(csum[c[y][1]], csum[z]));
		setc(y, z, 1);
		update(y);
	}
	return res;
}

int que[N], l, r;
inline void BFS ()
{
	l = r = 0;
	que[++r] = 1, f[1] = 0;
	while (l != r)
	{
		int x = que[++l], y;
		initNode(x);
		for (edge *i = e[x]; i; i = i->n)
			if ((y = i->t) != f[x]) f[que[++r] = y] = x;
	}
}

int main ()
{
	// init ...
	BFS();
	int m; read(m);
	while (m--)
	{
		// ...
		if (a == 1) { printf("%d\n", query(b, c)); }
		else { change(b, c, d); }
	}
	return 0;
}




