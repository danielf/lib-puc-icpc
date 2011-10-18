
// given a series of congruences x = y mod m,
// returns the value of y' for x = y' mod N, with N = m1*m2*...*mn
// or -1 if impossible (that is, gcd of m1,m2,...,mn is not 1)

typedef pair<long long int, long long int> bezout;

// start: 0b5b5ca8b25d69813d635b333738b5f2
bezout find_bezout_mod(int x, int y, int m) {
	if (y == 0) return bezout(1, 0);
	bezout u = find_bezout_mod(y, x % y, m);
	return bezout(u.second, (u.first + m - ((x / y) * u.second) % m) % m);
}

// start: d43af594497721b2ee4e7d19feeea3c4
int TRC(vector< pair<int,int> > &v) { // vector of (remainder, modulo)
	int e[v.size()], N = 1, g = 0;
	for (int i=0; i < (int)v.size(); i++) {
		N = lcm(N, v[i].second);
		g = gcd(g, v[i].second);
	}
	if (g != 1) return -1;

	for (int i=0; i < (int)v.size(); i++) {
		int ni = v[i].second, N_ni = N / ni;
		e[i] = 0;
		int d = find_bezout_mod(ni, N_ni, ni).second;
		e[i] = ((long long)d * N_ni) % N;
	}

	int x = 0;
	for (int i=0; i < (int)v.size(); i++) {
		x += ((long long)v[i].first * e[i]) % N;
		x %= N;
	}
	return x;
}

