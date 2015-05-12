#include "template.cpp"
#include "number_theory/number_theory.cpp"
#include "FFT.cpp"

const ll MOD = 469762049;
const ll g = 385303873;

int main() {
	int N;
	scanf("%d", &N);
	vll V(N);
	for (ll &x: V) scanf("%lld", &x);
	vll V2(N);
	for (ll &x :V2) scanf("%lld", &x);
	
	auto W = FFT(V, MOD, g);
	auto W2 = FFT(V2, MOD, g);
	vll Z(N);
	fu(i, N) Z[i] = (W[i] * W2[i]) % MOD;
	Z = IFFT(Z, MOD, g);

	vi inx{0, N-1, N/2};

	for (int i : inx) {
		ll tmp = 0;
		fu(j, N) tmp = (tmp + V[j] * V2[mod(i-j, N)]) % MOD;
		if (tmp != Z[i]) printf("%d: %lld != %lld\n", i, tmp, Z[i]);
	}

	return 0;
}
