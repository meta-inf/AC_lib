/*
 * Self-adjust simpson integration
 * Last Edit : 09-05-2013
 */
double f (double t)
{
	return f(t);
}
 /*
 	When the function is approximately piecewise linear 
inline double eval (double l, double r, double fl, double fr, double fm) {
    return (fl + fr + fm) / 3.0 * (r - l);
}*/
inline double eval (double l, double r, double fl, double fr, double fm) {
    return (fl + fr + 4 * fm) / 6.0 * (r - l);
}
double rsimpson (double l, double r, double fl, double fm, double fr)
{
//  fprintf(stderr, "%.2lf %.2lf\n", l, r);
    double m = (l + r) / 2,
        ml = (l + m) / 2, mr = (m + r) / 2,
        fml = f(ml), fmr = f(mr),
        s = eval(l, r, fl, fr, fm), sl = eval(l, m, fl, fm, fml),
        sr = eval(m, r, fm, fr, fmr);
	if (fabs(sl + sr - s) < eps) return sl + sr;
//    if (r - l < 1e-3 && fabs(sl + sr - s) < eps) return sl + sr;
    return rsimpson(l, m, fl, fml, fm) + rsimpson(m, r, fm, fmr, fr);
}
 
double rsimpson (int l, int r) {
    return rsimpson(l, r, f(l), f((l + r) / 2), f(r));
}
 
//to prevent the loss of precision, do not call rsimpson(-inf,+inf) directly
