//@ Link Cut Tree
/*
 * Suggested version.
 * */

struct node {
	static node *nil;
	node *f, *c[2];
	LL w, delt, maxw;
	int rev, cnt;
	inline void update () {
		assert(!delt);
		cnt = c[0]->cnt + c[1]->cnt + 1;
		maxw = max(max(c[0]->maxw, c[1]->maxw), w);
	}
	inline void inc (LL d) {
		if (this == nil) return;
		delt += d; maxw += d; w += d;
	}
	inline void flipRev () {
		if (this == nil) return;
		rev ^= 1; swap(c[0], c[1]);
	}
	inline void push () {
		if (rev) c[0]->flipRev(), c[1]->flipRev(), rev = 0;
		if (delt) c[0]->inc(delt), c[1]->inc(delt), delt = 0;
	}
	inline void sc (node *z, int w) { c[w] = z; z->f = this; }
	inline int ty () { return this == f->c[1]; }
	inline int rt () { return !(this == f->c[0] || this == f->c[1]); }
	inline void zig () {
		node *y = this->f; y->push(); push();
		int w = ty();
		f = y->f; if (!y->rt()) y->f->c[y->ty()] = this;
		y->sc(c[!w], w); sc(y, !w);
		y->update();
	}
	void init (LL _w) {
		c[0] = c[1] = f = nil;
		w = maxw = _w; cnt = 1; delt = 0; rev = 0;
	}
	static void init () {
		nil->f = nil->c[0] = nil->c[1] = nil;
		nil->w = nil->maxw = -inf;
	}
} T[N], *node::nil = &T[0];

inline void splay (node *x) { // zig only when reverse tag is added
	for (x->push(); !x->rt(); x->zig()) ; x->update();
}
inline void access (node *x) {
	for (node *y(x), *z(node::nil); y != node::nil; z = y, y = y->f)
		splay(y), y->sc(z, 1), y->update();
}
inline void increase (node *a, node *b, LL d) {
	access(a);
	for (node *y(b), *z(node::nil); y != node::nil; z = y, y = y->f) {
		splay(y);
		if (y->f == node::nil) {
			y->w += d; y->update();
			y->c[1]->inc(d); z->inc(d);
		}
		y->sc(z, 1); y->update();
	}
}
inline LL getMax (node *a, node *b) {
	access(a);
	LL res = 0;
	for (node *y(b), *z(node::nil); y != node::nil; z = y, y = y->f) {
		splay(y);
		if (y->f == node::nil) res = max(max(y->c[1]->maxw, z->maxw), y->w);
		y->sc(z, 1); y->update();
	}
	return res;
}
inline void evert (node *x) {
	access(x); splay(x); x->flipRev();
}
inline node* getRoot (node *x) {
	access(x); splay(x);
	for (; x->push(), x->c[0] != node::nil; x = x->c[0]) ;
	splay(x); return x;
}
inline node* getPre (node *x) {
	access(x); splay(x);
	for (x = x->c[0]; x->push(), x->c[1] != node::nil; x = x->c[1]) ;
	splay(x); return x;
}
inline void link (node *a, node *b) {
	evert(a);
	assert(a->f == node::nil);
	a->f = b; access(a);
}
inline void cut (node *a, node *b) {
	// cut b and its father in the tree rooted by a
	evert(a);
	access(b); splay(b);
	b->c[0]->f = node::nil; b->c[0] = node::nil; b->update();
}
