// Convolution of vectors V and W is a vector Z
// where Z_i = \sum_{j=0,...,N-1} V_j * W_{i-j}
//
// Let F' be the FFT(F), then if Z is the convolution
// of V and W, then Z' = V' . W', where . means pointwise
// multiplication.
//
// g should be a sz(input)-th root 1 in Z_MOD
// sz(input) should be a power of 2
// O(n log n)

const int LIM = 16;
vll FFT(const vll &input, ll MOD, ll g) {
	ll N = sz(input);
	if (N == 1) return input;
	if (N <= LIM) { // O(N^2) implementation for reference
		vll ans(N, 0);
		fu(i, N) {
			ll twim = 1;
			fu(j, i) twim = (g*twim) % MOD;
			ll twiddle = 1;
			fu(j, N) {
				ans[i] = (ans[i] + input[j]*twiddle) % MOD;
				twiddle = (twiddle * twim) % MOD;
			}
		}
		return ans;
	}
	vll odd(N/2), even(N/2);
	fu(i, N/2) {
		even[i] = input[2*i];
		odd[i] = input[2*i+1];
	}
	even = FFT(even, MOD, (g*g) % MOD);
	odd = FFT(odd, MOD, (g*g) % MOD);
	vll ans(N);
	ll twiddle = 1;
	fu(i, N) {
		ans[i] = (even[i % (N/2)] + twiddle * odd[i % (N/2)]) % MOD;
		twiddle = (g*twiddle) % MOD;
	}
	return ans;
}

vll IFFT(const vll &input, ll MOD, ll g) {
	ll N = sz(input);
	vll tmp = input;
	reverse(tmp.begin() + 1, tmp.end());
	vll ans = FFT(tmp, MOD, g);
	ll Ninv = power(N, MOD-2, MOD);
	fu(i, N) ans[i] = (ans[i] * Ninv) % MOD;
	return ans;
}

