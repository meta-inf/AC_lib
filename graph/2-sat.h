/*
 * Construct a feasible solution for 2-SAT problem using tarjan
 * Counting the solutions is possibly NPC.
 * */
#include "scc-tarjan.h"

namespace SAT {
	const int AND = 0, OR = 1, SEL = 0, UNS = 1;
	int tlist[VT], cc[VT], color[VT], tmp[VT];
	void tsort ()
	{
		static int deg[VT], cur = 0;
		static queue<int> que;
		for (int i = 1; i <= cn; ++i)
			for_(it, modi[i]) ++ deg[*it];
		for (int i = 1; i <= cn; ++i)
			if (!deg[i]) que.push(i);
		while (!que.empty())
		{
			int x = que.front(); que.pop(); tlist[++cur] = x;
			for_(it, modi[x]) if (!--deg[*it]) que.push(*it);
		}
	}
	inline int dual (int x) { return x + ((x & 1) ? 1 : -1); }
	inline int sel (int x) { return x * 2 - 1; }
	inline int uns (int x) { return x * 2; }
	inline void add (int x, int y, int ty)
	{ // edge(x,y): select(x) imp. select(y)
		if (ty == AND) // x&&y==0
		{
			esat[sel(x)].pback(uns(y));
			esat[sel(y)].pback(uns(x));
		}
		else // x||y==1
		{
			esat[uns(x)].pback(sel(y));
			esat[uns(y)].pback(sel(x));
		}
	}
	bool solve (int *res)
	{
		cn = tarjan::solve(cc);
		for (int i = 1; i <= on; ++i)
		{
			if (cc[i] == cc[dual(i)]) return false;
			conf[cc[i]].pback(cc[dual(i)]);
		}
		tsort();
		fill(color + 1, color + cn + 1, -1);
		for (int i = cn; i > 0; --i)
		{
			int x = tlist[i];
			if (color[x] != -1) continue;
			color[x] = SEL;
			for_(it, modi[x]) if (color[*it] == UNS)
				{ color[x] = UNS; break; }
			for_(it, conf[x]) color[*it] = color[x] ^ 1;
		}
		for (int i = 1; i <= on; ++i) tmp[i] = color[cc[i]];
		for (int i = 1; i <= in; ++i) res[i] = tmp[sel(i)] == SEL;
		return true;
	}
}
