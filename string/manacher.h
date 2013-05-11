
int rad[N];

int main ()
{
	int m, n(0);
	scanf("%d\n", &m);
	str[++n] = '$';
	str[++n] = '#';
	while (m--)
	{
		char op; scanf("%c", &op);
		str[++n] = op;
		str[++n] = '#';
	} str[++n] = '%';
	int mx = 2, id = 1; rad[1] = 1;	
	for (int i = 2; i <= n; ++i)
	{
		rad[i] = min(rad[id * 2 - i], mx - i);
		for (; i + rad[i] <= n && str[i + rad[i]] == str[i - rad[i]]; ++rad[i]) ;
		if (rad[i] + i > mx) mx = rad[i] + i, id = i;
	}
}



