//header files : cmath, algorithm, numeric, iostream[optional]
/*
 * Code fragments on planar geometry.
 * Not yet completed.
 * edit log
 * 3-18-2013 Covex Hull, point::rotate, crdcomp
 * Jan-      Point class, Half Plane Intersection (bruteforce approach)
 */
#define Px first
#define Py second

typedef long double LD;
const LD eps = 1e-10, infcmp = 1e200, infcrd = 1e9; // avoid possible errors, infcrd * eps < 1
template <class T> inline T abs (T x) { return x > 0 ? x : -x; }
template <class T> inline int sgn (T x) { return abs(x) < eps ? 0 : x > 0 ? 1 : -1; }
template <class T> inline T sqr (T x) { return x * x; }

struct point
{
	LD x, y;
	point (LD xx = 0, LD yy = 0) { x = xx, y = yy; }
	LD len () { return hypot(x, y); }
	LD angle () { return atan2(y, x); }
	point normalize () { LD l = len(); return point(x / l, y / l); }
	point oppo () { return point(y, -x); }
	point rotate (LD t) { // counterclockwise
		LD cost = cos(t), sint = sin(t);
		return point(x * cost - y * sint, x * sint + y * cost);
	}
};
inline int crdcomp (point s, point e) { int sx = sgn(s.x - e.x); return sx ? sx == -1 : sgn(s.y - e.y) == -1; }
inline point operator+ (const point &a, const point &b) { return point(a.x + b.x, a.y + b.y); }
inline point operator- (const point &a, const point &b) { return point(a.x - b.x, a.y - b.y); }
inline LD operator* (const point &a, const point &b) { return a.x * b.y - b.x * a.y; } // DASH
inline LD operator^ (const point &a, const point &b) { return a.x * b.x + a.y * b.y; } // DOT
inline point operator* (LD l, const point &a) { return point(a.x * l, a.y * l); }
inline point operator* (const point &a, LD l) { return point(a.x * l, a.y * l); }
inline point operator/ (const point &a, LD l) { return (LD(1) / l) * a; }
#ifdef _GLIBCXX_IOSTREAM
inline istream& operator>> (istream &in, point &a) { return in >> a.x >> a.y; }
inline ostream& operator<< (ostream &out, point &a) { return out << '(' << a.x << ',' << a.y << ')'; }
#endif
inline bool operator== (const point &a, const point &b) { return sgn(a.x - b.x) == 0 && sgn(a.y - b.y) == 0; }

inline bool onSeg (point a, point s, point e) { return sgn((e - a).len() + (a - s).len() - (e - s).len()) == 0; }

inline pair<int, point> getConj (point s1, point e1, point s2, point e2)
{
	LD a = (s2 - s1) * (e1 - s1), b = (e2 - s1) * (e1 - s1);
	if (sgn(a - b) == 0)
		return make_pair(0, point(0.0, 0.0));
	point r = (a * e2 - b * s2) / (a - b);
	if (!onSeg(r, s1, e1) || !onSeg(r, s2, e2)) return make_pair(-1, r); // intersection of the two lines.
	return make_pair(1, r); // intersection of the two segments.
}
inline int colinear (point s1, point e1, point s2, point e2)
{
	LD a = (s2 - s1) * (e1 - s1), b = (e2 - s1) * (e1 - s1);
	if (sgn(a - b) != 0 || sgn(a) != 0) return 0;
	if (onSeg(s2, s1, e1) && onSeg(e2, s1, e1)) return 1;
	if (onSeg(s2, s1, e1) || onSeg(e2, s1, e1)) return 2;
	return 3;
}
inline int getCH (point P[], int n, point CH[]) { // 1-based, colinear points removed
	sort(P + 1, P + 1 + n, crdcomp);
	int m = 0; CH[m = 1] = P[1];
	for (int i = 1; i <= n; ++i) {
		while (m > 1 && sgn((P[i] - CH[m]) * (CH[m] - CH[m - 1])) <= 0) --m;
		CH[++m] = P[i];
	}
	int cm = m;
	for (int i = n - 1; i >= 1; --i) {
		while (m > cm && sgn((P[i] - CH[m]) * (CH[m] - CH[m - 1])) <= 0) --m;
		CH[++m] = P[i];
	}
	return m - 1;
}
inline LD calcArea (point *s, point *e) // [s, e), sorted, position e accessable
{
	if (e - s < 3) return 0;
	*e = *s; LD res(0);
	for (point *i = s; i < e; ++i) res += i[0] * i[1];
	return abs(res) / 2.0;
}


// Half Plane Intersection.
struct hplane {
	point s, e; // feasible region = {P|(P-s)*(e-s)>=0} (sP lies on the right hand side of se)
	hplane (point ss, point ee) : s(ss), e(ee) {}
	bool inside (point p) { return sgn((p - s) * (e - s)) >= 0; }
};

struct HPI_t
{
	static const int N = 1024;
	point P1[N], P2[N];
	int n, m, inside[N];
	point* begin() { return P1 + 1; }
	point* end() { return P1 + n + 1; }
	void clear () {
		n = 4; m = 0;
		P1[1] = point(infcrd, infcrd);
		P1[2] = point(-infcrd, infcrd);
		P1[3] = point(-infcrd, -infcrd);
		P1[4] = point(infcrd, -infcrd);
	}
	bool extend (hplane now)
	{
		P1[n + 1] = P1[1]; m = 0;
		for (int i = 1; i <= n + 1; ++i) inside[i] = now.inside(P1[i]);
		for (int i = 1; i <= n; ++i)
		{
			if (inside[i]) P2[++m] = P1[i];
			if (inside[i] ^ inside[i + 1]) P2[++m] = getConj(now.s, now.e, P1[i], P1[i + 1]).Py;
		}
		copy(P2 + 1, P2 + 1 + (n = m), P1 + 1);
		return n;
	}
};
