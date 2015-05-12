
// given a series of congruences x = y_i mod m_i, ..., x = y_k mod m_k
// returns the value of y' such that x = y' mod N, with N = m1*m2*...*mn
// 
// Assumes gcd(a_i, a_j) == 1 for all i != j
//
// Runs in O(k*log(M)) where M = max(m_i)

int CRT(vector< pair<int,int> > &v) { // vector of (remainder, modulo)
	int e[v.size()], N = 1, g = 0;
	for (int i=0; i < (int)v.size(); i++) {
		N = lcm(N, v[i].second);
		g = gcd(g, v[i].second);
	}

	for (int i=0; i < (int)v.size(); i++) {
		int ni = v[i].second, N_ni = N / ni;
		e[i] = 0;
		int d = find_bezout(ni, N_ni, ni).second;
		e[i] = ((long long)d * N_ni) % N;
	}

	int x = 0;
	for (int i=0; i < (int)v.size(); i++) {
		x += ((long long)v[i].first * e[i]) % N;
		x %= N;
	}
	return x;
}

