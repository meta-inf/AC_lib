// used when character set is small
// trie graph
struct node { bool fl; node *f, *n[26]; }
	buff[N << 1], *rt = buff, *cr = buff + 1;
inline node* new_node () { return cr++; }
int alpha;

void expand (node *c, char *s)
{
	if (!*s) { c->fl = true; return; }
	int p = *s - 'a';
	expand(c->n[p] ? c->n[p] : (c->n[p] = new_node()), s + 1);
}

void build_fail ()
{
	queue<node*> que;
	que.push(rt); rt->f = rt;
	while (!que.empty())
	{
		node *t = que.front(); que.pop();
		for (int i = 0; i < alpha; ++i)
		{
			node *f = (t == rt) ? t : t->f->n[i];
			if (t->n[i])
			{
				t->n[i]->f = f;
				t->n[i]->fl |= f->fl || t->fl; // ,b - proof
				que.push(t->n[i]);
			}
			else t->n[i] = f;
		}
	}
}

//traditional implementation
//when |SIGMA| is big
struct node
{
	int id, fl;
	node *f;
	map<int, node*> n;
} Tbuf[L << 2], *Tcur = Tbuf + 1, *root = Tbuf;

vector<int> same[N];
int head[N];

inline node* newNode () { return Tcur++; }

void expand (node *t, int *s, int id)
{
	if (*s == -1)
	{
		if (!t->id) t->id = id, t->fl = 1;
		same[t->id].push_back(id);
		head[id] = t->id;
		return;
	}
	if (!t->n.count(*s)) t->n[*s] = newNode();
	expand(t->n[*s], s + 1, id);
}

node* next (node *t, int w)
{
	while (t != root && !t->n.count(w)) t = t->f;
	return t->n.count(w) ? t->n[w] : t;
}

void build_fail ()
{
	queue<node*> que;
	que.push(root); root->f = root;
	while (!que.empty())
	{
		node *t = que.front(); que.pop();
		for_(it, (t->n))
		{
			node *nt = (t == root) ? t : next(t->f, it->first);
			it->second->f = nt; it->second->fl |= nt->fl;
			que.push(it->second);
		}
	}
}

