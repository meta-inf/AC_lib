//# Online Dynamic Connectivity
/*
 * O(log^2N) online dynamic connectivity
 * Maybe the ETT part is useful?
 * */

#include <bits/stdc++.h>
using namespace std;

#define rep(I,L,R) for(int I=(L);I<=(R);++I)
#define NDEBUG

typedef long long LL;

const LL inf = ~0ULL >> 3;
const int N = int(5e4) + 50;


struct enode {
	static const int LIM = int(2e6) + 50;
	static enode *nil, *cur, T[LIM];
	enode *f, *c[2];
	int s, tx, tag[2], sumt[2];
	LL val, sumv, maxv;
	enode () {}
	inline int w () { return this == f->c[1]; }
	inline void sc (enode *t, int k) { c[k] = t; if (t != nil) t->f = this; }
	inline void update () {
		assert(this != nil);
		s = c[0]->s + c[1]->s + 1;
		sumt[0] = tag[0] + c[0]->sumt[0] + c[1]->sumt[0];
		sumt[1] = tag[1] + c[0]->sumt[1] + c[1]->sumt[1];
		sumv = c[0]->sumv + c[1]->sumv + val;
		maxv = max(max(c[0]->maxv, c[1]->maxv), val);
	}
	inline void cut (int k) { c[k]->f = nil; c[k] = nil; update(); }
	inline void zig () {
		int w = this->w(); enode *y = f;
		y->sc(c[!w], w);
		if (y->f != nil) y->f->sc(this, y->w());
		else f = y->f;
		sc(y, !w); y->update();
	}
	inline enode* splay (enode *y) {
		for (; f != y; zig()) if (f->f != y)
			if (w() == f->w()) f->zig(); else zig();
		update();
		return this;
	}
	inline enode* splay () { return splay(nil); }
	inline enode* gmin () {
		enode *a = this, *b = a;
		for (; b != nil; a = b, b = b->c[0]) ;
		return a;
	}
	inline enode* gmax () {
		enode *a = this, *b = a;
		for (; b != nil; a = b, b = b->c[1]) ;
		return a;
	}
	static enode* join (enode *a, enode *b) {
		if (a == nil) return b;
		if (b == nil) return a;
		enode *r = a->gmax();
		r->splay();
		r->sc(b, 1); r->update();
		return r;
	}
	static void init () {
		nil = T;
		nil->f = nil->c[0] = nil->c[1] = nil;
		nil->s = 0;
		cur = T + 1;
	}
	static enode* get (int tx) {
		cur->f = cur->c[0] = cur->c[1] = nil;
		cur->tx = tx; cur->update();
		return cur++;
	}
	static void dump (enode* t, vector<enode*> &a) {
		if (t == nil) return;
		assert(t->c[0] == nil || t->c[0]->f == t); dump(t->c[0], a);
		a.push_back(t);
		assert(t->c[1] == nil || t->c[1]->f == t); dump(t->c[1], a);
		assert(t->s == t->c[0]->s + t->c[1]->s + 1);
		rep(d, 0, 1) {
			assert(t->tag[d] >= 0);
			assert(t->sumt[d] == t->c[0]->sumt[d] + t->c[1]->sumt[d] + t->tag[d]);
		}
	}
	static void print (enode* t) {
		if (t == nil) return;
		assert(t->c[0] == nil || t->c[0]->f == t); print(t->c[0]);
		printf("%d(%d) ", t->tx, (int)(t - enode::T));
		//	printf("%d ", t->tx);
		assert(t->c[1] == nil || t->c[1]->f == t); print(t->c[1]);
		//	assert(t->s == t->c[0]->s + t->c[1]->s + 1);
	}

} enode::T[enode::LIM], *enode::nil, *enode::cur;

#define Px first
#define Py second
int cmdcnt, cmdcnt2;

struct EulerTourTree {
	int n;
	map<int, pair<enode*, enode*>> iter[N];
	enode* tagIter[N];
	set<int> SE[2][N];
	set<int> e[N];
	LL W[N];
	inline void reg_iter (int u, int v, enode* a, enode* b) {
		assert(!iter[u].count(v));
		iter[u][v] = make_pair(a, b);
	}
	inline void del_iter (int u, int v) {
		assert(iter[u].count(v));
		iter[u].erase(v);
	}
	enode* evert (int a, int b = 0) {
		assert(!iter[a].empty());
		enode *f = iter[a].lower_bound(b)->Py.Px;
		f->splay();
		enode *t = f->c[0]; f->cut(0); f->update();
		enode::join(f, t); f->splay();
		return f;
	}
	inline int connected (int u, int v) { 
		if (u == v) return 1;
		if (iter[u].empty() || iter[v].empty()) return 0;
		enode *i = iter[u].begin()->Py.Px; i->splay();
		enode *j = iter[v].begin()->Py.Px; j->splay();
		return !(i->f == enode::nil && j->f == enode::nil);
	}
	inline void chg_iter (enode *x, enode *y, enode *z, enode *w) {
		int a = x->tx, b = y->tx;
		assert(iter[a].count(b) && (iter[a][b].Px == x && iter[a][b].Py == y));
		iter[a][b] = make_pair(z, w);
	}
	inline enode* get_node (int u) {
		enode *ret = enode::get(u);
		if (!tagIter[u]) {
			tagIter[u] = ret;
			rep(d, 0, 1)
				ret->tag[d] = SE[d][u].size();

			ret->val = W[u];	
			ret->update();
		}
		return ret;
	}
	void link (int u, int v) { ++cmdcnt2;
		e[u].insert(v); e[v].insert(u); // TEST
		if (iter[u].empty() && iter[v].empty())
		{
			enode *fu = get_node(u), *fv = get_node(v);
			reg_iter(u, v, fu, fv);
			reg_iter(v, u, fv, fu);
			fu->sc(fv, 1); fu->update();
			return;
		}
		if (iter[u].empty()) swap(u, v);
		enode *rv, *nv;
		if (iter[v].empty())
			rv = nv = get_node(v);
		else
		{
			rv = evert(v); nv = get_node(v);
			enode *ta = rv->gmax();
			chg_iter(ta, rv, ta, nv);
			enode::join(rv, nv); rv->splay();
		}

		enode *fu = evert(u), *nu = get_node(u);
		reg_iter(u, v, fu, rv), reg_iter(v, u, nv, nu);
		enode *su = fu->c[1]->gmin();
		chg_iter(fu, su, nu, su);
		enode *pc1 = fu->c[1]; fu->cut(1);
		fu->sc(enode::join(enode::join(rv, nu), pc1), 1);
		fu->update();
	}
	inline void transfer_tag (enode *v, enode *nv) {
		// transfer v's tags to nv as v is to be sepereated from evert(v->tx);
		if (v == tagIter[v->tx]) {
			rep(d, 0, 1)
				if (v->tag[d]) nv->tag[d] += v->tag[d]; 

			nv->val = v->val;
			nv->update(); nv->splay();
			tagIter[v->tx] = nv;
		}
	}
	inline void cut (int u, int v) { ++cmdcnt2;
		assert(e[u].erase(v) && e[v].erase(u));
		assert(iter[u].count(v) && iter[v].count(u));
		evert(u, v);
		enode *x = iter[u][v].Px, *y = iter[u][v].Py,
			  *z = iter[v][u].Px, *w = iter[v][u].Py;
		// cut <x, y> and <z, w>, and remove x if w != x
		if (w == x) {
			assert(x->c[1]->gmin()->tx == v && x->c[1]->gmax()->tx == v);
			x->cut(1); 
		}
		else {
			w->splay(x);
			assert(w->c[0]->gmin()->tx == v && w->c[0]->gmax()->tx == v);
			w->cut(0);
			enode *z = w->c[1]->gmax();
			chg_iter(z, x, z, w);
			x->cut(1);
			transfer_tag(x, w);
		}
		if (y != z) { 
			y->splay(); z->splay(y);
			enode *w = z->gmin(); w->splay(z);
			chg_iter(y, w, z, w);
			y->cut(1);
			transfer_tag(y, z);
		}
		del_iter(u, v), del_iter(v, u);
		if (iter[u].empty()) tagIter[u] = 0;
		if (iter[v].empty()) tagIter[v] = 0;
	}
	inline void sett (int ty, int u, int v, int nv) {
		if (u > v) swap(u, v);
		if (nv == 0) {
			SE[ty][u].erase(v);
			SE[ty][v].erase(u);
		}
		else {
			SE[ty][u].insert(v);
			SE[ty][v].insert(u);
		}
		int d = nv - !nv;
		if (tagIter[u]) tagIter[u]->tag[ty] += d, tagIter[u]->update(), tagIter[u]->splay();
		if (tagIter[v]) tagIter[v]->tag[ty] += d, tagIter[v]->update(), tagIter[v]->splay();
	}
	inline void init (int _n) {
		n = _n;
	}
	inline void query (int x, LL &maxw, LL &sumw) {
		enode *t = evert(x);
		maxw = max(maxw, t->maxv);
		sumw += t->sumv;
	}
	inline void inc (int x, LL d) {
		W[x] += d;
		if (tagIter[x]) tagIter[x]->val = W[x], tagIter[x]->update(), tagIter[x]->splay();
	}

	int vis[N], cnt[N], deg[N];
	void dfs (int x, int ax, int co)
	{
		if (ax) deg[x] ++;
		cnt[co] ++;
		vis[x] = co;
		for (int y : e[x]) if (y != ax) dfs(y, x, co), ++deg[x];
	}
	void check ()
	{
		printf("\n  cmdcnt = %d ncnt = %d\n", cmdcnt, int(enode::cur - enode::T));
		fill(vis, vis + 1 + n, 0);
		fill(deg, deg + 1 + n, 0);
		for (int i = 1; i <= n; ++i)
		{
			if (vis[i]) continue;
			cnt[i] = 0;
			dfs(i, 0, i);
			vector<enode*> tmp{};
			if (cnt[i] == 1)
			{
				assert(iter[i].empty());
				continue;
			}
			enode *u = evert(i);
			//			enode::print(u); puts("");
			enode::dump(u, tmp);
			int last = i;
			assert(tmp[0]->tx == i && tmp.size() / 2 + 1 == cnt[i]);
			tmp.push_back(tmp[0]);
			for (int i = 1; i < tmp.size(); ++i)
			{
				assert(e[last].count(tmp[i]->tx));
				auto it = iter[last][tmp[i]->tx], jt = iter[tmp[i]->tx][last];
				assert(it.Px == tmp[i - 1] && it.Py == tmp[i] ||
						jt.Px == tmp[i] && jt.Py == tmp[i - 1]);
				last = tmp[i]->tx;
			}
		}
		// finger check (1)
		std::set<enode*> actived;
		for (int i = 1; i <= n; ++i)
		{
			assert(deg[i] == iter[i].size());
			for (auto& it : iter[i])
			{
				assert(it.Py.Px->tx == i && it.Py.Py->tx == it.Px);
				actived.insert(it.Py.Px);
				actived.insert(it.Py.Py);
			}
		}

		for (int i = 1; i <= n; ++i)
		{
			if (SE[0][i].empty() && SE[1][i].empty())
				assert(! tagIter[i] || (!tagIter[i]->tag[0] && !tagIter[i]->tag[1]));
			else
			{
				if (!tagIter[i])
					assert(iter[i].empty());
				else
				{
					assert(actived.count(tagIter[i]));
					assert(tagIter[i]->tag[0] == SE[0][i].size()
							&& tagIter[i]->tag[1] == SE[1][i].size());
				}
			}
		}
	}
};
// duplicate edge not supported (yet?)
struct DynamicGraph {
	static const int lgN = 16, G = 1, T = 0;

	struct hashPi {
		size_t operator() (const pair<int, int> &a) const {
			return ((LL)a.Px << 31LL) | a.Py;
		}
	};

	EulerTourTree ett[lgN + 1];
	unordered_map<pair<int, int>, int, hashPi> level;

	inline void inc (int v, LL d) {
		ett[lgN].inc(v, d);
	}
	inline void query (int v, LL &maxw, LL &sumw) {
		ett[lgN].query(v, maxw, sumw);
	}
	void init (int n) {
		rep(i, 0, lgN) ett[i].init(n);
	}
	void link (int u, int v) {
		++cmdcnt;
		if (u > v) swap(u, v);
		level[make_pair(u, v)] = lgN;
		if (ett[lgN].connected(u, v)) {
			ett[lgN].sett(G, u, v, 1);
		}
		else {
			ett[lgN].link(u, v);
			ett[lgN].sett(T, u, v, 1);
		}
	}
	int connected (int u, int v) {
		return ett[lgN].connected(u, v);
	}
	int pushG (int i, int v, int u) {
		for (auto it = ett[i].SE[G][v].begin(); it != ett[i].SE[G][v].end(); ) {
			auto nit = next(it);
			ett[i].sett(G, v, *it, 0);
			if (ett[i].connected(*it, u))
				return *it;

			int a = *it, b = v;
			if (a > b) swap(a, b);
			level[make_pair(a, b)] --;
			ett[i - 1].sett(G, a, b, 1);
			it = nit;
		}
		return 0;
	}
	void pushT (int i, int v) {
		for (auto it = ett[i].SE[T][v].begin(); it != ett[i].SE[T][v].end(); ) {
			auto nit = next(it);
			int a = *it, b = v;
			if (a > b) swap(a, b);
			ett[i].sett(T, a, b, 0);
			level[make_pair(a, b)] --;
			ett[i - 1].link(a, b);
			ett[i - 1].sett(T, a, b, 1);
			it = nit;
		}
	}
	void cut (int u, int v) {
		++cmdcnt;
		if (u > v) swap(u, v);
		int l = level.at(make_pair(u, v));
		level.erase(make_pair(u, v));

		if (!ett[l].SE[T][u].count(v)) {
			ett[l].sett(G, u, v, 0);
			return;
		}
		ett[l].sett(T, u, v, 0);
		for (int i = l; i <= lgN; ++i)
			ett[i].cut(u, v);

		for (int i = l; i <= lgN; ++i) {
			if (ett[i].iter[u].empty()) swap(u, v);
			int rx = 0, ry = 0;
			if (!ett[i].iter[u].empty() && !ett[i].iter[v].empty()) {
				enode *tu = ett[i].evert(u),
					  *tv = ett[i].evert(v);
				if (tv->s > tu->s)
					swap(u, v), swap(tu, tv);

				while (tv->sumt[T]) {
					enode *cur = tv;
					while (!cur->tag[T] || cur->c[0]->sumt[T]) 
						cur = cur->c[(cur->c[0]->sumt[T] == 0)];
					pushT(i, cur->tx);
					(tv = cur)->splay();
				}
				while (tv->sumt[G]) {
					enode *cur = tv;
					while (!cur->tag[G] || cur->c[0]->sumt[G]) 
						cur = cur->c[(cur->c[0]->sumt[G] == 0)];
					rx = cur->tx;
					if ((ry = pushG(i, cur->tx, u))) break;
					(tv = cur)->splay();
				}
			}
			else {
				rx = v;
				ry = pushG(i, v, u);
			}
			if (ry) {
				ett[i].sett(T, rx, ry, 1);
				for (int j = i; j <= lgN; ++j)
					ett[j].link(rx, ry);

				break;
			}
		}
	}
	void check () {
		rep(i, 0, lgN) ett[i].check();
		for (auto &it : level) {
			EulerTourTree &e = ett[it.second];
			auto rt = it.Px;
			assert(e.SE[G][rt.Px].count(rt.Py) ^ e.SE[T][rt.Px].count(rt.Py));
			assert(e.SE[G][rt.Py].count(rt.Px) ^ e.SE[T][rt.Py].count(rt.Px));
			assert(e.connected(rt.Px, rt.Py));
		}
	}
} DG;


int main () {
#ifdef LOCAL
	freopen("in", "r", stdin);
#endif
	int n, q;
	scanf("%d%d", &n, &q);
	enode::init();
	DG.init(n);
	rep(_, 1, q) {
		cmdcnt = _;
		char op[20]; int a, b;
		scanf("%s", op);
		if (op[0] == 'L') {
			scanf("%d%d", &a, &b);
			DG.link(a, b);
		}
		else if (op[0] == 'C') {
			scanf("%d%d", &a, &b);
			DG.cut(a, b);
		}
		if (_ % 100 == 0) DG.check();
	}
	return 0;
}

