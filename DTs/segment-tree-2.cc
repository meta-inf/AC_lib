struct node {
	static node NBf[LIM], *NCr, *nil;
	node *lc, *rc;
	int s, tag, maxt, mint;
	LL sumt, delt;
	static void init () {
		nil = NBf; NCr = NBf + 1;
		nil->lc = nil->rc = nil;
		nil->maxt = -inf, nil->mint = inf;
	}
	static node* get () {
		assert(NCr - NBf < LIM);
		NCr->lc = NCr->rc = nil;
		return NCr++;
	}
	static node* getLeaf (int l, int r, int p, int w) {
		node *re = get();
		if (l == r) {
			re->s = 1;
			re->tag = re->maxt = re->mint = re->sumt = w;
		}
		else {
			int m = (l + r) >> 1;
			if (p <= m) re->lc = getLeaf(l, m, p, w);
			else re->rc = getLeaf(m + 1, r, p, w);
			re->update();
		}
		return re;
	}
	void add (int d) {
		assert(this != nil);
		delt += d, sumt += (LL)s * d;
		maxt += d, mint += d;
	}
	void push () {
		assert(this != nil);
		if (delt) {
			if (lc != nil) lc->add(delt);
			if (rc != nil) rc->add(delt);
			delt = 0;
		}
	}
	void update () {
//		assert(!delt && this != nil);
		s = lc->s + rc->s;
		sumt = lc->sumt + rc->sumt;
		maxt = max(lc->maxt, rc->maxt);
		mint = min(lc->mint, rc->mint);
	}
	void print (int l, int r) {
		if (l == r) {
			assert(s);
			cerr << l << ' ';
		}
		else {
			int m = (l + r) >> 1;
			if (lc != nil) lc->print(l, m);
			if (rc != nil) rc->print(m + 1, r);
		}
	}
	void split (int l, int r, int v, node *&rl, node *&rr) {
		if (this == nil) {
			rl = rr = nil;
			return;
		}
		if (v >= r) {
			rl = this, rr = nil;
			return;
		}
		if (v < l) {
			rl = nil, rr = this;
			return;
		}
		push();
		int m = (l + r) >> 1;
		rl = node::get(), rr = node::get();
		lc->split(l, m, v, rl->lc, rr->lc);
		rc->split(m + 1, r, v, rl->rc, rr->rc);
		if (rl->lc == nil && rl->rc == nil) rl = nil; else rl->update();
		if (rr->lc == nil && rr->rc == nil) rr = nil; else rr->update();
		assert(rl->lc == nil && rl->rc == nil || rl->s);
		assert(rr->lc == nil && rr->rc == nil || rr->s);
	}
	static node* merge (int l, int r, node *a, node *b) {
		if (a == nil) return b;
		if (b == nil) return a;
		a->push(); b->push();
		node *re = node::get();
		int m = (l + r) >> 1;
		re->lc = merge(l, m, a->lc, b->lc);
		re->rc = merge(m + 1, r, a->rc, b->rc);
		re->update();
		assert(re->s);
		return re;
	}
} node::NBf[LIM], *node::NCr, *node::nil;
