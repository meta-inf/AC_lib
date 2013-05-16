const int N = 1000050, a = 7, p = 97, b = 31, P = 42961211;

set<int> cnt[N];
char buff[N << 2], *cur = buff;

int getHash (int &rdep)
{
	int res, tm, hv[3], deg(0);
	rdep = 0;
	while (*cur && *cur++ == '(') {
		hv[deg++] = getHash(tm);
		if (tm + 1 > rdep) rdep = tm + 1;
	}
	sort(hv, hv + deg);
	res = a;
	for (int i = 0; i < deg; ++i) res = (res * p ^ hv[i]) % P;
	res = res * b % P;
	cnt[rdep].insert(res);
	return res;
}

