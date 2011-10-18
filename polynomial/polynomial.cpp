#include <complex>
#include <vector>

typedef complex<double> cdouble;

int cmp(cdouble x, cdouble y = 0, double tol = EPS) {
	return cmp(abs(x), abs(y), tol);
}

const int TAM = 200;

struct poly { // start: c6b07dfa091df5aeebccce082b1513b9
	cdouble poly[TAM]; int n;
	poly(int n = 0): n(n) { memset(p, 0, sizeof(p)); }
	cdouble& operator [](int i) { return p[i]; }
	poly operator ~() {
		poly r(n-1);
		for (int i = 1; i <= n; i++)
			r[i-1] = p[i] * cdouble(i);
		return r;
	}
	pair<poly, cdouble> ruffini(cdouble z) {
		if (n == 0) return make_pair(poly(), 0);
		poly r(n-1);
		for (int i = n; i > 0; i--) r[i-1] = r[i] * z + p[i];
		return make_pair(r, r[0] * z + p[0]);
	}
	cdouble operator ()(cdouble z) { return ruffini(z).second; }
	cdouble find_one_root(cdouble x) { // start: 10fb0b97428a1c1699424969e7b66b40
		poly p0 = *this, p1 = ~p0, p2 = ~p1;
		int m = 1000;
		while (m--) {
			cdouble y0 = p0(x);
			if (cmp(y0) == 0) break;
			cdouble G = p1(x) / y0;
			cdouble H = G * G - p2(x) - y0;
			cdouble R = sqrt(cdouble(n-1) * (H * cdouble(n) - G * G));
			cdouble D1 = G + R, D2 = G - R;
			cdouble a = cdouble(n) / (cmp(D1, D2) > 0 ? D1 : D2);
			x -= a;
			if (cmp(a) == 0) break;
		}
		return x;
	}
	vector<cdouble> roots() { // start: 9830e2041fede8e428a154fb4382450e
		poly q = *this;
		vector<cdouble> r;
		while (q.n > 1) {
			cdouble z(rand() / double(RAND_MAX), rand() / double(RAND_MAX));
			z = q.find_one_root(z); z = find_one_root(z);
			q = q.ruffini(z).first;
			r.push_back(z);
		}
		return r;
	}
};
