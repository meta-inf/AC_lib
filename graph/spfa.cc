namespace SPFA {
    const int V = 100000;
    struct enode { int t; lint w; };
    vector<enode> e[V];
	bool inque[V];
    queue<int> que;
    int ln, pre[V];
    
    inline void addEdge (int a, int b, int c)
    {
        e[a].push_back((enode){b, c});
        e[b].push_back((enode){a, c});
    }
 
    void reset ()
    { for (int i = 0; i <= ln; ++i) e[i].clear(); }
    
    void solve (int n, int s, int t)
    {
        ln = n;
        fill(dis + 1, dis + n + 1, inf64);
        fill(inque + 1, inque + n + 1, 0);
        dis[s] = 0; inque[s] = 1; que.push(s);
        while (!que.empty())
        {
            int x = que.front();
            inque[x] = false; que.pop();
            for (vector<enode>::iterator it = e[x].begin(); it != e[x].end(); ++it)
                if (dis[x] + it->w < dis[it->t])
                {
                    dis[it->t] = dis[x] + it->w; pre[it->t] = x;
                    if (!inque[it->t]) inque[it->t] = 1, que.push(it->t);
                }
        }
    }
}
