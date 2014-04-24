
const int MXN = 262145;
typedef long long lint;
lint a[MXN], b[MXN], c[MXN], d[MXN];
int sa, sb, sc, t;
lint buff[MXN * 30], *cur = buff;

inline lint* newla (int size) { return cur += size, cur - size; }
inline void setcur (lint *a) { cur = a; }

void multiply (const lint a[], const lint b[], int n, lint c[])
{ 
	if (n == 1) c[0] = a[0] * b[0], c[1] = 0;
	else
	{
		int nn = n >> 1;
		lint *bak = cur, 
			*A = newla(n), *B = newla(n), *C = newla(n), 
			*D = newla(nn), *E = newla(nn);
		multiply (a, b, nn, A);
		multiply (a + nn, b + nn, nn, B);
		for (int i = 0; i < nn; ++i)
			D[i] = a[i] + a[nn + i], E[i] = b[i] + b[nn + i];
		multiply (D, E, nn, C);
		for (int i = 0; i < n; ++i) c[i] = A[i], c[i + n] = B[i];
		for (int i = 0; i < n; ++i) c[i + nn] += C[i] - A[i] - B[i];
		setcur(bak);
	}
}

inline void process (int csz)
{
	int sz(1); ++csz;
	while (sz < csz) sz <<= 1;
	multiply (a, b, sz, d);
	multiply (d, c, sz, a);
	sz <<= 1;
	for (int i = 1; i <= sz; ++i)
		a[i] += a[i - 1];
}

