//@ Suffix Array (DA)

namespace SA
{

	int r[N], sa[N], rank[N], height[N];
	int wa[N], wb[N], ws[N], wv[N];
	inline int cmp (int *r, int x, int y, int l) {
		return r[x] == r[y] && r[x + l] == r[y + l];
	}
	void da (int *r, int n, int m)
	{
		int i, j, p, *x = wa, *y = wb;
		for (i = 0; i < m; ++i) ws[i] = 0;
		for (i = 0; i < n; ++i) ws[x[i] = r[i]]++;
		for (i = 1; i < m; ++i) ws[i] += ws[i - 1];
		//downto is for stability concern when elements duplicate
		for (i = n - 1; i >= 0; --i) sa[--ws[x[i]]] = i;
		for (j = 1, p = 1; p < n; j <<= 1, m = p)
		{
			//sort by (rank[i],rank[i+j])
			//second
			//second == 0, in arbitary order
			for (i = n - j, p = 0; i < n; ++i) y[p++] = i;
			// sa[i]-j : (*,i)
			for (i = 0; i < n; ++i) if (sa[i] >= j) y[p++] = sa[i] - j;
			//first
			for (i = 0; i < n; ++i) wv[i] = x[y[i]];
			for (i = 0; i < m; ++i) ws[i] = 0;
			for (i = 0; i < n; ++i) ws[wv[i]]++;
			for (i = 1; i < m; ++i) ws[i] += ws[i - 1];
			//rvalue is not i : stability
			for (i = n - 1; i >= 0; --i) sa[--ws[wv[i]]] = y[i]; 
			//store the current rank in x
			for (swap(x, y), p = i = 1, x[sa[0]] = 0; i < n; ++i)
				x[sa[i]] = cmp(y, sa[i - 1], sa[i], j) ? p - 1 : p++;
		}
	}
	// LCP(i, SA[Rank[i] - 1]) >= LCP(i - 1, SA[Rank[i - 1] - 1]) - 1
	void calheight (int *r, int n)
	{
		int i, j, k = 0;
		for (i = 1; i <= n; ++i) rank[sa[i]] = i;
		for (i = 0; i < n; height[rank[i++]] = k)
			for (k ? --k : 0, j = sa[rank[i] - 1]; r[i + k] == r[j + k]; ++k) ;
	}

	struct SparseTable
	{
		static const int lgN = 22;
		int GP[lgN][N], n, lg[N];
		SparseTable ()
		{
			lg[1] = 0;
			for (int i = 2, j = 0; i < N; ++i)
				lg[i] = (j += ((1 << (j + 1)) == i));
		}
		void init (int S[], int n)
		{
			this->n = n;
			for (int j = 0; j < n; ++j)
				GP[0][j] = S[j];

			for (int i = 1; i <= lg[n]; ++i)
				for (int j = 0; j + (1 << i) <= n; ++j)
					GP[i][j] = min(GP[i - 1][j], GP[i - 1][j + (1 << (i - 1))]);
		}
		inline int query (int l, int r)
		{
			assert(l >= 0 && l <= r && r < n);

			int z = lg[r - l + 1];
			int ret = min(GP[z][l], GP[z][r - (1 << z) + 1]);
			return ret;
		}
	} st;

	int n;
	// S[0 .. n)
	inline void init (int S[], int n)
	{
		SA::n = n;
		S[n] = 0;
		da(S, n + 1, 256);
		calheight(S, n);
		st.init(height, n + 1);
	}
	inline int LCP (int i, int j)
	{
		if (i == j) return n - i;
		int l = rank[i], r = rank[j];
		if (l > r) swap(l, r);
		return st.query(l + 1, r);
	}
}
