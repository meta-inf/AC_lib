int root[N], size[N], que[M], dep[N], fa[N], n;
lint dis[N];
vector<lint> maxDis[M];
vector< pair<int, int> > child[M];
int start[N], L, U, f, r;
int gravity, cid;

void getGravity (int rt, int ID)
{
	int x, y, z;
	for (int i = r; i > 0; --i) size[que[i]] = 1;
	for (int i = r; i > 0; --i)
		size[fa[que[i]]] += size[que[i]];
	int mn(N), cur, id(-1);
	for (int i = r; i > 0; --i)
	{
		cur = 0;
		forEdges(it, x = que[i])
			if (root[y = it->t] == rt && y != fa[x])
			cur = max(cur, size[y]);
		cur = max(cur, size[rt] - size[x]);
		if (cur < mn) mn = cur, id = x;
	}
	if ((gravity = id) == -1) return;
	forEdges(it, gravity) if (root[it->t] == rt)
		child[ID].push_back(make_pair(it->t, it->w));
	root[gravity] = gravity;
	forEdges(it, gravity) if (root[it->t] == rt)
	{
		root[x = it->t] = x;
		f = r = 0; que[++r] = x; fa[x] = 0;
		while (f < r)
			forEdges(i, y = que[++f])
				if (root[z = i->t] == rt && z != fa[y])
					fa[z] = y, root[z] = x, que[++r] = z;
	}
}

void calc (int rt, vector<lint> &res)
{
	int x, y; f = r = 0;
	que[++r] = rt; dep[rt] = 1, dis[rt] = fa[rt] = 0; 
	while (f < r)
		forEdges(it, x = que[++f])
			if (root[y = it->t] == rt && y != fa[x])
			{
				dep[y] = dep[x] + 1, dis[y] = dis[x] + it->w;
				que[++r] = y; fa[y] = x;
			}
	x = 0;
	for (int i = r; i > 0; --i) if (dep[que[i]] > x) x = dep[que[i]];
	res.resize(x + 1);
	for (int i = r; i > 0; --i)
		res[dep[que[i]]] = max(res[dep[que[i]]], dis[que[i]]);
}

int solve (int x)
{
	int r = ++cid;
	calc(x, maxDis[r]);
	getGravity(x, r);
	if (gravity == -1) return r;
	// 从子树得到当前树的信息
	for (int i = 0; i < child[r].size(); ++i)
	{	
		int id = (child[r][i].first = solve(child[r][i].first));
		for (int j = 0; j < maxDis[id].size(); ++j)
			maxDis[id][j] += child[r][i].second;
	}
	return r;
}

vector<lint> tmp;
bool check (double delt)
{
	// update the answer with root
	for (int i = 1; i <= cid; ++i)
	{
		// update the answer with tree i
	}
	return 0;
}
