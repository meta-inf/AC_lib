
/*
 * Full-featured unsigned integer class by kAc
 * stQQQQQQQQQQQQQ
 * Has *never* tested by me. Seems that some reference uses may be dangerous ?
 * The division operation is O(Nlog^2N); other operations are all linear or sublinear.
 * */
#define MOD 10000
#define brute_limit 0
#define max 30000
typedef std::complex<double> Complex;
Complex *w[7], *stack, *e;
int *tmp, maxn;
const double pi = 3.14159265358979324;
struct mat;
class Biguint
{
public:
	template <class T> Biguint (T x = 0, int N = max) {
		n = N;
		d = new int[n];
		f = n - 1, Initialize();
		for (; x; x /= MOD, f++) d[f] = x % MOD;
		if (f) f--;
	}
	Biguint() {
		n = max;
		d = new int[n];
		f = n - 1, Initialize();
	}
	~Biguint() {
		delete d;
	}
	void Initialize() {
		for (int i = 0; i <= f; i++) d[i] = 0;
		f = 0;
	}
	int operator [] (int i) const {
		return d[i];
	}
	int& operator [] (int i) {
		return d[i];
	}
	Biguint& operator = (const Biguint &x) {
		for (int i = 0; i <= f || i <= x.f; i++)
			if (i < x.n) d[i] = x.d[i];
			else d[i] = 0;
		f = x.f;
		return *this;
	}
	friend int operator == (const Biguint &x, const Biguint &y) {
		return Abseq (x, y);
	}
	friend int operator != (const Biguint &x, const Biguint &y) {
		return ! (x == y);
	}
	friend int operator < (const Biguint &x, const Biguint &y) {
		return Absless (x, y);
	}
	friend int operator > (const Biguint &x, const Biguint &y) {
		return y < x;
	}
	friend int operator <= (const Biguint &x, const Biguint &y) {
		return ! (y < x);
	}
	friend int operator >= (const Biguint &x, const Biguint &y) {
		return ! (x < y);
	}
	Biguint& operator += (const Biguint &x) {
		Absaddup (*this, x);
		return *this;
	}
	friend Biguint& operator + (const Biguint &x, const Biguint &y) {
		static Biguint tmp (0, max + 10);
		tmp = x;
		tmp += y;
		return tmp;
	}
	Biguint& operator -= (const Biguint &x) {
		Absminusup (*this, x);
		return *this;
	}
	friend Biguint& operator - (const Biguint &x, const Biguint &y) {
		static Biguint tmp (0, max + 10);
		tmp = x;
		tmp -= y;
		return tmp;
	}
	template <class T> Biguint& operator *= (T x) {
		Absmul (*this, x);
		return *this;
	}
	template <class T> friend Biguint& operator * (const Biguint &x, T y) {
		static Biguint tmp (0, 2 * max + 10);
		tmp = x;
		tmp *= y;
		return tmp;
	}
	template <class T> friend Biguint& operator * (T x, const Biguint &y) {
		static Biguint tmp (0, 2 * max + 10);
		tmp = y;
		tmp *= x;
		return tmp;
	}
	friend Biguint& operator * (const Biguint &x, const Biguint &y) {
		static Biguint tmp (0, max + 10);
		Absmul (x, y, tmp);
		return tmp;
	}
	Biguint& operator *= (const Biguint &x) {
		*this = (*this) * x;
		return *this;
	}
	template <class T> static void Divide (const Biguint &x, T y, Biguint &div, T &mod) {
		Absdiv (x, y, div, mod);
	}
	template <class T> Biguint& operator /= (T y) {
		static Biguint div (0, max + 10);
		T mod;
		Divide (*this, y, div, mod);
		*this = div;
		return *this;
	}
	template <class T> friend Biguint& operator / (const Biguint &x, T y) {
		static Biguint div (0, max + 10);
		T mod;
		Divide (x, y, div, mod);
		return div;
	}
	template <class T> friend T operator % (const Biguint &x, T y) {
		static Biguint div (0, max + 10);
		T mod;
		Divide (x, y, div, mod);
		return mod;
	}
	static void Divide (const Biguint &x, const Biguint &y, Biguint &div, Biguint &mod) {
		Absdivide (x, y, div, mod);
	}
	Biguint& operator /= (const Biguint &y) {
		static Biguint div (0, max + 10), mod (0, max + 10);
		Divide (*this, y, div, mod);
		*this = div;
		return *this;
	}
	Biguint& operator %= (const Biguint &y) {
		static Biguint div (0, max + 10), mod (0, max + 10);
		Divide (*this, y, div, mod);
		*this = mod;
		return *this;
	}
	friend Biguint& operator / (const Biguint &x, const Biguint &y) {
		static Biguint div (0, max + 10), mod (0, max + 10);
		Divide (x, y, div, mod);
		return div;
	}
	friend Biguint& operator % (const Biguint &x, const Biguint &y) {
		static Biguint div (0, max + 10), mod (0, max + 10);
		Divide (x, y, div, mod);
		return mod;
	}
	friend void swap (Biguint &a, Biguint &b);
	void Print() {
		Absprint();
	}
	int n, *d, f;
	static int null (const Biguint &x) {
		return !x.f && !x.d[x.f];
	}
	static int Abseq (const Biguint &x, const Biguint &y) {
		if (x.f != y.f) return 0;
		for (int i = x.f; i >= 0; i--) if (x[i] != y[i]) return 0;
		return 1;
	}
	static int Absless (const Biguint &x, const Biguint &y) {
		if (x.f < y.f) return 1;
		else if (x.f > y.f) return 0;
		for (int i = x.f; i >= 0; i--) if (x[i] < y[i]) return 1;
			else if (x[i] > y[i]) return 0;
		return 0;
	}
	static void Absaddup (Biguint &x, const Biguint &y) {
		int f = x.f;
		if (f < y.f) f = y.f;
		f++;
		for (int i = 0, g = 0; i <= f; i++) x[i] += y[i] + g, x[i] >= MOD ? (g = 1, x[i] -= MOD) : g = 0;
		for (; f && !x[f]; f--);
		x.f = f;
	}
	static void Absminusup (Biguint &x, const Biguint &y) {
		int f = x.f;
		for (int i = 0, g = 0; i <= f; i++) x[i] -= y[i] + g, x[i] < 0 ? (g = 1, x[i] += MOD) : g = 0;
		for (; f && !x[f]; f--);
		x.f = f;
	}
	template <class T> static void Absmul (Biguint &x, T y) {
		T s;
		int f = 0;
		for (int g = 0; f <= x.f || g; f++) s = x[f] * y + g, g = s / MOD, x[f] = s - g * MOD;
		f--;
		for (; f && !x[f]; f--);
		x.f = f;
	}
	static void Absmul (const Biguint &x, const Biguint &y, Biguint &z) {
		if (x.f < brute_limit && y.f < brute_limit) mul_brute (x, y, z);
		else fft (x, y, z);
	}
	template <class T> static void Absdiv (const Biguint &x, T y, Biguint &div, T &mod) {
		div.Initialize();
		T s;
		mod = 0;
		for (int i = x.f; i >= 0; i--) s = mod * MOD + x[i], div[i] = s / y, mod = s - div[i] * y;
		for (div.f = x.f; div.f && !div[div.f]; div.f--);
	}
	static void Absdivide (const Biguint &x, const Biguint &y, Biguint &div, Biguint &mod) {
		if (x.f < brute_limit && y.f < brute_limit) div_brute (x, y, div, mod);
		else div_fft (x, y, div, mod);
	}
	void Absprint() {
		for (int i = f; i >= 0; i--) {
			if (i < f) {
				if (d[i] < 1000) putchar (48);
				if (d[i] < 100) putchar (48);
				if (d[i] < 10) putchar (48);
			}
			printf ("%d", d[i]);
		}
	}
	void shl (int _n) {
		for (int i = f; i >= 0; i--) if (i + _n < n) d[i + _n] = d[i];
		for (int i = 0; i < _n; i++) if (i < n) d[i] = 0;
		f += _n;
		if (f >= n) f = n - 1;
		if (f && !d[f]) f--;
	}
	void shr (int _n) {
		for (int i = _n; i <= f; i++) d[i - _n] = d[i];
		for (int i = f + 1 - _n; i <= f; i++) if (i >= 0) d[i] = 0;
		f -= _n;
		if (f < 0) f = 0;
	}
	static void mul_brute (const Biguint &x, const Biguint &y, Biguint &z) {
		int f = y.f + x.f + 1;
		z.Initialize();
		for (int i = 0; i <= x.f; i++) {
			int g = 0;
			for (int j = 0; j <= y.f + 1; j++)
				z[i + j] += x[i] * y[j] + g, g = z[i + j] / MOD, z[i + j] -= g * MOD;
		}
		for (; f && !z[f]; f--);
		z.f = f;
	}
	static void dft (Complex *a, int N, int f) {
		int d = maxn / N * f;
		Complex x;
		for (int n = N, m; m = n / 2, m >= 1; n = m, d *= 2)
			for (int i = 0; i < m; i++)
				for (int j = i; j < N; j += n)
					x = a[j] - a[j + m], a[j] += a[j + m], a[j + m] = x * e[d * i];
		for (int i = 0, j = 1; j < N - 1; j++) {
			for (int k = N / 2; k > (i ^= k); k /= 2);
			if (j < i) std::swap (a[i], a[j]);
		}
	}
	static void fft_fwd (const Biguint &x, Complex *wx, int n) {
		for (int i = 0; i < n; i++)
			wx[i] = Complex (i <= x.f ? x[i] : 0, 0);
		dft (wx, n, 1);
	}
	static void fft_bwd (Complex *wx, Biguint &x, int n) {
		dft (wx, n, -1);
		double t, g = 0;
		for (int i = 0; i < n; i++) {
			t = wx[i].real() / n + g;
			g = (int) (t / MOD), t -= g * MOD;
			if (t + 0.1 > MOD) t = 0, g += 1;
			if (t + 0.1 < 0) t += MOD, g -= 1;
			tmp[i] = (int) (t + 0.5);
		}
		int tmpf = n - 1;
		for (; tmpf && !tmp[tmpf]; tmpf--);
		for (int i = (tmpf < x.f ? x.f : tmpf); i >= 0; i--) x[i] = (i < n ? tmp[i] : 0);
		x.f = tmpf;
	}
	static int find (int n) {
		for (int i = 0; i <= 18; i++)
			if (1 << i >= n) {
				n = 1 << i;
				break;
			}
		return n;
	}
	static void mult (Complex *a, Complex *b, int n) {
		for (int i = 0; i < n; i++) a[i] *= b[i];
	}
	static void fft (const Biguint &x, const Biguint &y, Biguint &z) {
		int n = find (x.f + y.f + 2);
		fft_fwd (x, w[0], n), fft_fwd (y, w[1], n);
		mult (w[0], w[1], n);
		fft_bwd (w[0], z, n);
	}
	static void sqr (const Biguint &x, Biguint &y) {
		int n = find (2 * x.f + 2);
		fft_fwd (x, w[0], n);
		mult (w[0], w[0], n);
		fft_bwd (w[0], y, n);
	}
	static void rev (Biguint &a, int n, Biguint &b, Biguint &tmp) {
		if (n == 1) {
			b = MOD * MOD / a[0];
			return;
		}
		if (n == 2) {
			b = (long long) MOD * MOD * MOD * MOD / (a[0] + a[1] * MOD);
			return;
		}
		int k = n + 2 >> 1;
		a.d += n - k;
		a.f -= n - k;
		rev (a, k, tmp, b);
		a.d -= n - k;
		a.f += n - k;
		b = tmp * 2;
		b.shl (n - k);
		sqr (tmp, tmp);
		int t = find (2 * n + 6);
		fft_fwd (tmp, w[0], t);
		fft_fwd (a, w[1], t);
		mult (w[0], w[1], t);
		fft_bwd (w[0], tmp, t);
		tmp.shr (2 * k);
		b -= tmp;
		fft_fwd (b, w[0], t);
		mult (w[0], w[1], t);
		fft_bwd (w[0], tmp, t);
		if (tmp.d[2 * n]) {
			tmp.d[2 * n]--;
			for (; tmp.f && !tmp.d[tmp.f]; tmp.f--);
			int l = 0, r = MOD * MOD;
			for (int mid; mid = l + r >> 1, l < r;)
				if (a * (long long) mid < tmp) l = mid + 1;
				else r = mid;
			b -= l;
		} else {
			for (int i = 0; i < 2 * n; i++) tmp.d[i] = MOD - 1 - tmp.d[i];
			for (tmp.f = 2 * n - 1; tmp.f && !tmp.d[tmp.f]; tmp.f--);
			tmp += 1;
			int l = 0, r = MOD * MOD;
			for (int mid; mid = l + r + 1 >> 1, l < r;)
				if (a * (long long) mid > tmp) r = mid - 1;
				else l = mid;
			b += l;
		}
	}
	static void div_brute (const Biguint &x, const Biguint &y, Biguint &div, Biguint &mod) {
		div.Initialize();
		mod.Initialize();
		for (int i = x.f; i >= 0; i--) {
			mod.shl (1);
			mod[0] = x[i];
			int l = 0, r = MOD - 1;
			for (int mid = l + r + 1 >> 1; l < r; mid = l + r + 1 >> 1)
				if (y * mid <= mod) l = mid;
				else r = mid - 1;
			div[i] = l;
			mod -= y * l;
		}
		int f = x.f;
		for (; f && !div[f]; f--);
		div.f = f;
		f = y.f;
		for (; f && !mod[f]; f--);
		mod.f = f;
	}
	static void div_fft (const Biguint &a, const Biguint &b, Biguint &c, Biguint &d) {
		if (a < b) {
			c.Initialize(), d = a;
			return;
		}
		int taf = a.f, tbf = b.f, shift = 0;
		if (taf > tbf * 2) shift = taf - tbf * 2, taf += shift, tbf += shift;
		static Biguint ta (0, 2 * max + 10), tb (0, max + 10);
		ta = a, tb = b;
		if (shift) ta.shl (shift), tb.shl (shift);
		static Biguint tc (0, 3 * max + 10), td (0, 2 * max + 10);
		rev (tb, tb.f + 1, td, tc);
		fft (ta, td, tc);
		tc.shr (2 * (tb.f + 1));
		fft (tb, tc, td);
		td = ta - td;
		int l = 0, r = MOD;
		for (int mid; mid = l + r + 1 >> 1, l < r;)
			if (tb * (long long) mid > td) r = mid - 1;
			else l = mid;
		tc += l;
		td -= tb * l;
		if (shift) td.shr (shift);
		c = tc, d = td;
	}
};
void Init()
{
	maxn = 3 * max + 10;
	int i = 1;
	for (; i < maxn; i <<= 1);
	maxn = i;
	stack = new Complex[2 * maxn - 1], tmp = new int[maxn];
	for (int i = 0; i < 7; i++) w[i] = new Complex[maxn];
	e = stack + maxn - 1;
	e[0] = Complex (1, 0);
	for (int i = 1; i < maxn; i <<= 1) e[i] = Complex (cos (2 * pi * i / maxn), sin (2 * pi * i / maxn) );
	for (int i = 1, j; i < maxn; i++)
		if (i - (j = i & -i) ) e[i] = e[i - j] * e[j];
	for (int i = 1; i < maxn; i++) e[-i] = e[maxn - i];
}
