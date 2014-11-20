//@ Blossom Algorithm
/*
 * 召唤友军支援 ... (
 * */

#include <cstdio>
#include <queue>

using namespace std;

const int maxn=230;

struct edge {
	int v;
	edge *next;
	edge(int _v,edge *_next)
	: v(_v),next(_next) {}
};

int n,spouse[maxn],fa[maxn],mark[maxn],link[maxn];
edge *E[maxn];
queue<int> Q;

int getfa(int i)
{
	return fa[i]==i? i: fa[i]=getfa(fa[i]);
}

void merge(int a,int b)
{
	a=getfa(a);  b=getfa(b);
	if (a!=b)  fa[a]=b;
}

void aug(int i)
{
	for(int j,k; i; i=k) {
		j=link[i], k=spouse[j];
		spouse[i]=j;  spouse[j]=i;
	}
}

int lca(int a,int b)
{
	static bool vis[maxn];
	for(int i=1; i<=n; vis[i++]=false);
	for(vis[a=getfa(a)]=true; spouse[a]; vis[a]=true)
		a=getfa(link[spouse[a]]);
	for(b=getfa(b); !vis[b]; b=getfa(link[spouse[b]]));
	return b;
}

void shrink(int a,int p)
{
	for(int b,c; getfa(a)!=p; a=c) {
		b=spouse[a];  c=link[b];
		if(getfa(c)!=p)  link[c]=b;
		mark[b]=0;  Q.push(b);
		merge(a,b);  merge(b,c);
	}
}

void findpath(int s)
{
	for(int i=1;i<=n;++i)  
		fa[i]=i, mark[i]=-1, link[i]=0;
	while (!Q.empty())  Q.pop();
	for(Q.push(s); !Q.empty() && !spouse[s]; Q.pop()) {
		int a=Q.front(); 
		for(edge *e=E[a]; e; e=e->next) {
			int b=e->v;
			if (getfa(a)!=getfa(b) && mark[b]!=1) {
				if (mark[b]==-1) {
					mark[b]=1;
					link[b]=a;
					if (!spouse[b]) {
						aug(b);
						break;
					} else {
						mark[spouse[b]]=0;
						Q.push(spouse[b]);
					}
				} else {
					int p=lca(a,b);
					if(getfa(a)!=p)  link[a]=b;  
					if(getfa(b)!=p)  link[b]=a;
					shrink(a,p);  shrink(b,p);
				}
			}
		}
	}
}

int main()
{
	int a,b;
	scanf("%d",&n);
	while(scanf("%d%d",&a,&b)!=EOF) {
		E[a]=new edge(b,E[a]);
		E[b]=new edge(a,E[b]);
	}
	for(int i=1;i<=n;++i)
		if (!spouse[i]) findpath(i);

	int ret=0;
	for(int i=1;i<=n;++i)
		if(spouse[i])  ++ret;
	printf("%d\n",ret);
	for(int i=1;i<=n;++i)
		if(spouse[i]) {
			printf("%d %d\n",i, spouse[i]);
			spouse[spouse[i]]=0;
		}
	return 0;
}


