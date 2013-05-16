//kd-tree by kAc
//see http://en.wikipedia.org/wiki/K-d_tree
//Last Edit : Before 2013
const int K = 3, N = 100000;

struct Tpoint {
	long long dt[K], now;
	long long get (int d) { return dt[d]; }
} p[N];
bool bynow (Tpoint a, Tpoint b) { return a.now < b.now; }

int lmax[N], rmin[N];

void build(int idx, int l, int r, int d)
{
//build the tree node of p[l .. r] in demension d
	if (l > r) return;
	int m = l + r >> 1;
	for (int i = l; i <= r; i++) p[i].now = p[i].get(d);
	sort(p + l, p + r + 1, bynow); 
	lmax[idx] = m != l ? p[m - 1].get(d) : 0xe0e0e0e0;
	rmin[idx] = m != r ? p[m + 1].get(d) : 0x3f3f3f3f;
	build(idx * 2, l, m - 1, (d + 1) % K);
	build(idx * 2 + 1, m + 1, r, (d + 1) % K);
}
void renew(long long A)
{
	if (A == 0) return;
	if (A < ans) { ans = A; num = 1;}
	else if (A == ans) ++num;
}
void query(int idx, int l, int r, Tpoint pt, int d)
{
// queries min(dis(p[i],dt)) in O(k * n ^ (1 - 1/k)) time
	if (l > r) return;
	renew(dis(pt, p[l + r >> 1]));
	if (l == r) return;
	int m = l + r >> 1;
	if (pt.get(d) <= lmax[idx]) {
		query(idx * 2, l, m - 1, pt, (d + 1) % K);
		if ((long long)(pt.get(d) - rmin[idx]) * (pt.get(d) - rmin[idx]) <= ans)
			// It is possible to use the best crds of other dimension to improve the estimation
			query(idx * 2 + 1, m + 1, r, pt, (d + 1) % K);
	}
	else {
		query(idx * 2 + 1, m + 1, r, pt, (d + 1) % K);
		if ((long long)(pt.get(d) - lmax[idx]) * (pt.get(d) - lmax[idx]) <= ans)
			query(idx * 2, l, m - 1, pt, (d + 1) % K);	
	}
		
}
