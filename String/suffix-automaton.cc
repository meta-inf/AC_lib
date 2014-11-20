//@ Suffix Automaton

const int SIGMA = 26, N = 500000;
struct node { node *f, *n[26]; int l, res, cur; }
  buff[N], *cur = buff, *rt, *last;
node *list[N];

//expand (ch) : add a new char ch to the string being maintained.
void expand (int ch)
{
	node *p = last, *q = cur++;
	q->l = q->res = last->l + 1, last = q;
	for (; p && !p->n[ch]; p = p->f) p->n[ch] = q;
	if (!p) q->f = rt;
	else // remove the conditional statements with 'q->f = p->n[ch]' and get a 
		 // Factor Oracle.
		if (p->n[ch]->l == p->l + 1) q->f = p->n[ch]; 
		else
		{
			node *u = p->n[ch], *v = cur++;
			memcpy(v->n, u->n, sizeof(u->n)); v->f = u->f;
			v->l = v->res = p->l + 1, q->f = u->f = v;
			for (; p && p->n[ch] == u; p = p->f) p->n[ch] = v;
		}
}

