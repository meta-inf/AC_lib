
typedef long long lint;
const lint P = (15 << 27) + 1, X = 27, R = 137,
	MXN = 262144;
lint a[MXN + 1], b[MXN + 1], c[MXN + 1];
int n, mxa, mxb, mxc;

inline lint mult (lint a, lint b) { return a * b % P; }
lint fpow (lint a, lint b)
{
	if (b == 0) return 1;
	lint p = fpow(a, b >> 1), r = mult(p, p);
	return (b & 1) ? mult(r, a) : r;
}

inline void calc (lint a[], lint wt)
{
	for (lint m(2); m <= n; m <<= 1)
		for (lint k(0), wi(fpow(wt, n / m)); k < n; k += m) //~
			for (lint j(0), w(1); (j << 1) < m; ++j, w = mult(w, wi))
			{
				lint u(a[k + j]), v(mult(a[k + j + (m >> 1)], w));
				a[k + j] = (u + v) % P;
				a[k + j + (m >> 1)] = (u - v + P) % P;
			}
}

inline void bitrc (lint a[])
{
	int mx(0); while ((1 << mx) < n) ++mx;
	for (int i = 0; i <= n; ++i)
	{
		int x(i), y(0);
		for (int j(0); j < mx; ++j)
			y = (y << 1) | (x & 1), x >>= 1;
		if (i < y) swap(a[i], a[y]);
	}
}

inline void FFT (lint a[])
{
	bitrc(a);
	calc(a, fpow(R, (15 << X) / n));
}
//n is a power of 2
inline void IFFT (lint a[])
{
	bitrc(a);
	calc(a, fpow(R, (15 << X) / n * (P - 2)));
	lint inv_n = fpow(n, P - 2);
	for (int i = 0; i <= n; ++i)
		a[i] = mult(a[i], inv_n); 
}


