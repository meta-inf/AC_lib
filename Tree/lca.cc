//@ LCA

void dfs (int x, int ax, int d)
{
	in[x] = ++ls; de[x] = d;
	ac[x][0] = ax;
	for (int k = 1; ac[x][k - 1]; ++k)
		ac[x][k] = ac[ac[x][k - 1]][k - 1];
	for (edge *it = e[x]; it; it = it->n)
		if (it->t != ax) dfs(it->t, x, d + 1);
	out[x] = ++ls;
}

int lg[Nm];

void lift (int &x, int d) {
	for (; de[x] > d; x = ac[x][lg[de[x] - d]]) ;
}
int LCA (int x, int y)
{
	if (de[x] < de[y]) lift(y, de[x]); else lift(x, de[y]);
	while (x != y)
	{
		int d;
		for (d = 0; ac[x][d] != ac[y][d]; ++d) ;
		d -= bool(d); x = ac[x][d], y = ac[y][d];
	}
	return x;
}

