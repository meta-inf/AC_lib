struct tree
{
	int l, r;
	tree *lc, *rc;
	info i;
	tree (int _l, int _r)
	{
		l = _l, r = _r;
		lc = rc = 0;
		if (l == r)
		{
			i = info(1, 1); // ...
			return;
		}
		int m = (l + r) >> 1;
		lc = new tree(l, m);
		rc = new tree(m + 1, r);
		i = lc->i + rc->i;
	}
	void modify (int p)
	{
		if (l == r)
		{
			i = info(0, 0); // ...
			return;
		}
		int m = (l + r) >> 1;
		if (p <= m) lc->modify(p);
		else rc->modify(p);
		i = lc->i + rc->i;
	}
	info query (int ql, int qr)
	{
		if (ql > qr) return info(0, 0);
		if (ql <= l && r <= qr) return i;
		int m = (l + r) >> 1;
		info res(0, 0); // ...
		if (ql <= m) res = lc->query(ql, qr);
		if (qr > m) res = res + rc->query(ql, qr);
		return res;
	}
} *root;

