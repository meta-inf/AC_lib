//@ Suffix Array (Hash)

const ULL F = 137;

ULL pow[N], hval[N];
char dt[N];
int sa[N], height[N], n;

inline ULL get_hash (int l, int r) {
	return hval[r] - hval[l - 1] * pow[r - l + 1];
}

int cmp_suffix (int a, int b)
{
	int cp = 0;
	for (int i = 22; i >= 0; --i)
		if (a + cp + (1 << i) <= n && b + cp + (1 << i) <= n && 
		  get_hash(a, a + cp + (1 << i) - 1) == 
		  get_hash(b, b + cp + (1 << i) - 1)) cp += 1 << i;
	return dt[a + cp] < dt[b + cp];
}

void SA ()
{
	pow[0] = 1; for (int i = 1; i < n; ++i) pow[i] = pow[i - 1] * F;
	for (int i = 0; i < n; ++i) hval[i] = hval[i - 1] * F + dt[i];
	for (int i = 0; i < n; ++i) sa[i] = i;
	stable_sort(sa, sa + n, cmp_suffix);
	for (int i = 1; i < n; ++i)
	{
		int a = sa[i - 1], b = sa[i], cp = 0;
		for (int i = 22; i >= 0; --i)
			if (a + cp + (1 << i) <= n && b + cp + (1 << i) <= n &&
			  get_hash(a, a + cp + (1 << i) - 1) == 
			  get_hash(b, b + cp + (1 << i) - 1)) cp += 1 << i;
		height[i] = cp;
	}
}
