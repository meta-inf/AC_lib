//@ Planar Graph & Point Location
/*
 * Construction of dual graph for planar graph and point location
 * Work for connected graph only
 * Last Edit : Feb 2013
 * */

typedef long long LL;
 
struct point {
    int x, y;
    point (int _x = 0, int _y = 0) : x(_x), y(_y) {}
    point operator+ (const point &b) const { return point(x + b.x, y + b.y); }
    point operator- (const point &b) const { return point(x - b.x, y - b.y); }
    LL operator* (const point &b) const { return (LL)x * b.y - (LL)y * b.x; }
    LL operator^ (const point &b) const { return (LL)x * b.x + (LL)y * b.y; }
    bool operator== (const point &b) const { return x == b.x && y == b.y; }
    double abs () { return hypot(x, y); }
    double angle () { return atan2(y, x); }
};
inline int sgn (LL x) { return x > 0 ? 1 : x == 0 ? 0 : -1; }


namespace TREAP {
    template <class valtype>
    struct treap {
        static int tot_cnt;
        treap *lc, *rc;
        int s, w;
        const valtype &v;
 
        static int gsize (treap *x) { return x ? x->s : 0; }
        void update () { s = treap::gsize(lc) + treap::gsize(rc) + 1; }
 
        treap (const valtype &nv, int nw, treap *l, treap *r)
            : v(nv), w(nw), lc(l), rc(r) { update(); ++tot_cnt; }
 
        static treap* newLeaf (valtype &v) { return new treap(v, rand(), 0, 0); }
        static treap *splitL (treap *x, valtype &v) {
            if (!x) return x;
            if (x->v >= v) return splitL(x->lc, v);
            return new treap(x->v, x->w, x->lc, splitL(x->rc, v));
        }
        static treap *splitR (treap *x, valtype &v) {
            if (!x) return x;
            if ((x->v <= v) ^ (x->v < v || x->v == v))
                throw;
            if (x->v <= v) return splitR(x->rc, v);
            return new treap(x->v, x->w, splitR(x->lc, v), x->rc);
        }
        static treap *merge (treap *l, treap *r) { // max(l) < min(r)
            if (!l || !r) return l ? l : r;
            if (l->w < r->w)
                return new treap(l->v, l->w, l->lc, merge(l->rc, r));
            else
                return new treap(r->v, r->w, merge(l, r->lc), r->rc);
        }
        static treap *insert (treap *x, valtype &v) {
            treap *a = splitL(x, v), *b = splitR(x, v);
            return merge(a, merge(newLeaf(v), b));
            return merge(splitL(x, v), merge(newLeaf(v), splitR(x, v)));
        }
        static treap *erase (treap *x, valtype &v) {
            return merge(splitL(x, v), splitR(x, v));
        }
        const treap* lower_bound (valtype &x) const {
            if (v == x) return this;
            if (x < v) {
                const treap *res;
                if (lc) res = lc->lower_bound(x); else res = this;
                if (!res) res = this;
                return res;
            }
            else {
                if (rc) return rc->lower_bound(x);
                else return 0x0;
            }
        }
        void printTree (int dp, int ty) {
            for (int i = 0; i < dp; ++i) putchar(' ');
            printf("Ty %d %d %d ", ty, s, w);
            v.output(); puts("");
            if (lc) { lc->printTree(dp + 1, 0);  }
            if (rc) { rc->printTree(dp + 1, 1);  }
        }
    };
    template <class T> int treap<T>::tot_cnt = 0;
}
 
// Dual graph & Point locating. Intereact with STDIO
namespace GRAPH {
    const int N = 300050, inf = ~0u >> 2;
    point P[N];
    struct edge { int s, t, w, id; double ang; edge *d, *n, *p; } edges[N];
    void add_edge (int s, int t, int w) {
        static edge* ec = edges + 1;
        *ec = (edge){s, t, w, 0, (P[t] - P[s]).angle(), ec + 1, 0x0, 0x0}; ec++;
        *ec = (edge){t, s, w, 0, (P[s] - P[t]).angle(), ec - 1, 0x0, 0x0}; ec++;
    }
 
    int n, m;
    namespace DUAL { // dual graph construction
        vector<edge*> e[N];
        int outer, cid; // outer region; number of regions
        bool e_comp (const edge *a, const edge *b) { return a->ang < b->ang; }
        void init () {
            for (int i = 1, ie = m * 2; i <= ie; ++i)
                e[edges[i].s].push_back(edges + i);
            for (int i = 1; i <= n; ++i) {
                sort(e[i].begin(), e[i].end(), e_comp);
                for (__typeof(e[i].begin())it = e[i].begin(); it != e[i].end(); ++it) {
                    if (it == e[i].begin()) (*it)->p = *e[i].rbegin(); else (*it)->p = it[-1];
                    if (it + 1 == e[i].end()) (*it)->n = *e[i].begin(); else (*it)->n = it[1];
                }
            }
            for (int i = 1, ie = m * 2; i <= ie; ++i) {
                if (edges[i].id) continue;
                int nid = ++cid;
                LL area = 0;
                for (edge *e = edges + i; !e->id; e = e->d->p) {
//                  printf("%d->%d\t", e->s, e->t);
                    area += P[e->t] * P[e->s];
                    e->id = nid;
                }
//              puts("\n----------------------------");
                if (area > 0) {
                    if (outer > 0) throw;
                    outer = nid;
                }
            }
            for (int i = 1, ie = m * 2; i <= ie; ++i) {
                edge *cur = edges + i;
                if (cur->id == outer || cur->d->id == outer)
                    cur->w = inf;
                if (cur->d > cur)
                    MST::add_edge(cur->id, cur->d->id, cur->w);
            }
        }
    }
    namespace LOCATOR { // Solve the point location problem. guaranteed that no point lies on borders.
        using namespace TREAP;
        struct tnode {
            point s, e; int id;
            void output () { printf(" (%d,%d)->(%d,%d) %d ", s.x, s.y, e.x, e.y, id); }
            tnode (point a,  point b, int i) : s(a), e(b), id(i) {}
            tnode () {}
            bool operator< (const tnode &b) const {
                if (s == e) return !(b < *this); else
                {
                    int f1 = sgn((b.s - s) * (e - s)), f2 = sgn((b.e - s) * (e - s));
                    if (f1 == 0 && f2 == 0) return false;
                    if (f1 >= 0 && f2 >= 0) return true;
                    if (f1 <= 0 && f2 <= 0) return false;
                    return !(b < *this);
                }
            }
            bool operator== (const tnode &b) const {
                return s == b.s && e == b.e;
            }
            bool operator<= (const tnode &b) const {
                return !(b < *this);
            }
            bool operator>= (const tnode &b) const {
                return !(*this < b);
            }
            bool operator> (const tnode &b) const {
                return b < *this;
            }
        };
        struct tnode_bufferpool {
            tnode bb[2000000];
            tnode& new_tnode (point a, point b, int c) {
                static int top = 0;
                bb[top] = tnode(a, b, c);
                return bb[top++];
            }
        } tpool;
        typedef treap<tnode> tree;
        map<int, tree*> tMap;
 
        struct event {
            tnode x; int ti, ty;
            bool operator< (const event &b) const {
                if (ti == b.ti) return ty < b.ty; else return ti < b.ti;
            }
            event (tnode b, int i, int y) : x(b), ti(i), ty(y) {}
        };
        vector<event> tbuff;
 
        void init () {
            tbuff.reserve(m * 2);
            for (int i = 1, ie = m * 2; i <= ie; ++i) {
                edge &c = edges[i];
                if (P[c.s].x >= P[c.t].x) continue;
                tnode &tcur = tpool.new_tnode(P[c.s], P[c.t], c.id);
                tbuff.push_back(event(tcur, P[c.t].x, 0)); // REMOVE
                tbuff.push_back(event(tcur, P[c.s].x, 1)); // INSERT
            }
            sort(tbuff.begin(), tbuff.end());
            tree *cur = 0x0;
            for (__typeof(tbuff.begin())it = tbuff.begin(); it != tbuff.end(); ++it) {
                if (it->ty == 0) // remove
                    cur = tree::erase(cur, it->x);
                else // insert
                    cur = tree::insert(cur, it->x);
//              if (cur) fprintf(stderr, "%d\n", cur->s);
                if (it + 1 == tbuff.end() || it[1].ti != it[0].ti) // save status
                    tMap[it->ti] = cur;
            }
            /*for (__typeof(tMap.begin())it = tMap.begin(); it != tMap.end(); ++it) {
                printf("X Coordinate = %d\t", it->first);
                it->second ? it->second->printTree(0, 0) : void();
                puts("---------------------------------------");
            }*/
        }
        int query (point x) {
            __typeof(tMap.begin()) it = tMap.upper_bound(x.x);
            if (it == tMap.begin()) return -1;
            --it;
            const tree *xcur = it->second;
            if (!xcur) return -1;
            const tree *ycur = xcur->lower_bound(tpool.new_tnode(x, x, 0));
            return ycur ? ycur->v.id : -1;
        }
    }
     
    void solve () {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; ++i) {
            scanf("%d%d", &P[i].x, &P[i].y);
            P[i].x *= 2;
            P[i].y *= 2;
        }
        for (int i = 1; i <= m; ++i) {
            int a, b, w;
            scanf("%d%d%d", &a, &b, &w);
            add_edge(a, b, w);
        }
        DUAL::init();
        LOCATOR::init();
        MST::solve(DUAL::cid, m);
        int q;
        scanf("%d", &q);
        for (int i = 0; i < q; ++i) {
            double x1, x2, y1, y2;
            scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
            int a = LOCATOR::query(point(int(x1 * 2), int(y1 * 2))),
                b = LOCATOR::query(point(int(x2 * 2), int(y2 * 2)));
//          fprintf(stderr, "Point 1 %d Point 2 %d\n", a, b);
            if (a == -1 || a == DUAL::outer || b == -1 || b == DUAL::outer) {
                puts("-1");
                continue;
            }
            printf("%d\n", TREE::getMax(a, b));
        }
    }
}
