typedef long long LL;
typedef long double LD;

int gcd (LL x, LL y) { return y ? gcd(y, x % y) : x; }

inline LL mult (LL a, LL b, LL modu)
{
	LL t = (LL)(a / (LD)modu * b + 1e-3), ret = a * b - t * modu;
	if(ret < 0) ret += modu; return ret;
}

inline LL fpow (LL x, LL m, LL modu)
{
	LL res(1);
	for (; m; m >>= 1)
	{
		x = mult(x, x, modu);
		if (m & 1) res = mult(res, x, modu);
	}
	return res;
}
inline bool primitive (LL x) // x should be large enough
{
	if (x < 300) throw;
	int p[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
	for (int i = 0; i < 10; ++i)
		if (fpow(p[i], x - 1, x) % x != 1) return 0;
	return 1;
}

map<LL, int> plist;

inline void naive_expand (LL x)
{
	for (int i = 2; i * i <= x; ++i)
		while (x % i == 0) x /= i, ++plist[i];
	if (x > 1) ++plist[x];
}

inline LL rho_iterate (LL a, LL ival)
{
	LL x = 2, y = 2, d = 1;
	while (d == 1)
	{
		x = (mult(x, x, a) + ival) % a;
		y = (mult(y, y, a) + ival) % a;
		y = (mult(y, y, a) + ival) % a;
		d = gcd(abs(y - x), a);
	}
	return d;
}
inline void expand (LL a, LL cival)
{
	if (a < 300) { naive_expand(a); return; }
	if (primitive(a)) { ++plist[a]; return; }
	LL d;
	while ((d = rho_iterate(a, cival)) == a) --cival;
	expand(d, cival); expand(a / d, cival);
}

