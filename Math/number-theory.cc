//@ Header for Number Theory
typedef long long LL;

// gcd
int gcd (int a, int b) { return b ? gcd(b, a % b) : a; }

LL getRoot (LL p) // Z*[p]原根
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

LL babyStep (LL a, LL b, LL p) // 离散模对数
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

LL exGCD (LL a, LL b, LL &x, LL &y) // 扩展GCD
{
	if (!b) { x = 1, y = 0; return a; }
	LL d = exGCD(b, a % b, y, x);
	y -= x * (a / b);
	return d;
}

void linerEqu (LL a, LL b, LL c, vector<LL> &res) // 模线性方程
{
	LL x, y, d = exGCD(a, c, x, y);
	res.clear();
	if (b % d) return;
	x = (x % c + c) * (b / d) % c;
	for (LL i = 0, t = c / d; i < d; ++i)
		res.push_back((x + i * t) % c);
}

LL modequ (vector< pair<LL, LL> > &a) // 模线性方程组
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

LL mult (LL a, LL b, LL modu) // multiply in case of overflow
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

inline LL mult (LL a, LL b, LL c) // another approach with some precision error
{
	LL t = (LL)(a / (__float128)c * b + 1e-3);
	LL ret = a * b - c * t;
	return (ret % c + c) % c;
}

inline LL mult (LL x, LL y, LL z) { return x * y % z; }

LL fpow (LL x, LL m, LL modu) // iterative fast power
{
	LL res(1);
	for (; m; m >>= 1)
	{
		if (m & 1) res = mult(res, x, modu);
		x = mult(x, x, modu); // X
	}
	return res;
}

int rtest (LL x) // Rabin-Miller
{
	int P[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
	if (x < 100)
	{
		for (int i = 2; i < x; ++i)
			if (x % i == 0) return 0;
		return 1;
	}
	for (int i = 0; i < 10; ++i)
		if (fpow(P[i], x - 1, x) % x != 1) return 0;
	
	return 1;
}

inline LL C (LL n, LL m) // Using Lucas Theorem
{
	LL res(1);
	while (n) {
		int x = n % P, y = m % P;
		if (x >= y)
		{
			LL cur = (LL)fac[x] * invfac[y] % P * invfac[x - y] % P;
			res = res * cur % P;
		}
		else return 0;
		n /= P, m /= P;
	}
	return res;
}

template <int N>
struct primeTable // Euler Sieve
{
	int sdiv[N], primes[N], pcnt;
	primeTable ()
	{
		pcnt = 0;
		for (int i = 2; i < N; ++i)
		{
			if (!sdiv[i]) primes[++pcnt] = (sdiv[i] = i);
			for (int j = 1; j <= pcnt && primes[j] * i < N; ++j)
			{
				sdiv[primes[j] * i] = primes[j];
				if (i % primes[j] == 0) break;
			}
		}
	}
};

struct frac // Fraction Class
{
	LL num, den;
	frac (LL n, LL d) {
		if (n == 0 || d == 0) { num = 0; den = 1; }
		else { LL r = gcd(n, d); num = n / r; den = d / r; }
		if (den < 0) num = -num, den = -den;
	}
	frac operator+ (const frac &b) {
		return frac(num * b.den + den * b.num, den * b.den);
	}
	frac operator- (const frac &b) {
		return frac(num * b.den - den * b.num, den * b.den);
	}
	frac operator* (const frac &b) {
		return frac(num * b.num, den * b.den);
	}
	frac operator/ (const frac &b) {
		return frac(num * b.den, den * b.num);
	}
};
ostream& operator<< (ostream &a, const frac &b) {
	return a << b.num << ' ' << b.den;
}

// vim:ts=4:fdm=syntax 
