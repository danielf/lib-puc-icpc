// given a series of congruences x = y mod m,
// returns the value of y' for x = y' mod N, with N = m1*m2*...*mn
// or -1 if impossible (that is, gcd of m1,m2,...,mn is not 1)
//
// Runs in O(n*log(N))

// Needs Bezout, gcd, lcm from number theory

// start: 0361be01566c3a8c10e4f8171c650fd3  -
int CRT(vector< pair<int, int> > &v) { // vector of (remainder, modulo)
// 		start: cd2001d31d8c6a2c13dd8233ee40546b  -
	vector<ll> e(v.size());
 	ll N = 1, g = 0;
	for (int i=0; i < (int)v.size(); i++) {
		N = lcm(N, v[i].second);
		g = gcd(g, v[i].second);
	}
	if (g != 1) return -1;

	for (int i=0; i < (int)v.size(); i++) {
		ll ni = v[i].second, N_ni = N / ni;
		e[i] = 0;
		ll d = find_bezout(ni, N_ni, ni).second;
		e[i] = (d * N_ni) % N;
	}
// 		end

// 		start: 5d7335e6ac4009d5ea6f6d3b94b4e712  -
	ll x = 0;
	for (int i=0; i < (int)v.size(); i++) {
		x += (v[i].first * e[i]) % N;
		x %= N;
	}
	return x;
// 		end
}
// end
