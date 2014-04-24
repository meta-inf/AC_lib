#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <algorithm>
using namespace std;

const int N = 64, ITER_T = 2048;
const double alpha = 0.86, I_TEMP = 8000, eps = 1e-4;

int type[N], ti[N][2], cn, n;

struct state {
	int list[N], next[N], pre[N], tag[N];
	inline bool valid_swap (int l, int r)
	{
		if (l == r) return false;
		if (type[list[l]] == 2 || r < next[l])
			if (type[list[r]] == 2 || pre[r] < l)
				return 1;
		return 0;
	}
	void adjust ()
	{
		int l(0), r(0);
		while (!valid_swap(l, r))
		{
			l = rand() % cn + 1, r = rand() % cn + 1;
			if (l > r) swap(l, r);
		}
		next[pre[l]] = pre[next[l]] = r;
		next[pre[r]] = pre[next[r]] = l;
		swap(pre[l], pre[r]), swap(next[l], next[r]);
		swap(list[l], list[r]), swap(tag[l], tag[r]);
	}
	int evaluate ()
	{
		int ti[N], cnt[2] = {0, 0};
		memset(ti, 0, sizeof ti);
		for (int i = 1; i <= cn; ++i)
			cnt[tag[i]] = max(cnt[tag[i]], ti[pre[i]]) + ::ti[list[i]][tag[i]],
			ti[i] = cnt[tag[i]];
		return max(cnt[0], cnt[1]);
	}
	void init ()
	{
		list[0] = 0;
		for (int i = 1; i <= n; ++i)
		{
			list[i] = i;
			tag[i] = type[i] == 1 ? 1 : 0;
			pre[i] = 0, next[i] = i + n;
			list[i + n] = i;
			tag[i + n] = type[i] == 1 ? 0 : 1;
			pre[i + n] = i, next[i + n] = n * 2 + 1;
		}
		list[n * 2 + 1] = n + 1;
	}
	state& operator= (const state &b) {
		memcpy(list, b.list, sizeof(list));
		memcpy(next, b.next, sizeof(next));
		memcpy(pre, b.pre, sizeof(pre));
		memcpy(tag, b.tag, sizeof(tag));
	}
} pre, cur;

inline double rand_real () {
	return double(rand()) / double(RAND_MAX);
}
inline bool adjustable (double t, double tis, int de) {
	return de < 0 || (rand_real() < exp(-de / (t + (tis / ITER_T * (alpha - 1) * t))));
}

int sa ()
{
	srand(time(0));
	cur.init();
	int res = cur.evaluate(), e0 = res, e1;
	for (double t = I_TEMP; t > eps; t *= alpha)
	{
		for (int i = 1; i <= ITER_T; ++i)
		{
			pre = cur;
			cur.adjust();
			res = min(res, e1 = cur.evaluate());
			if (adjustable(t, i, e1 - e0)) e0 = e1;
			else cur = pre;
		}
	}
	return res;
}

int main ()
{
	freopen("in", "r", stdin);
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i)
		scanf("%d%d%d", type + i, &ti[i][0], &ti[i][1]),
		--type[i];
	ti[0][0] = ti[0][1] = ti[n + 1][0] = ti[n + 1][1] = 0;
	cn = n << 1;
	printf("%d\n", sa());
	return 0;
}
