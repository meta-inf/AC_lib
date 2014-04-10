/*
 * dynamic convex hull (deletion not supported)
 * ~ <set>, <algorithm>
 * */

typedef long double LD;
typedef long long LL;

const LL inf = ~0ULL >> 2;
const int N = 200050;

inline LL sqr (LL x) { return x * x; }

const int POINT = 0, QUERY = 1;

struct point { LL x, y; };
inline LL operator^ (point a, point b) { return a.x * b.x + a.y * b.y; }
inline LD operator* (point a, point b) { return (LD)a.x * b.y - (LD)b.x * a.y; } // prevent overflow
inline point operator- (point a, point b) { return (point){a.x - b.x, a.y - b.y}; }
struct snode { point dt; const snode *n; int ty; };

bool operator< (snode a, snode b)
{
	if (a.ty == POINT && b.ty == POINT)
		return a.dt.x < b.dt.x;
	bool rev = false, res;
	if (b.ty == QUERY) swap(a, b), rev = true;
	// return true if the next one is worse.
	// according to the defination of upper_bound, we should stop in that case.
	if (! b.n) res = true;
	else
	{
		LL va = a.dt ^ b.dt, vb = a.dt ^ b.n->dt;
		res = bool(va > vb);
	}
	return res ^ rev;
}
template <class T> T prev (T x) { return --x; }
template <class T> T succ (T x) { return ++x; }

/*
 * Maintain the lower convex envelope of the points inserted.
 * Query (point p) : return max(p.x * i.x + p.y * i.y : i in S)
 * Note that p.y is expected to be negative.
 * To query the minimum, in the < operator of snode change 
 * > res = bool(va > vb);
 * to
 * > res = bool(va < vb);
 * ; To get the upper convex envelope, negate the y coordinate of all points.
 */

struct point_set
{
	typedef set<snode>::iterator iter;
	set<snode> S;
	void printSet ()
	{
		for (iter it = S.begin(); it != S.end(); ++it)
			fprintf(stderr, "%llx (%lld %lld) %llx %d\n", reinterpret_cast<LL>(&*it),
					it->dt.x, it->dt.y, reinterpret_cast<LL>(it->n), it->ty);
	}
	void clear ()
	{
		S.clear();
	}
	void insert (point x)
	{
		snode sn = (snode){x, 0x0, POINT};
		iter it = S.find(sn);
		if (it != S.end())
		{
			if ((x.y - it->dt.y) >= 0) return;
			S.erase(it);
		}
		iter isuc = S.upper_bound(sn), ipre;
		if (isuc != S.begin() && isuc != S.end())
		{
			ipre = prev(isuc);
			if (((x - ipre->dt) * (isuc->dt - ipre->dt)) <= 0) return;
		}
		if (isuc != S.end()) // the right pointt
		{
			for (iter cr = isuc, nr = succ(cr); nr != S.end(); cr = nr, nr = succ(nr))
				if (((nr->dt - x) * (cr->dt - x)) < 0) break;
				else S.erase(cr);
		}
		isuc = S.upper_bound(sn);
		if (isuc != S.begin()) // the left pointt
		{
			ipre = prev(isuc);
			for (iter cr = ipre, nr = prev(cr); cr != S.begin(); cr = nr, nr = (nr == S.begin() ? nr : prev(nr)))
				if (((nr->dt - x) * (cr->dt - x)) > 0) break;
				else S.erase(cr);
		}
		// rebuild the bi-directional link list
		isuc = S.upper_bound(sn);
		sn.n = isuc == S.end() ? 0x0 : &*isuc;
		isuc = S.insert(sn).first;
		if (isuc != S.begin())
		{
			ipre = prev(isuc);
			snode *s = const_cast<snode*>(&*ipre);
			s->n = &*isuc;
		}
	}
	LL query (point qr)
	{
		iter it = S.upper_bound((snode){qr, 0x0, QUERY});
		return qr ^ it->dt;
	}
} s;


