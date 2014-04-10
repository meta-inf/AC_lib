//cmath, algorithm, numeric, iostream[optional]
/*
  5-30-2013 3D point class
  3-18-2013 Covex Hull, point::rotate, crdcomp
  Jan-      Point class, Half Plane Intersection(Bruteforce)
 */


#define Px first
#define Py second

// {{{ FP functions & constants .. 
typedef long double LD;
const LD eps = 1e-10, infcmp = 1e200, infcrd = 1e9, pi = acos(-1);
// avoid possible errors, infcrd * eps < 1
template <class T> inline T abs (T x) { return x > 0 ? x : -x; }
template <class T> inline int sgn (T x) { return abs(x) < eps ? 0 : x > 0 ? 1 : -1; }
template <class T> inline T sqr (T x) { return x * x; }
template <class T> inline T normalize1 (T x) { // [0, 2pi)
	while (sgn(x - pi * 2) >= 0) x -= 2 * pi;
	while (sgn(x) < 0) x += 2 * pi;
	return x;
}
template <class T> inline T normalize2 (T x) { // (-pi,pi]
	while (sgn(-pi - x) >= 0) x += 2 * pi;
	while (sgn(x - pi) > 0) x -= 2 * pi;
	return x;
}
// }}}

// {{{ 2D Point .. 
struct point
{
	LD x, y;
	point (LD xx = 0, LD yy = 0) { x = xx, y = yy; }
	LD len () const { return hypot(x, y); }
	point oppo () const { return point(y, -x); }
	point rotate (LD t) const { // counterclockwise
		LD cost = cos(t), sint = sin(t);
		return point(x * cost - y * sint, x * sint + y * cost);
	}
};
inline int crdcomp (point s, point e) { int sx = sgn(s.x - e.x); return sx ? sx == -1 : sgn(s.y - e.y) == -1; }
inline point operator+ (const point &a, const point &b) { return point(a.x + b.x, a.y + b.y); }
inline point operator- (const point &a, const point &b) { return point(a.x - b.x, a.y - b.y); }
inline LD operator* (const point &a, const point &b) { return a.x * b.y - b.x * a.y; } // det
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
// }}}

// {{{ 2D Polygon Functions .. 
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

// }}} 

// {{{ 3D Point .. 
struct point3 {
	LD x, y, z;
	point3 () { x = y = z = 0; }
	point3 (LD _x, LD _y, LD _z) { x = _x, y = _y, z = _z; }
	LD len () const { return sqrt(len2()); }
	LD len2 () const { return sqr(x) + sqr(y) + sqr(z); }
	point3 normalize () const { LD l = len(); return point3(x / l, y / l, z / l); }
};

inline bool operator< (const point3 &a, const point3 &b)
{
	if (sgn(a.x - b.x)) return sgn(a.x - b.x) < 0;
	if (sgn(a.y - b.y)) return sgn(a.y - b.y) < 0;
	return sgn(a.z - b.z) < 0;
}
inline bool operator== (const point3 &a, const point3 &b) {
	return !sgn(a.x - b.x) && !sgn(a.y - b.y) && !sgn(a.z - b.z);
}

#ifdef _GLIBCXX_IOSTREAM
inline istream& operator>> (istream &in, point3 &a) { return in >> a.x >> a.y >> a.z; }
inline ostream& operator<< (ostream &out, const point3 &a) {
	return out << "(" << a.x << ", " << a.y << ", " << a.z << ")";
}
#endif
inline LD operator^ (const point3 &a, const point3 &b) { // dot
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline point3 operator* (const point3 &a, const point3 &b) { // cross
	return point3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
inline point3 operator+ (const point3 &a, const point3 &b) {
	return point3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline point3 operator- (const point3 &a, const point3 &b) {
	return point3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline point3 operator* (const point3 &a, const LD b) {
	return point3(a.x * b, a.y * b, a.z * b);
}
inline point3 operator* (const LD b, const point3 &a) {
	return point3(a.x * b, a.y * b, a.z * b);
}
inline point3 operator/ (const point3 &a, const LD b) { return a * (1.0 / b); }

inline LD angle (const point3 &a, const point3 &b) { // [0, pi]
	return acos((a ^ b) / (a.len() * b.len()));
}
inline pair<int, point3> intersection (
		const point3 &s1, const point3 &e1, 
		const point3 &s2, const point3 &e2)
{
	if (sgn(((e1 - s1) * (e2 - s2)).len()) == 0) // parallel
		return make_pair(0, point3());

	if (sgn((e1 - s1) ^ ((e2 - s1) * (s2 - s1)))) // non-coplanar
		return make_pair(-2, point3());

	point3 pa = (s2 - s1) * (e1 - s1), pb = (e2 - s1) * (e1 - s1);
	LD a = pa.len(), b = pb.len();
	if (sgn(pa ^ pb) < 0) b = -b;
	point3 r = (a * e2 - b * s2) / (a - b);
//	assert(sgn(((s1 - r) * (e1 - r)).len()) == 0);
//	assert(sgn(((s2 - r) * (e2 - r)).len()) == 0);
	return make_pair(1, r); // intersect
}

struct plane {
	point3 A, B, C, L;
	LD lenB, lenC;
	// three points A, B, C or {P|(P-A)*L=0}
	plane () {}
	plane (const point3 &a, const point3 &b, const point3 &c) {
		A = a; B = b; C = c;
		C = (B - A) * (C - A) + A;
		C = (B - A) * (C - A) * ((B - A).len2() / (C - A).len2()) + A;
//		exception may raised by fp error
//		assert(sgn((B - A) ^ (C - A)) == 0);
//		assert(sgn(((c - a) * (b - a)) ^ (C - A)) == 0);
		L = ((C - A) * (B - A)).normalize();
		lenB = (B - A).len(), lenC = (C - A).len();
	}
	static plane normalPlane (const point3 &s, const point3 &e) {
		// possible fp error
		point3 x(rand() % 100, rand() % 100, rand() % 100),
			   y(rand() % 100, rand() % 100, rand() % 100);
		return plane(s, s + (e - s) * x * 0.01, s + (e - s) * y * 0.01);
	}
	LD dis (const point3 &p) const { return (p - A) ^ L; } // may return negative value
	point projection (const point3 &p) const { // relative coordinate
		return point((p - A) ^ (B - A) / lenB, (p - A) ^ (C - A) / lenC);
	}
	pair<int, point3> intersection (const point3 &s, const point3 &e) const {
	// line-plane intersection. 0 - parallel, 1 - intersect
		LD a = dis(s), b = dis(e);
		if (sgn(a - b) == 0)
			return make_pair(0, point3());
		return make_pair(1, (s * -b + e * a) / (a - b));
	}
	LD lAngle (const point3 &s, const point3 &e) const { // line-plane angle, [0, pi/2]
		pair<int, point3> ret = intersection(s, e);
		if (ret.Px == 0)
			return 0;
		LD t = angle(e - ret.Py, L);
		return pi / 2 - min(t, pi - t);
	}
};

// }}}


