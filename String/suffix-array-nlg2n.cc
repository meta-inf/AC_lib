#include <cstdio>
#include <cstring>
#include <algorithm>
#include <utility>
using namespace std;

typedef pair<int, int> pi;

const int N = 210005;

int rk[N], dt[N], hei[N], n;
char ta[N], tb[N];

struct px {
	pi dt; int i;
	bool operator< (const px &b) const { return dt < b.dt; }
} sa[N];

int rank (int n) //~~~
{
	int p(1); sort(sa, sa + n); rk[sa[0].i] = 0;
	for (int i = 1; i < n; ++i)
		rk[sa[i].i] = (sa[i].dt == sa[i - 1].dt) ? p - 1 : p++;
	return p;
}
void DA (int n)
{
	for (int i = 0; i < n; ++i) sa[i] = (px){pi(dt[i], -1), i};
	for (int j = 1; rank(n) < n; j <<= 1)
	{
		for (int i = 0; i < n - j; ++i) sa[i] = (px){pi(rk[i], rk[i + j]), i};
		for (int i = n - j; i < n; ++i) sa[i] = (px){pi(rk[i], -1), i};
	} --n; //~~~
	for (int i = 0, k = 0, j; i < n; hei[rk[i++]] = k)
		for (j = sa[rk[i] - 1].i, k ? (--k) : 0; dt[i + k] == dt[j + k]; ++k) ;
}


	dt[n] = 0;
	DA(n + 1);

