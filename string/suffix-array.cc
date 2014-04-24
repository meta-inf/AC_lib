/*
	Suffix Array - Doubling Algorithm
	Tested [ Correct ]
	it is strongly recommended to write a brute force program to check the code in exams
*/
int r[N], sa[N], rank[N], height[N];
int wa[N], wb[N], ws[N], wv[N];
inline int cmp (int *r, int x, int y, int l) {
	return r[x] == r[y] && r[x + l] == r[y + l];
}
//call da(r, length(r) + 1, max(r[i]) + 1)
//r is 0-based, r[length(r)] = 0, r[0,length(r)) > 0
//the output is 1-based since suffix(length(r)) is included
void da (int *r, int n, int m)
{
	int i, j, p, *x = wa, *y = wb;
	for (i = 0; i < m; ++i) ws[i] = 0;
	for (i = 0; i < n; ++i) ws[x[i] = r[i]]++;
	for (i = 1; i < m; ++i) ws[i] += ws[i - 1];
	//downto is for stability concern when elements duplicate
	for (i = n - 1; i >= 0; --i) sa[--ws[x[i]]] = i;
	for (j = 1, p = 1; p < n; j <<= 1, m = p)
	{
		//sort by (rank[i],rank[i+j])
		//second
		for (i = n - j, p = 0; i < n; ++i) y[p++] = i; //second == 0, in arbitary order
		for (i = 0; i < n; ++i) if (sa[i] >= j) y[p++] = sa[i] - j; // sa[i]-j : (*,i)
		//first
		for (i = 0; i < n; ++i) wv[i] = x[y[i]];
		for (i = 0; i < m; ++i) ws[i] = 0;
		for (i = 0; i < n; ++i) ws[wv[i]]++;
		for (i = 1; i < m; ++i) ws[i] += ws[i - 1];
		for (i = n - 1; i >= 0; --i) sa[--ws[wv[i]]] = y[i]; //rvalue is not i : stability concern
		//store the current rank in x
		for (swap(x, y), p = i = 1, x[sa[0]] = 0; i < n; ++i)
			x[sa[i]] = cmp(y, sa[i - 1], sa[i], j) ? p - 1 : p++;
	}
}
// call calheight(r, length(r))
// LCP(i, SA[Rank[i] - 1]) >= LCP(i - 1, SA[Rank[i - 1] - 1]) - 1
void calheight (int *r, int n)
{
	int i, j, k = 0;
	for (i = 1; i <= n; ++i) rank[sa[i]] = i;
	for (i = 0; i < n; height[rank[i++]] = k)
		for (k ? --k : 0, j = sa[rank[i] - 1]; r[i + k] == r[j + k]; ++k) ;
}

/*
uasge:
scanf("%s", r);
if (r has zero) for each i in r do i<-i+1
r[n=length(r)]=0
m=max(r[i])
da(r, n + 1, m + 1);
*/
