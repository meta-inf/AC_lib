//@ kd-Tree
/*
 * check http://en.wikipedia.org/wiki/K-d_tree
 * Orthogonal range search : O(N^{(d-1)/d})
 * Nearest neighbor query : O(N^{(d-1)/d})
 * */

const int K = 3, N = 100000;

struct point {
	long long dt[K], now;
	long long get (int d) { return dt[d]; }
} P[N];
bool bynow (point a, point b) { return a.now < b.now; }

int lmax[N], rmin[N];

void build (int t, int l, int r, int d)
{
//build the tree node of P[l .. r] in demension d
	if (l > r) return;
	int m = l + r >> 1;
	for (int i = l; i <= r; i++)
	{
		P[i].now = P[i].get(d);
		for (int j = 0; j < dmax; ++j) // for range query
			maxd[t][j] = max(maxd[t][j], P[i].get(j)),
			mind[t][j] = min(mind[t][j], P[i].get(j));
	}
	sort(P + l, P + r + 1, bynow); 
	lmax[t] = m != l ? P[m - 1].get(d) : -inf;
	rmin[t] = m != r ? P[m + 1].get(d) : inf;
	build(t * 2, l, m - 1, (d + 1) % K);
	build(t * 2 + 1, m + 1, r, (d + 1) % K);
}
void update (long long A)
{
	if (A == 0) return;
	if (A < ans) { ans = A; num = 1;}
	else if (A == ans) ++num;
}

// nearest neighbor query
void query (int t, int l, int r, point pt, int d)
{
	if (l > r) return;
	update(dis(pt, P[l + r >> 1]));
	if (l == r) return;
	int m = l + r >> 1;
	if (pt.get(d) <= lmax[t]) {
		query(t * 2, l, m - 1, pt, (d + 1) % K);
		if ((long long)(pt.get(d) - rmin[t]) * (pt.get(d) - rmin[t]) <= ans) // improvable
			query(t * 2 + 1, m + 1, r, pt, (d + 1) % K);
	}
	else {
		query(t * 2 + 1, m + 1, r, pt, (d + 1) % K);
		if ((long long)(pt.get(d) - lmax[t]) * (pt.get(d) - lmax[t]) <= ans)
			query(t * 2, l, m - 1, pt, (d + 1) % K);	
	}
}

// sketch of range query
void rquery (int t, int l, int r, int QR) 
{
	if (l > r) return;
	if (included(t, QR))
	{
		// do something 
		return;
	}
	if (disjoint(t, QR))
	{
		return;
	}
	int m = l + r >> 1;
	if (included(P[m], QR))
	{
		// do something
	}
	rquery(t * 2, l, m - 1, QR);
	rquery(t * 2 + 1, m + 1, r, QR);
}

