//# Block List
/*
 * Example: NOI2005 Sequence
 * */

#include <cstdio>
#include <cstring>
#include <climits>
#include <cmath>
#include <algorithm>
using namespace std;

#define BSZ 900
struct block;
typedef block *p_seg;
struct block {
	p_seg pre, next;
	int dt[BSZ * 2], mxsum[2], nmax, size, sum;
	bool flip, same; int val;
	inline void reverse(); //reverse dt[l .. r]
	inline void split(int); //split the block into two : 1 .. p && p + 1 .. size
	inline void merge(); //merge this && this->next
	inline void set_flag(); //set same and flip
	inline void update(); //calculate mxsum[], nmax
	inline void make_same(int);
	inline int max_sum();
	inline int right_max();
	inline int left_max();
	inline int gsum(int, int); //return sum(l, r)
	block(int); //default construct funciton
	block(int*, int, int); //construct a block from a[l .. r]
} *nil_T= new block(0);
int Q, CT = 0; //for convenience while debugging

inline void link(block *a, block *b) { //a->(a->next) --> a->b->(a->next)
	b->next = a->next, b->pre = a;
	if (a->next != nil_T) a->next->pre = b;
	a->next = b;
}

inline void remove_block(block *a) {
	if (a->pre != nil_T) a->pre->next = a->next;
	if (a->next != nil_T) a->next->pre = a->pre;
	delete a;
}

inline void block::reverse() {
	for (int _l = 1, _r = size; _l < _r; ++_l, --_r)
		swap(dt[_l], dt[_r]);
}

inline void block::set_flag() {
	if (same) {
		same = flip = false;
		for (int i = 1; i <= size; ++i) dt[i] = val;
	}
	if (flip) 
	{ flip = false; reverse(); }
}

inline void block::update() {
	int cur = 0;
	mxsum[0] = mxsum[1] = nmax = -INT_MAX;
	sum = 0;
	for (int i = 1; i <= size; ++i) {
		sum += dt[i]; 
		mxsum[0] = max(mxsum[0], sum);
		cur += dt[i];
		nmax = max(nmax, cur);
		cur = max(cur, 0);
	}
	cur = 0;
	for (int i = size; i > 0; --i) {
		cur += dt[i];
		mxsum[1] = max(mxsum[1], cur);
	}
}

inline void block::merge() {
	block *b = next;
	set_flag(); b->set_flag();
	memcpy(dt + size + 1, b->dt + 1, sizeof(int) * b->size);
	size += b->size; update();
	remove_block(b);
}

inline void block::split(int p) {
	set_flag();
	block *b = new block(dt, p + 1, size);
	link(this, b); size -= b->size; update();
}

inline void block::make_same(int nval) { same = true, val = nval; }

inline int block::max_sum() {
	if (same) {
		if (val > 0) return val * size;
		else return val;
	}
	return nmax;
}

inline int block::right_max() {
	if (same) {
		if (val > 0) return val * size;
		else return val;
	}
	return mxsum[1 ^ flip];
}

inline int block::left_max() {
	if (same) {
		if (val > 0) return val * size;
		else return val;
	}
	return mxsum[flip];
}

inline int block::gsum(int l, int r) {
	if (same) return (r - l + 1) * val;
	if (l <= 1 && r >= size) return sum;
	int res = 0;
	if (flip)
		for (int i = l; i <= r; ++i) res += dt[size - i + 1];
	else
		for (int i = l; i <= r; ++i) res += dt[i];
	return res;
}

block::block(int s): same(false), flip(false), sum(0), size(s), next(nil_T), pre(nil_T) {}

block::block(int *a, int l, int r): same(false), flip(false), next(nil_T), pre(nil_T) {
	memcpy(dt + 1, a + l, sizeof(int) * (r - l + 1));
	size = r - l + 1; update(); 
}

inline pair<p_seg, p_seg> build(int *A, int n, int rsize) {
	p_seg c, a, b;
	if (n % rsize > 0)
		a = new block(A, 1, n % rsize);
	else a = new block(A, 1, rsize);
	c = a;
	for (int i = a->size + 1; i <= n; i += rsize) {
		b = new block(A, i, i + rsize - 1);
		link(a, b); a = b;
	}
	return pair<p_seg, p_seg>(c, a);
}

struct block_list {
	p_seg H;
	int rsize; // recommended block size
	inline pair<p_seg, int> find(int);
	inline void remove(int, int);
	inline void insert(int, int*, int);
	inline long long query_sum(int, int);
	inline void make_same(int, int, int);
	inline void reverse(int, int);
	inline long long query_max_sum();
	block_list(int*, int);
#ifdef SPOJ
	~block_list();
#endif
};

block_list::block_list(int *A, int n) { 
	pair<p_seg, p_seg> X =build(A, n, rsize = BSZ);
	p_seg T, p1 = X.first, p2 = X.second;
	H = new block(1); H->dt[1] = -0x3f3f3f3f; H->next = p1, p1->pre = H;
	T = new block(1); T->dt[1] = -0x3f3f3f3f; p2->next = T, T->pre = p2;
}

#ifdef SPOJ
block_list::~block_list() {
	for (p_seg p1 = H; p1 != nil_T;) {
		p1 = p1->next;
		delete p1->pre;
	}
}
#endif

#define NEXT(S,P) (S+=P->size,P=P->next)
inline pair<p_seg, int> block_list::find(int x) {
	p_seg b; int s = 0;
	for (b = H->next; s + b->size < x; NEXT(s, b)) ;
	return pair<p_seg, int>(b, s);
}

inline void block_list::remove(int l, int r) {
	pair<p_seg, int> X = find(l), Y = find(r);
	p_seg p1 = X.first, p2 = Y.first;
	int s1 = X.second, s2 = Y.second;
	if (r - s2 != p2->size) p2->split(r - s2);
	if (l - s1 > 1) { p1->split(l - s1 - 1); NEXT(s1, p1); }
	while (s1 < r) { 
		NEXT(s1, p1); remove_block(p1->pre); 
	}
}

inline void block_list::insert(int x, int *A, int n) {
	pair<p_seg, int> X = find(x);
	pair<p_seg, p_seg> Y = build(A, n, rsize);
	p_seg p = X.first, l = Y.first, r = Y.second;
	int i = x - X.second;
	if (i) p->split(i);
	else p = p->pre;
	r->next = p->next;
	if (p->next != nil_T) 
		p->next->pre = r; 
	p->next = l; l->pre = p;
}

p_seg stack[BSZ];
inline void block_list::reverse(int l, int r) {
	if (l == r) return;
	pair<p_seg, int> X = find(l), Y = find(r);
	p_seg t, p1 = X.first, p2 = Y.first, a1, a2; 
	int s1 = X.second, s2 = Y.second, stop = 0;
	bool sb = (p1 == p2);
	if (r - s2 != p2->size) p2->split(r - s2);
	if (l - s1 > 1) { p1->split(l - s1 - 1); NEXT(s1, p1); }
	if (sb) {
		p1->flip ^= 1;
		return;
	}
	a1 = p1->pre, a2 = p2->next;
	for (p_seg i = p1; i != a2; i = i->next) 
		stack[++stop] = i;
	while (stop--) {
		t = stack[stop + 1]; t->flip ^= 1;
		a1->next = t, t->pre = a1;
		a1 = t;
	}
	a1->next = a2, a2->pre = a1;
}

inline long long block_list::query_sum(int l, int r) {
	pair<p_seg, int> X = find(l);
	p_seg p = X.first; int s = X.second;
	/* The whole interval queried is inside the block */
	if (r - s <= p->size) return p->gsum(l - s, r - s); 
	long long res = p->gsum(l - s, p->size);
	for (s += p->size, p = p->next; s + p->size < r; s += p->size, p = p->next)
		res += p->gsum(1, p->size); //use this to deal with same or flipped blocks
	if (s < r) res += p->gsum(1, r - s);
	return res;
}

inline void block_list::make_same(int l, int r, int val) {
	pair<p_seg, int> X = find(l), Y = find(r);
	p_seg p1 = X.first, p2 = Y.first;
	int s1 = X.second, s2 = Y.second;
	if (r - s2 != p2->size) p2->split(r - s2);
	if (l - s1 > 1) { p1->split(l - s1 - 1); NEXT(s1, p1); }
	for (; s1 < r; NEXT(s1, p1)) p1->make_same(val);
}

inline long long block_list::query_max_sum() {
	if (H->next->size == 0) return 0;
	long long res = -INT_MAX, cur = 0;
	for (p_seg p1 = H->next; p1->next != nil_T; p1 = p1->next)
		if (p1->size > 0) {
			res = max(res, (long long)(cur + p1->left_max()));
			res = max(res, (long long)(p1->max_sum()));
			cur = max(cur + p1->gsum(1, p1->size), (long long)(p1->right_max()));
		}
	return res;
} 

inline void read(int &x) {
	char ch; bool f = false;
	while (ch = getchar(), (ch != '-') && (ch < '0' || ch > '9'));
	if (ch == '-') x = 0, f = true;
	else x = ch - '0';
	while (ch = getchar(), ch >= '0' && ch <= '9') x = x * 10 + ch - '0'; 
	if (f) x *= -1;
}

int tsz, A[1000000];
#define skip(X) for(tm=1;tm<=X;++tm)getchar()
int main() { 
	int n, b, c, tm;
	/*freopen("sequence.in", "r", stdin);
	freopen("sequence.out", "w", stdout);*/
	scanf("%d%d", &n, &Q); tsz = n;
	for (int i = 1; i <= n; ++i) scanf("%d", A + i);
	block_list *B = new block_list(A, n);
	while (Q--) {
		char ch1; 
		scanf("\n%c", &ch1);
		switch(ch1) {
			case 'I': 
				skip(5); read(b), read(c); tsz += c;
				for (int i = 1; i <= c; ++i) read(A[i]);
				B->insert(b, A, c);
				break;
			case 'D': 
				skip(5); read(b), read(c); tsz -= c;
				B->remove(b, b + c - 1);
				break;
			case 'M': 
				getchar();
				if (getchar() == 'K') { //MAKE-SAME
					skip(6); read(b), read(c), read(tm);
					B->make_same(b, b + c - 1, tm);
				} else {
					skip(4); 
					printf("%lld\n", B->query_max_sum());
				}
				break;
			case 'R': 
				skip(6); read(b), read(c);
				B->reverse(b, b + c - 1);
				break;
			case 'G':
				skip(6); read(b), read(c);
				printf("%lld\n", B->query_sum(b, b + c - 1));
				break;
		}
	}
	return 0;
}

