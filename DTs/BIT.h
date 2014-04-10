struct BIT {
	LL C[N];
	int n;
	LL sum (int x) {
		LL r = 0;
		for (; x; x -= x & -x) r += C[x];
		return r;
	}
	LL sum (int l, int r) {
		if (l > r) return 0;
		return sum(r) - sum(l - 1);
	}
	void modify (int x, LL d) {
		for (; x <= n; x += x & -x) C[x] += d;
	}
	void clear () {
		fill(C + 1, C + 1 + n, 0LL);
	}
} 
