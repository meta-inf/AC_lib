//SPOJ CONANGSS
//0-Based
#include <cstdio>
#include <cmath>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <functional>
using namespace std;

typedef long long lint;
const int N = 50050, MBS = 500;
const lint inf = ~0ull >> 4;

struct point { lint x, y; };
lint operator* (const point &a, const point &b) {
	return a.x * b.y - b.x * a.y;
}
point operator- (const point &a, const point &b) {
	return (point){a.x - b.x, a.y - b.y};
}
bool operator< (const point &a, const point &b) {
	return a.x == b.x ? a.y < b.y : a.x < b.x;
}

template <typename T, int size>
struct static_allocator {
	T buff[size << 2], *cur;
	static_allocator () { cur = buff; }
	inline T* getpos () { return cur; }
	inline void setpos (T* _cur) { cur = _cur; }
};
static_allocator<point, N> palloc;
static_allocator<lint, N> ialloc;

struct base_block {
	lint *sum_s, *sum_e, delta;
	point *ch_s, *ch_e;
	void update () // O(size)
	{
		lint *sc = sum_s;
		static point tmp[N], *pe;
		for (pe = tmp; sc != sum_e; ++sc) *(pe++) = (point){pe - tmp, *sc};
		point *cp = ch_s, *pcur = tmp;
		sort(pcur, pe);
		*(cp++) = *(pcur++); *(cp++) = *(pcur++);
		for (; pcur < pe; )
		{
			while (cp > ch_s + 1 && (pcur[0] - cp[-1]) * (cp[-1] - cp[-2]) <= 0) --cp;
			*(cp++) = *(pcur++);
		}
		ch_e = cp;
	}
	void bind (lint *arr, int size) // O(size)
	{
		sum_s = ialloc.getpos(); ialloc.setpos(sum_e = sum_s + size);
		ch_s = palloc.getpos(); palloc.setpos(ch_e = ch_s + size);
		sum_s[0] = arr[0];
		for (int i = 1; i < size; ++i) sum_s[i] = sum_s[i - 1] + arr[i];
		update();
	}
	inline void push (int do_update = 1) { // O(size)
		if (delta) {
			int i(0);
			for (lint *it = sum_s; it != sum_e; ++it) *it += delta * ++i;
			delta = 0; if (do_update) update();
		}
	}
	void inc (int l, int r, lint de) { // O(size)
		push(0);
		for (int i = l; i <= r; ++i) sum_s[i] += de * (i - l + 1);
		for (int i = r + 1; i <= sum_e - sum_s - 1; ++i) sum_s[i] += de * (r - l + 1);
		update();
	}
	inline void inc (lint de) { delta += de; } // O(1)
	lint _res; int _pos;
	lint _eval (int i) { return ch_s[i].y + delta * (ch_s[i].x + 1); } // O(1)
	void _lmax (int l, int r) // O(lg_size)
	{
		if (r - l < 4) for (; l <= r; ++l)
			if (_eval(l) > _res) _res = _eval(_pos = l); else;
		else {
			int m = (l + r) >> 1, mr = (m + r) >> 1;
			lint cm = _eval(m), cmr = _eval(mr);
			if (cm > cmr) _lmax(l, mr); else _lmax(m, r);
		}
	}
	inline void getlMax () { // O(lg_size)
		_res = -inf; _pos = -1;
		_lmax(0, ch_e - ch_s - 1);
	}
	inline lint sum (int l, int r) { // O(1)
		if (l) return sum_s[r] - sum_s[l - 1] + delta * (r - l + 1);
		else return sum_s[r] + delta * (r + 1);
	}
	inline lint getlMax (int le) { // O(size)
		lint res(-inf);
		for (int i = 0; i <= le; ++i) res = max(res, sum_s[i] + delta * (i + 1));
		return res;
	}
	inline void print ()
	{
		fprintf(stderr, "base_block %llx\nSum = ", reinterpret_cast<unsigned long long>(this));
		for (lint *i = sum_s; i < sum_e; ++i)
			fprintf(stderr, "%lld ", *i);
		fprintf(stderr, "\nCH = ");
		for (point *i = ch_s; i < ch_e; ++i)
			fprintf(stderr, "(%lld, %lld) ", i->x, i->y);
		getlMax();
		fprintf(stderr, "\nLMAX %lld@%d\n", _res, _pos);
	}
};

struct block {
	int s, size;
	base_block nor;
	inline void inc (lint de) { nor.inc(de); } // O(1)
	inline void inc (int l, int r, lint de) // O(size)
	{
		assert(l >= 0 && l <= r && r < size);
		nor.inc(l, r, de);
	}
	inline void print ()
	{
		fprintf(stderr, "Bblock %llx\nSum = ", reinterpret_cast<unsigned long long>(this));
		nor.print();
		fprintf(stderr, "L %lld\n", lMax());
	}

	inline void bind (lint *arr, int _s, int _size) // O(size)
	{
		static lint tmp[N];
		s = _s; size = _size;
		nor.bind(arr, size);
		//print();
	}
	inline lint lMax () { nor.getlMax(); return nor._res; } // O(lg_size)
	inline lint getMax (int l, int r) // O(size)
	{
		assert(l >= 0 && l <= r && r < size);
		lint res = -inf;
		for (int i = l; i <= r; ++i) res = max(res, nor.sum_s[i] + nor.delta * (i + 1));
		return res;
	}
	inline lint lMax (int l) { assert(l >= 0 && l < size); return nor.getlMax(l); } // O(size)
	inline lint sum () { return nor.sum_s[size - 1] + nor.delta * size; } // O(1)
} B[MBS];

lint dt[N];
int n, bcnt;

block* getBlock (int x) {
	for (int i = 0; i < bcnt; ++i) if (x >= B[i].s && x < B[i].s + B[i].size)
		return B + i;
}

lint query (int l, int r)
{
	static int n_query = 0;
	++ n_query;
	block *lb = getBlock(l), *rb = getBlock(r);
	//UNTITLE1
	lint base(0), res;
	for (block *t = B; t != lb; ++t) base += t->sum();
	if (lb == rb) return base + lb->getMax(l - lb->s, r - lb->s);
	res = base + lb->getMax(l - lb->s, lb->size - 1);
	for (block *t = lb + 1; t != rb; ++t)
		base += t[-1].sum(), res = max(res, base + t->lMax());
	base += rb[-1].sum(), res = max(res, base + rb->getMax(0, r - rb->s));
	return res;
}
void inc (int l, int r, lint de)
{
	block *lb = getBlock(l), *rb = getBlock(r);
	if (lb == rb) lb->inc(l - lb->s, r - lb->s, de);
	else
	{
		lb->inc(l - lb->s, lb->size - 1, de); ++lb;
		for (; lb != rb; ++lb) lb->inc(de);
		rb->inc(0, r - rb->s, de);
	}
}

int main ()
{
	freopen("in", "r", stdin);
	scanf("%d", &n);
	int bsz(0);
#ifdef LOCAL_DEBUG
	bsz = 5;
#else
	for (; bsz * bsz <= n; ++bsz) ;
#endif
	for (int i = 0; i < n; ++i) scanf("%lld", dt + i);
	lint *cur = dt;
	for (bcnt = 0; cur < dt + n; cur += bsz, bcnt ++)
		B[bcnt].bind(cur, cur - dt, bsz);
	int q; scanf("%d", &q);
	while (q--)
	{
		char op[20];
		int x, y; lint delt;
		scanf("%s", op);
		scanf("%d%d", &x, &y);
		x = max(x - 1, 0);
		y = min(y - 1, n - 1);
		if (op[0] == '0')
		{
			scanf("%lld", &delt);
			inc(x, y, delt);
		}
		else printf("%lld\n", query(x, y));
	}
	return 0;
}

