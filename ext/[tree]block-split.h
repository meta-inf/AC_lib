struct info {
	info () {}
	info operator+ (const info &b) const { } // return a + b
	info operator~ () { } // return the reverse version of *this
} node[N], block[N];
 
int root[N], pre[N], depth[N], size[N];
int bSize;
 
void update (int x)
{
	if (root[x] == x) block[x] = node[x];
	else block[x] = node[x] + block[pre[x]];
	forEdges(it, x)
		if (it->t != pre[x] && root[it->t] == root[x])
			update(it->t);
}
 
void dfs (int x, int ax, int rt)
{
	root[x] = rt, pre[x] = ax, ++size[rt];
	depth[x] = depth[ax] + 1;
	if (x == rt) block[x] = node[x];
	else block[x] = node[x] + block[ax];
	forEdges(it, x) if (it->t != ax)
		if (size[rt] < bSize) dfs(it->t, x, rt);
		else dfs(it->t, x, it->t);
}
 
info query (int x, int y)
{
	info lInfo, rInfo;
	while (root[x] != root[y])
		if (depth[root[x]] > depth[root[y]])
			lInfo = lInfo + block[x], x = pre[root[x]];
		else rInfo = ~block[y] + rInfo, y = pre[root[y]];
	while (x != y)
		if (depth[x] > depth[y])
			lInfo = lInfo + node[x], x = pre[x];
		else rInfo = node[y] + rInfo, y = pre[y];
	info res = lInfo + node[x] + rInfo;
	return res;
}
 
int main ()
{
	// ...
	for (bSize = 0; bSize * bSize < n; ++bSize) ;
	// add edges
	for (int i = 1; i < n; ++i)
	{
		scanf("%d%d", &a, &b);
		link(a, b); link(b, a);
	}
	//init node record
	char s[2];
	for (int i = 1; i <= n; ++i) {
		scanf("%s", s); node[i] = info(s);
	}
	dfs(1, 0, 1);
	for (int i = 1; i <= m; ++i)
	{
		if (/*query a node*/) {
			printf("%d\n", query(a, b));
		}
		else { // modify a node
			node[a] = info(s); update(a);
		}
	}
	return 0;
}
