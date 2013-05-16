const int N = 2048, C = 0, M = 1;

typedef int yTree[N][2];
yTree xC[N], xM[N];
int mx, my;

#define L(X) (((X)<<1)+1)
#define R(X) (((X)<<1)+2)
inline void update (int &x, int y) { if (y > x) x = y; }

int queryY (int x, int l, int r, int ql, int qr, yTree base)
{
	if (ql <= l && qr >= r) return base[x][M];
	int re = base[x][C], m = (l + r) >> 1;
	if (ql <= m) update(re, queryY(L(x), l, m, ql, qr, base));
	if (qr > m) update(re, queryY(R(x), m + 1, r, ql, qr, base));
	return re;
}
void coverY (int x, int l, int r, int ql, int qr, int nv, yTree base)
{
	update(base[x][M], nv);
	if (ql <= l && qr >= r) update(base[x][C], nv);
	else 
	{
		int m = (l + r) >> 1;
		if (ql <= m) coverY(L(x), l, m, ql, qr, nv, base);
		if (qr > m) coverY(R(x), m + 1, r, ql, qr, nv, base);
	}
}

int queryX (int x, int l, int r, int f, int b, int ql, int qr)
{
	if (ql <= l && qr >= r) return queryY(0, 1, my, f, b, xM[x]);
	int m = (l + r) >> 1, re = queryY(0, 1, my, f, b, xC[x]);
	if (ql <= m) update(re, queryX(L(x), l, m, f, b, ql, qr));
	if (qr > m) update(re, queryX(R(x), m + 1, r, f, b, ql, qr));
	return re;
}
void coverX (int x, int l, int r, int f, int b, int ql, int qr, int nv)
{
	coverY(0, 1, my, f, b, nv, xM[x]);
	if (ql <= l && qr >= r) coverY(0, 1, my, f, b, nv, xC[x]);
	else
	{
		int m = (l + r) >> 1;
		if (ql <= m) coverX(L(x), l, m, f, b, ql, qr, nv);
		if (qr > m) coverX(R(x), m + 1, r, f, b, ql, qr, nv);
	}
}

