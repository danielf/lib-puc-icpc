#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;

#define TRACE(x...) x
#define WATCH(x) TRACE(cout << #x << " = " << x << endl)
#define PRINT(x...) TRACE(printf(x))

#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()

// If using C++11, change __typeof__ to decltype
#define _for(i, a, b) for (decltype(a) i = (a); i != (b); ++i)
#define foreach(x...) _for(x)
#define forall(i, v) foreach(i, all(v))
// If using C++11, change __typeof__ to decltype
#define FU(i, a, b) for(decltype(a) i = (a); i < (b); ++i)
#define fu(i, n) FU(i, 0, n)

#define mset(c, v) memset(c, v, sizeof(c))
#define mod(a, b) ((((a)%(b))+(b))%(b))
#define pb push_back
#define sz(c) int((c).size())
const int INF = 0x3F3F3F3F; const int NEGINF = 0xC0C0C0C0;
const int NULO = -1; const double EPS = 1e-8;

typedef long long ll;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<vi> vvi;
const ll MOD = 1000000000ll;

int cmp(double x, double y = 0, double tol = EPS) {
	return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}

vector<ll> read(ll N, ll m, ll plus) {
	ll S1, S2, XS, YS, ZS;
	scanf("%lld %lld %lld %lld %lld", &S1, &S2, &XS, &YS, &ZS);
	vector<ll> ans(N);
	ans[0] = S1; ans[1] = S2;
	FU(i, 2, N) ans[i] = ((XS*ans[i-2] + YS*ans[i-1] + ZS) % m) + plus;
	return ans;
}


int main() {
	int _41;
	scanf("%d", &_41);
	fu(_42, _41) {
		ll N, M;
		scanf("%lld %lld", &N, &M);
		auto S = read(N, MOD, 0);
		auto O = read(M, 4, 1);
		auto A = read(M, N, 1);
		auto B = read(M, N, 1);
		auto C = read(M, MOD, 0);
		auto D = read(M, MOD, 0);
		ll ans = 0;
		fu(i, M) {
			A[i]--;
			if (O[i] == 1) {
				fu(j, B[i]) {
					int k = (A[i] + j) % N;
					ans += C[i] + (ll)j*D[i];
					S[k] += C[i] + (ll)j*D[i];
					ans %= MOD;
					S[k] %= MOD;
				}
			} else if (O[i] == 2) {
				fu(j, B[i]) {
					int k = (A[i] + j) % N;
					ans += S[k] + C[i];
					S[k] = C[i];
					ans %= MOD;
				}
			} else if (O[i] == 3) {
				fu(j, B[i]) ans = (ans + S[(A[i]+j)%N]) % MOD;
			} else
				fu(j, B[i]) ans = (ans + (S[(A[i]+j)%N]%2)) % MOD;
		}
		printf("Case #%d: %lld\n",_42+1, ans);
	}
	return 0;
}
