struct node {
	int cnt, len;
	const char *s;
	node *sib, *pos;
	node () { ++__c; }
	node (int c, int l, const char *ss) { cnt = c; len = l; s = ss; sib = pos = 0; ++__c; }
} *root;

inline void addString (const char *s, int len) {
	node *r = root, *nr;
	for (int j = 0, k; j < len; ) {
	//	printf("%d\n", j);
		for (nr = r->pos; nr && nr->s[0] != s[j]; nr = nr->sib) ;
		if (nr) {
			for (k = 0; k < nr->len && j < len && nr->s[k] == s[j]; ++k, ++j) ;
			if (k == nr->len) { ++nr->cnt; r = nr; continue; }
			node *lc = new node(nr->cnt, nr->len - k, nr->s + k);
			++nr->cnt;
			nr->len = k;
			lc->pos = nr->pos;
			nr->pos = lc; r = nr;
		}
		if (j != len) {
			node *p = new node(1, len - j, s + j);
			p->sib = r->pos; r->pos = p;
		}
		break;
	}
}
