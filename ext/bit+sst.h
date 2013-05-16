tree l[TS], r[TS];
int s[TS], key[TS], tc;

inline void l_rot (int &t)
{
	int k = r[t]; r[t] = l[k]; l[k] = t;
	s[t] = s[l[t]] + s[r[t]] + 1;
	s[k] = s[l[k]] + s[r[k]] + 1;
	t = k;
}
inline void r_rot (int &t)
{
	int k = l[t]; l[t] = r[k]; r[k] = t;
	s[t] = s[l[t]] + s[r[t]] + 1;
	s[k] = s[l[k]] + s[r[k]] + 1;
	t = k;
}
inline void balance (int &t)
{
	if (s[l[t]] > (s[r[t]] << 1) + 1) r_rot(t);
	if (s[r[t]] > (s[l[t]] << 1) + 1) l_rot(t);
}
inline void insert (int &t, int x)
{
	if (!t) key[t = ++tc] = x, ++s[t];
	else
	{
		insert(x < key[t] ? l[t] : r[t], x);
		++s[t]; balance(t);
	}
}
inline int remove (int &t, int x)
{
	--s[t];
	if (key[t] == x || x < key[t] && !l[t] || x > key[t] && !r[t])
	{
		int re = key[t];
		if (!l[t] || !r[t]) t = l[t] + r[t];
		else key[t] = remove(l[t], x + 1);
		return re;
	}
	else return remove(x < key[t] ? l[t] : r[t], x);
}
inline int rank (int t, int x)
{ // count(t : key[t] <= x)
	if (!t) return 0;
	if (x < key[t]) return rank(l[t], x);
	else return s[l[t]] + rank(r[t], x) + 1;
}
//~SST

//BIT
tree P[LS], N[LS];
int ls;

inline int lowbit (int x) { return x & -x; }

inline void add (tree *B, int p, int x) {
	for (; p <= ls; p += lowbit(p)) insert(B[p], x);
}
inline void replace (tree *B, int p, int x, int nx) {
	for (; p <= ls; p += lowbit(p))
		{ remove(B[p], x); insert(B[p], nx); }
}
inline int grank (int p, int x) {
	int res = 0;
	for (; p; p -= lowbit(p))
		res += rank(P[p], x) - rank(N[p], x);
	return res;
}

