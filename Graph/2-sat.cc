//@ 2-SAT
/*
 * Construct a feasible solution for 2-SAT problem using tarjan
 * Counting the solutions is hard to solve.
 * Last Edit : Feb 2014
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

/*
 * 建立 2N 个点分别表示变量 i 为 true / false 的情况。
 * 如果根据约束 (i, v) imp (j, v')，则连边 ((i, v) -> (j, v')) 
 * // v, v' \in {true, false}
 * 缩 SCC 之后，如果 scc[(i, true)] == scc[(i, false)] 则无解
 * 否则如下构造方案：将原图拓扑排序为tlist[1 .. cn]，之后按拓扑序
 *		sel(scc[i]) = 0 iff (exists (i, j) in E, sel(j) = 0)
 *			// and sel(scc[i]) is not set
 *      sel(scc[conf[i]]) = not sel(scc[i])
 * 由于 2-SAT 构图有对称性，这一做法的正确性可归纳证明。
 * */

