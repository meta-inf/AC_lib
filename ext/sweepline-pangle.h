
typedef pair<double, int> event;
const double eps = 1e-6, pi = acos(-1);
const int N = 512;

struct point { double x, y; } P[N];
vector<event> elist;
int n;

int dblcmp (double x) { return fabs(x) < eps ? 0 : x > 0 ? +1 : -1; }
double sqr (double x) { return x * x; }
double gdis (int a, int b) { return sqrt(sqr(P[a].x - P[b].x) + sqr(P[a].y - P[b].y)); }

double sr, r, x, y, dis[N][N], alfa[N][N];
int k;

bool checkPoint (int p)
{
	elist.clear();
	//fprintf(stderr, "%d\n", p);
	for (int i = 1; i <= n; ++i)
	{
		double d = gdis(i, p);
		if (i != p && d <= 2.0 * r)
		{
			double alpha = alfa[p][i],
				beta = acos(d / (2 * r));
			elist.push_back(event(alpha - beta, +1));
			elist.push_back(event(alpha - beta + pi * 2.0, +1));
			elist.push_back(event(alpha + beta, -1));
			elist.push_back(event(alpha + beta + pi * 2.0, -1));
		}
	}
	sort(elist.begin(), elist.end());
	int cur = 1;
	typeof(elist.begin()) iter = elist.begin();
	while (iter != elist.end())
	{
		double th = iter->first;
		for (; iter != elist.end() && iter->first == th; ++iter)
			cur += iter->second;
		if (cur >= k)
		{
			sr = r;
			x = r * cos(th) + P[p].x,
			y = r * sin(th) + P[p].y;
			return 1;
		}
	}
	return 0;
}
bool check (double _r) {
	r = _r;
	for (int i = 1; i <= n; ++i) if (checkPoint(i)) return 1;
	return 0;
}



