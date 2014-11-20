//@ Simulated Annealing

const int N = 32, T = 1000;

//...

struct state { double a, b; } cur[N];

double P, Q, R;
inline double evaluate (state &s) { } // evalutae the state
inline bool legal (state &s) { } // if the state is legal
inline double rand1 () { return (double(rand() * 2) / RAND_MAX) - 1; }
inline double rand2 () { return double(rand()) / RAND_MAX; }

int main ()
{
	// Init
	for (int i = 0; i < N; ++i)
	{
		cur[i] = (state){rand2(), rand2()};
		res = min(res, evaluate(cur[i]));
	}
	for (double e = 1.0; e > 1e-10; e /= 2.0)
	{
		for (int i = 0; i < N; ++i)
		{
			double cv = evaluate(cur[i]), cr;
			for (int t = T; t > 0; --t)
			{
				state tmp = cur[i];
				tmp.a += rand1() * e, tmp.b += rand1() * e;
				if (legal(tmp))
					if ((cr = evaluate(tmp)) < cv)
						cv = cr, cur[i] = tmp;
					else if (rand2() < 1e-4) cur[i] = tmp;
			}
			res = min(res, cv);
		}
	}
	printf("%.2lf\n", res);
	return 0;
}

