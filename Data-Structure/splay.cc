//@ Splay Tree

// Rev 2
struct node {
	static node *nil;
	node *f, *c[2];
	int s, dt;
	lint hval;
	node () : f(nil), s(0), hval(0) { c[0] = c[1] = nil; }
	int w () { return this == f->c[1]; }
	void setc (node *t, int k) { c[k] = t; t->f = this; }
	void update ()
	{
		hval = c[0]->hval * M + dt;
		hval = hval * power[c[1]->s] + c[1]->hval;
		s = c[0]->s + c[1]->s + 1;
	}
} T[N], *node::nil = T, *cur = T + 1, *root;
 
inline node* newNode (int dt = 0)
{
	cur->dt = dt, cur->s = 1;
	return cur++;
}
 
inline void zig (node *x)
{
	int w = x->w(); node *y = x->f;
	y->setc(x->c[!w], w);
	if (y->f != node::nil) y->f->setc(x, y->w());
	else x->f = y->f;
	x->setc(y, !w);
	y->update();
	if (y == root) root = x;
}
 
void splay (node *x, node *y)
{
	for (; x->f != y; zig(x)) if (x->f->f != y)
		if (x->w() == x->f->w()) zig(x->f); else zig(x);
	x->update();
}
node* select (node *x, int k) // 0-based
{
	for (; ; )
	{
		int r = x->c[0]->s;
		if (k == r) return x;
		x = x->c[r < k];
		if (r < k) k -= r + 1;
	}
}
 
node* expose (int l, int r) // (l, r), 1-based
{
	splay(select(root, l), node::nil);
	splay(select(root, r), root);
	return root->c[1]->c[0];
}
 
lint getHash (int l, int r) { return expose(l - 1, r + 1)->hval; }
 
void insert (int rk, int dt) {
	node *t = newNode(dt);
	expose(rk - 1, rk);
	root->c[1]->setc(t, 0);
	splay(t, node::nil);
}
void replace (int p, int dt) {
	node *x = expose(p - 1, p + 1);
	x->dt = dt; splay(x, node::nil);
}
 
int query (int a, int b)
{
	int res(0), n = root->s - 2;
	if (getHash(a, a) != getHash(b, b)) return 0;
	for (int i = 1 << 18; i; i >>= 1)
		if (a + res + i <= n && b + res + i <= n &&
			getHash(a, a + res + i) == getHash(b, b + res + i))
			res += i;
	return ++ res;
}
 
struct qNode { int l, r; node *t; } que[N];
void buildTree (char *s, int len)
{
	int l(0), r(0), m;
	que[++ r] = (qNode){0, len, root = newNode()};
	while (l < r)
	{
		qNode cr = que[++ l];
		cr.t->dt = s[m = (cr.l + cr.r) >> 1];
		if (cr.l < m)
		{
			cr.t->setc(newNode(), 0);
			que[++ r] = (qNode){cr.l, m - 1, cr.t->c[0]};
		}
		if (m < cr.r)
		{
			cr.t->setc(newNode(), 1);
			que[++ r] = (qNode){m + 1, cr.r, cr.t->c[1]};
		}
	}
	for (int i = r; i > 0; --i) que[i].t->update();
}
 
void printTree (node *x, int de)
{
	for (int i = 1; i <= de; ++i) fputc(' ', stderr);
	fprintf(stderr, "Node %d %d %I64u %d %d %d\n", x-T, x->dt, x->hval,
		x->s, x->c[0]-T, x->c[1]-T);
	if (x->c[0] != node::nil) printTree(x->c[0], de + 1);
	if (x->c[1] != node::nil) printTree(x->c[1], de + 1);
}
