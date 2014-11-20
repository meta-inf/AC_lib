//@ Half-plane Intersection

/*
 * Last Edit: May 2011
 * I don't know why it works either.
 * */

#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

namespace HPI {

#define MXN 20050

const double eps = 1e-7;

inline int sgn(double a) {
	if (fabs(a) < eps) return 0;
	return (a < 0)? -1 : 1;
}

#define ZERO(X) (sgn(X)==0)
#define NEGA(X) (sgn(X)==-1)
#define POSI(X) (sgn(X)==1)

struct point {
	double x, y;
	point(): x(0), y(0) {}
	point(double a, double b): x(a), y(b) {}
	point operator-(const point &b) const {
		return point(x - b.x, y - b.y);
	}
	double operator*(const point &b) const {
		return x * b.y - y * b.x;
	}
	bool operator==(const point &b) const {
		return (ZERO(y - b.y) && ZERO(x - b.x));
	}
} P[MXN];

struct hvec {
	point s, t;
	double ang;
	point operator*(const hvec &b) const {
		double s1 = (b.t - s) * (b.s - s),
			s2 = (b.s - t) * (b.t - t);
		return point((s.x * s2 + t.x * s1) / (s2 + s1),
			(s.y * s2 + t.y * s1) / (s2 + s1));
	}
	bool operator<(const hvec &b) const {
		if (ZERO(b.ang - ang)) 
			return sgn((b.t - b.s) * (t - b.s)) >= 0;
		else return ang < b.ang;
	}
} H[MXN], Q[MXN];
int N;

inline bool isout(const hvec &h, const point &p) {
	return POSI((p - h.s) * (h.t - h.s));
}
inline bool parralel(const hvec &a, const hvec &b) {
	return ZERO((a.t - a.s) * (b.t - b.s));
}

point inte(hvec a, hvec b) { return a * b; }
bool lesstahn(hvec a, hvec b) {return a < b; }

inline int hvec_int() {
	int M = 0, C = 1, l = 0, r = 1;
	sort(H + 1, H + N + 1);
	for (int i = 2; i <= N; ++i)
		if (! ZERO(H[i].ang - H[i - 1].ang))
			H[++C] = H[i];
	Q[0] = H[1], Q[1] = H[2];
	for (int i = 3; i <= C; ++i) {
		while (l < r && isout(H[i], Q[r] * Q[r - 1])) --r;
		while (l < r && isout(H[i], Q[l] * Q[l + 1])) ++l;
		Q[++r] = H[i];
	}
	while (l < r && isout(Q[l], Q[r] * Q[r - 1])) --r;
	while (l < r && isout(Q[r], Q[l] * Q[l + 1])) ++l;
	if (r <= l + 1) return 0;
	for (int i = l; i < r; ++i) P[++M] = Q[i] * Q[i + 1];
	P[++M] = Q[l] * Q[r];
	M = unique(P + 1, P + M + 1) - P - 1;
	return M;
}

double area(int n) {
	if (n < 3) return 0;
	double s = 0;
	for (int i = 1; i <= n; ++i)
		s += P[i] * P[i % n + 1];
	return fabs(s / 2);
}

void add_hvec(point s, point t) {
	H[++N].s = s, H[N].t = t;
	point c = t - s;
	H[N].ang = atan2(c.y, c.x);
}

int main() { 
	int m;
	N = 0;
	add_hvec(point(0, 0), point(10000, 0)),
	add_hvec(point(10000, 0), point(10000, 10000)),
	add_hvec(point(10000, 10000), point(0, 10000)),
	add_hvec(point(0, 10000), point(0, 0));
	scanf("%d", &m);
	for (int i = 1; i <= m; ++i) {
		double x1, y1, x2, y2;
		scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
		add_hvec(point(x1, y1), point(x2, y2));
	}
	m = hvec_int();
	for (int i = 1; i <= m; ++i)
		printf("%.3f %.3f\n", P[i].x, P[i].y);
	printf("%.1lf\n", area(m));
	return 0;
}

}

int main ()
{
#ifdef LOCAL
	freopen("in", "r", stdin);
#endif
	HPI::main();
}
