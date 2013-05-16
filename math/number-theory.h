/*
 * Code fragments for number theory
 */
typedef long long LL;

// gcd
int gcd (int a, int b) { return b ? gcd(b, a % b) : a; }

// Z*[p]原根
LL getRoot (LL p)
{ // p is a prime
	static vector<LL> fac;
	decompose(p - 1, fac); // <-- phi(p)
	for (LL i = 2; i < p; ++i)
	{
		bool able = true;
		for_(it, fac) if (power(i, (p - 1) / *it) == 1)
			{ able = false; break; }
		if (able) return i;
	}
	return -1;
}

//离散模对数
LL babyStep (LL a, LL b, LL p)
{
	LL sq = sqrt(p);
	static map<LL, int> id; id.clear();
	for (LL i = 0, cp = 1; i <= sq; ++i, cp = cp * a % p)
		if (!id.count(cp)) id[cp] = i;
	for (LL i = 0, cq = b, ivg = power(power(a, sq), p - 2);
		i <= sq; ++i, cq = cq * ivg % p)
		if (id.count(cq)) return i * sq + id[cq];
	return -1;
}

//扩展GCD
LL exGCD (LL a, LL b, LL &x, LL &y)
{
	if (!b) { x = 1, y = 0; return a; }
	LL d = exGCD(b, a % b, y, x);
	y -= x * (a / b);
	return d;
}

//模线性方程
void linerEqu (LL a, LL b, LL c, vector<LL> &res)
{
	LL x, y, d = exGCD(a, c, x, y);
	res.clear();
	if (b % d) return;
	x = (x % c + c) * (b / d) % c;
	for (LL i = 0, t = c / d; i < d; ++i)
		res.push_back((x + i * t) % c);
}

//模线性方程组
LL modequ (vector< pair<LL, LL> > &a)
{
	LL n(1), res(0);
	for_(i, a) n *= i->second;
	for_(i, a)
	{
		LL m = n / i->second, x, y;
		if (exgcd(m % i->second, i->second, x, y) != 1) throw;
		res += x * m * i->first; res %= n;
	}
	while (res < 0) res += n;
	return res;
}

//multiply in case of overflow
LL mult (LL a, LL b, LL modu)
{
#ifndef LOCAL
	LL res = 0;
	for (; b; b >>= 1)
	{
		if (b & 1)
		{
			res += a;
			while (res >= modu) res -= modu;
		}
		a <<= 1; // A
		while (a >= modu) a -= modu;
	} return res;
#else
	return (__int128)a * b % modu;
#endif
}
//another approach with some precision error
inline LL mult (LL a, LL b, LL c)
{
	LL t = (LL)(a / (__float128)c * b + 1e-3);
	LL ret = a * b - c * t;
	return (ret % c + c) % c;
}

//iterative fast power
LL fpow (LL x, LL m, LL modu)
{
	LL res(1);
	for (; m; m >>= 1)
	{
		if (m & 1) res = mult(res, x, modu);
		x = mult(x, x, modu); // X
	}
	return res;
}

//Rabin-Miller
int rtest (LL x)
{
	for (int i = 0; i < 20; ++i)
	{
		LL cr = rand() % (x - 1) + 1;
		if (fpow(cr, x - 1, x) % x != 1) return 0;
	}
	return 1;
}
