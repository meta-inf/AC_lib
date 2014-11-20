//@ Simplex Algorithm
#include <bits/stdc++.h>
using namespace std;

typedef long double LD;

const LD inf = 1e100, eps = 1e-9;
const int N = 505;

inline int sgn (LD x) { return fabs(x) < eps ? 0 : x > 0 ? 1 : -1; }

LD A[N][N];
int equ[N], var[N], n, m;

void pivot (int e, int v)
{
	static int que[N], f;

	swap(equ[e], var[v]);
	f = 0;
	for (int i = 0; i <= m; ++i)
		if (sgn(A[e][i])) que[++f] = i;

	A[e][v] = 1 / A[e][v];
	for (int i = 1; i <= f; ++i)
		if (que[i] != v) A[e][que[i]] *= A[e][v];

	for (int i = 0; i <= n + 1; ++i)
		if (i != e && sgn(A[i][v]))
		{
			LD t = A[i][v]; A[i][v] = 0;
			for (int j = 1; j <= f; ++j)
				A[i][que[j]] -= A[e][que[j]] * t;
		}
}

/*
   max(\sum A_{0,i}x_i)
   s.t. \sum A_{i,j}x_j \leq A_{i,0}
 */
LD base_simplex ()
{
	// assert A[0][i] >= 0
	while (1)
	{
		int v = 1, e = 0;
		for (int i = 2; i <= m; ++i)
			if (A[0][i] > A[0][v]) v = i;

		if (sgn(A[0][v]) <= 0) return -A[0][0];

		for (int i = 1; i <= n; ++i)
			if (sgn(A[i][v]) > 0 && (e == 0 || A[i][0] / A[i][v] < A[e][0] / A[e][v]))
				e = i;

		if (! e) return +inf; // unbounded
		pivot(e, v);
	}
}
LD simplex ()
{
	for (int i = 1; i <= n; ++i) equ[i] = -i;
	for (int i = 1; i <= m; ++i) var[i] = i;

	int v = 1;
	for (int i = 2; i <= n; ++i)
		if (A[i][0] < A[v][0]) v = i;

	if (A[v][0] < 0) // Initialization (CLRS)
	{
		// Let the original objective function get transformed with the matrix,
		// so that we don't need to restore the basic variables after init process.
		for (int i = 1; i <= m; ++i)
		{
			A[n + 1][i] = A[0][i];
			A[0][i] = 0;
		}
		++m; var[m] = m;
		A[0][m] = -1;
		for (int i = 1; i <= n; ++i) A[i][m] = -1;
		pivot(v, m);
		if (sgn(base_simplex()) != 0)
			return -inf; // no solution

		for (int i = 0; i <= m; ++i)
		{
			A[0][i] = A[n + 1][i];
			A[n + 1][i] = 0;
		}

		// add constraint x_m <= 0
		int p = find(var + 1, var + 1 + m, m) - var;
		if (var[p] == m)
		{
			++n;
			A[n][p] = 1;
		}
		else
		{
			p = find(equ + 1, equ + 1 + n, m) - equ;
			++n;
			for (int i = 0; i <= m; ++i)
				A[n][i] = -A[p][i];
		}
	}
	return base_simplex();
}
