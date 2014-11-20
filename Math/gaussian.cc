//@ Gaussian Elimination

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

const int N = 128;

double buff[N][N], *a[N], sol[N];
int n;

void gauss ()
{
	for (int i = 1; i <= n; ++i) a[i] = buff[i];
	for (int i = 1; i <= n; ++i)
	{
		double cur = 0; int k(i);
		for (int j = i; j <= n; ++j)
			if (fabs(a[j][i]) > cur)
				{ cur = fabs(a[k = j][i]); break; }
		swap(a[k], a[i]);
		for (int j = i + 1; j <= n; ++j)
		{
			double d = -a[j][i] / a[i][i];
			for (int k = i; k <= n + 1; ++k)
				a[j][k] += d * a[i][k];
		}
	}
}
void evaluate ()
{
	for (int i = n; i > 0; --i)
	{
		sol[i] = a[i][n + 1] / a[i][i];
		for (int j = i - 1; j > 0; --j)
			a[j][n + 1] -= sol[i] * a[j][i], a[j][i] = 0.0;
	}
}

int main ()
{
	ios::sync_with_stdio(false);
	cin >> n; 
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n + 1; ++j)
			cin >> buff[i][j];
	gauss();
	evaluate();
	for (int i = 1; i <= n; ++i)
		cout << int(sol[i] + 0.5) << ' ';
	cout << endl;
	return 0;
}

