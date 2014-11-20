//@ Hash

const int N = 1000050, P = 31;

int hval[N], power[N], str[N], n;

int gethash (int l, int r) {
	return hval[r] - hval[l - 1] * power[r - l + 1];
}

void init ()
{
	srand(123);
	power[0] = 1;
	for (int i = 1; i <= n; i++) power[i] = power[i - 1] * P;
	hval[1] = str[1];
	for (int i = 2; i <= n; i++) hval[i] = hval[i - 1] * P + str[i];
}

int main()
{
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i)
		scanf("%d", str + i);
	init();
	int m, a, b;
	for (scanf("%d", &m); m--; )
	{
		scanf("%d%d", &a, &b);
		printf("%d\n", gethash(a, b));
	}
}
