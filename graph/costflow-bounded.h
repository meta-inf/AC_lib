/*
 * Calculate feasible flow with edges having lower bounds.
 * No sink or source included. add (sink, source, 0, inf) to fix.
 * The mcFlow part used zkw algorithm. Keep in mind its bugs.
 * Last Edit : before 2013
 * */
namespace mcFlow
{
    const int N = 2048, M = 1048576;
    struct edge {
        int t, of, f, c;
        edge *n, *op;
    } ebf[M], *ecur = ebf, *e[N];
    int dis[N], vis[N], vn, s, t, cost, flow;
    int modlabel ()
    {
        int delta = inf;
        for (int i = 1; i <= vn; ++i) if (vis[i])
            for (edge *it = e[i]; it; it = it->n)
                if (!vis[it->t] && it->f)
                    delta = min(delta, dis[it->t] + it->c - dis[i]);
        if (delta == inf) return 1;
        for (int i = 1; i <= vn; ++i)
            if (vis[i]) dis[i] += delta;
        return 0;
    }
    int augument (int x, int f)
    {
        if (x == t) { flow += f, cost += dis[s] * f; return f; }
        int r(f), de;
        vis[x] = true;
        for (edge *it = e[x]; it; it = it->n)
            if (!vis[it->t] && dis[x] == dis[it->t] + it->c && it->f)
            {
                de = augument(it->t, min(it->f, r));
                r -= de, it->f -= de, it->op->f += de;
                if (!r) break;
            }
        return f - r;
    }
    inline void solve ()
    {
        do
            do memset(vis, 0, sizeof(vis[0]) * (vn + 1));
            while (augument(s, inf));
        while (!modlabel());
    }
    inline edge* add (int s, int t, int f, int c)
    {
        //fprintf(stderr, "\t--MCFLOW--%d->%d F %d C %d\n", s, t, f, c);
        *ecur = (edge){t, f, f, c,  e[s], ecur + 1}; e[s] = ecur++;
        *ecur = (edge){s, 0, 0, -c, e[t], ecur - 1}; e[t] = ecur++;
        return ecur - 2;
    }
    inline int new_node () { return ++vn; }
    inline int set_src () { return s = new_node(); }
    inline int set_dst () { return t = new_node(); }
}
 
namespace acf
{
    const int N = 2048;
    int delt[N], preRes;
    inline int new_node () { return mcFlow::new_node(); }
    inline mcFlow::edge* add (int x, int y, int mn, int mx, int c)
    {
        delt[x] -= mn, delt[y] += mn;
        preRes += mn * c;
        return mcFlow::add(x, y, mx - mn, c);
    }
    inline int solve ()
    {
        int s = mcFlow::set_src(), t = mcFlow::set_dst();
        int supposed = 0;
        for (int i = 1; i <= mcFlow::vn - 2; ++i)
            if (delt[i] > 0) mcFlow::add(s, i, delt[i], 0, 0x0),
                supposed += delt[i];
            else if (delt[i] < 0) mcFlow::add(i, t, -delt[i], 0, 0x0);
        mcFlow::solve();
        if (mcFlow::flow != supposed) Throw("ACF Failed");
        return mcFlow::cost;
    }
}
