//////////////////////////////////////////////////////////////////////////////
// Example: simpson([](double x){return cos(x);}).adaptive(0,M_PI/2, 1.e-5);
//
// Simple is $O(1)$ and error <= |b-a|^4 * fourth_derivative
// Adaptive is roughly $O((b-a)/eps)$ with small constant. Error roughly $eps$
template<typename F> struct Tsimpson {
	F &f;
	Tsimpson(F f) : f(f) {}
	double simple(double a, double b) {
		double c = (a + b) / 2.;
		double h3 = fabs(b-a)/6;
		return h3 * (f(a) + 4.*f(c) + f(b));
	}
	double adaptive(double a, double b, double eps, double whole = INFINITY) {
		if (whole == INFINITY) whole = simple(a, b);
		double c = (a+b)/2;
		double left = simple(a, c);
		double right = simple(c, b);
		if (fabs(left+right -whole) <= 15*eps)
			return left + right + (left + right - whole)/15.;
		return adaptive(a, c, eps/2, left) +
			adaptive(c, b, eps/2, right);
	}
};
template<class F> Tsimpson<F> simpson(F f) {
	return Tsimpson<F>(f);
}
