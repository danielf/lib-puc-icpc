// g should be a sz(input)-th root 1 in Z_MOD
// sz(input) should be a power of 2
// (2, 1), (3, 2), (5, 2), (17, 9), (17, 3), (97, 28), (193, 125), (257, 9),
// (257, 3), (7681, 7146), (12289, 10302), (12289, 1945), (12289, 1331),
// (40961, 243), (65537, 81), (65537, 9), (65537, 3), (786433, 213567),
// (786433, 1000), (5767169, 177147), (7340033, 2187), (23068673, 177147),
// (104857601, 39193363), (167772161, 131341181), (167772161, 59049),
// (167772161, 243), (469762049, 2187), (2013265921, 440564289)
//
// ith tuple (a, b) = prime a = k*2**i + 1, b is (2**i)-th root of 1, 0 indexed

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

