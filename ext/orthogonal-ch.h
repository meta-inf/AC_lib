/*
 * Calculate orthogonal convex hull in sublinear time.
 * */

#include <cstdio>
#include <utility>
#include <algorithm>
using namespace std;

typedef long long LL;

const int N = 100050;
const LL inf = ~0ull >> 3;

struct point { LL x, y; } P[N], OCH[N * 2];

bool byCrd (const point &a, const point &b) { return a.y == b.y ? a.x < b.x : a.y < b.y; }

int sel[N];
int n, m;

void solve ()
{
	sort(P + 1, P + n + 1, byCrd);
	int xmin(P[1].x), xmax(P[1].x); sel[1] |= 1;
	P[0].y = P[n + 1].y = inf;
	for (int i = 1, j = 1; i <= n; i = j)
	{
		for (; P[j].y == P[i].y; ++j) ;
		if (P[i].x < xmin) sel[i] |= 1, xmin = P[i].x;
		if (P[j - 1].x > xmax) sel[j - 1] |= 2, xmax = P[j - 1].x;
	}
	xmin = P[n].x, xmax = P[n].x; sel[n] |= 2;
	for (int i = n, j = n; i >= 1; i = j)
	{
		for (; P[j].y == P[i].y; --j) ;
		if (P[j + 1].x < xmin) sel[j + 1] |= 1, xmin = P[j + 1].x;
		if (P[i].x > xmax) sel[i] |= 2, xmax = P[i].x;
	}
	m = 0;
	OCH[++m] = P[1];
	for (int i = 2; i <= n; ++i)
		if ((sel[i] & 2) == 2) OCH[++m] = P[i];
	for (int i = n - 1; i >= 1; --i)
		if ((sel[i] & 1) == 1) OCH[++m] = P[i];
	LL perimetre(0), area(0);
	for (int i = 1; i < m; ++i)
		perimetre += abs(OCH[i + 1].y - OCH[i].y) + abs(OCH[i + 1].x - OCH[i].x);
	for (int i = 1; i < m; ++i)
		area += OCH[i].x * OCH[i + 1].y - OCH[i + 1].x * OCH[i].y;
	area = abs(area);
	for (int i = 1; i < m; ++i)
		area -= abs((OCH[i + 1].y - OCH[i].y) * (OCH[i + 1].x - OCH[i].x));
	printf("%lld\n%lld\n", perimetre, area / 2);
}

int main ()
{
#ifdef LOCAL
	freopen("in", "r", stdin);
#endif
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) scanf("%lld%lld", &P[i].x, &P[i].y);
	solve();
	return 0;
}

