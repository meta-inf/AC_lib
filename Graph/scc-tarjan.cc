//@ Tarjan (SCC)
/*
 * Tarjan Algorithm on directed graph
 * Finding SCC
 * Last Edit : before 2013
 * */

#define pback push_back
#define for_(it,E) for(__typeof(E.begin())it=E.begin();it!=E.end();++it)

const int N = 5005, VT = N << 1;

vector<int> orig[N], esat[VT], modi[VT], conf[VT];
int on, cn, in;

namespace tarjan {
	int dfn[VT], low[VT], ins[VT], *cc, cnt, ctime;
	stack<int> s;
	void dfs (int x)
	{
		dfn[x] = low[x] = ++ctime; ins[x] = 1;
		s.push(x);
		for_(it, esat[x])
			if (!dfn[*it]) dfs(*it), low[x] = min(low[x], low[*it]);
			else if (ins[*it]) low[x] = min(low[x], dfn[*it]);
		if (dfn[x] == low[x])
		{
			cc[x] = ++ cnt; ins[x] = 0;
			while (s.top() != x) cc[s.top()] = cnt, ins[s.top()] = 0, s.pop();
			s.pop();
		}
	}
	int solve (int *res)
	{
		cc = res; cnt = 0;
		for (int i = 1; i <= on; ++i) if (!dfn[i]) dfs(i);
		for (int i = 1; i <= on; ++i)
			for_(it, esat[i]) if (cc[i] != cc[*it])
				modi[cc[i]].pback(cc[*it]);
		return cnt;
	}
}

