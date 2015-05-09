struct RMQ {
	vi V;
	int N, l;
	RMQ(const vi& input) {
		N = sz(input);
		// in case N is ll use 63-__builtin_clzll
		l = 32 - __builtin_clz(N);
		V = vi(N*l);
		copy(all(input), V.begin());
		FU(j, 1, l) fu(i, N) {
			int mid = i + (1 << (j-1));
			if (mid >= N) mid = N-1;
			// change min to whatever function you want.
			// Function has to satisfy f(a,a) = a
			V[j*N+i] = min(V[(j-1)*N+i], V[(j-1)*N + mid]);
		}
	}
	// Returns the minimum of the CLOSED interval [a,b]
	int get(int a, int b) {
		b++;
		int j = 31 - __builtin_clz(b - a);
		// change min to whatever function
		return min(V[j*N+a], V[j*N + b - (1<<j)]);
	}
};
