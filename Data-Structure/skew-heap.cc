//@ Skew Heap (Mergeable Heap)

struct skewHeap {
	skewHeap *lc, *rc;
	int val, dist;
	skewHeap ();
	void extract_min ();
} SBuff[MXN], *nil = SBuff, *St = nil + 1, *top[MXN];

skewHeap::skewHeap () : lc(nil), rc(nil), val(0), dist(0) {}
	
skewHeap* merge (skewHeap *x, skewHeap *y)
{
	if (x == nil) return y;
	if (y == nil) return x;
	if (x->val > y->val) swap(x, y);
	x->rc = merge(x->rc, y);
	/*uncomment the two lines below to get a leftish-tree*/
	//if (x->lc->dist > x->rc->dist)
		swap(x->lc, x->rc);
	//x->dist = x->rc == nil ? 0 : x->rc->dist + 1;
	return x;
}
