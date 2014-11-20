//@ Factorial

/*
 * given p, P=p^k, calculate N = a(modP) * p^b
 * O(P) - O(logN)
 * */
struct mint { LL r, c; };

LL fact[N], p, P, cnt[N], n, m;

LL fpow (LL x, LL m, LL modu)
{
	if (!m) return 1;
	LL y = fpow(x, m >> 1, modu), r = y * y % modu;
	return (m & 1) ? r * x % modu : r;
}
LL exGCD (LL a, LL b, LL &x, LL &y)
{
	if (!b) { x = 1, y = 0; return a; }
	LL d = exGCD(b, a % b, y, x);
	y -= x * (a / b);
	return d;
}

inline mint getInv (mint p) {
	return (mint){fpow(p.r, P * (::p - 1) / ::p - 1, P), -p.c};
}
mint operator* (mint a, mint b) { return (mint){a.r * b.r % P, a.c + b.c}; }

mint gfact (LL n)
{
	if (n < p) return (mint){fact[n], 0};
	LL base = fpow(fact[P - 1], n / P, P) * fact[n % P] % P;
	return (mint){base, n / p} * gfact(n / p);
}

