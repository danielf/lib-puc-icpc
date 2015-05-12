#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;

#define TRACE(x...) 
#define WATCH(x) TRACE(cout << #x" = " << x << endl)
#define PRINT(x...) TRACE(printf(x); fflush(stdout))

#define all(v) (v).begin(), (v).end()

#define FU(i, a, b) for(decltype(b) i = (a); i < (b); ++i)
#define fu(i, n) FU(i, 0, n)

#define FD(i, a, b) for (decltype(b) i = (b)-1; i >= a; --i)
#define fd(i, n) FD(i, 0, n)

#define mod(a, b) ((((a)%(b))+(b))%(b))
#define pb push_back
#define sz(c) int((c).size())
#define mp make_pair

const int INF = 0x3F3F3F3F; const int NEGINF = 0xC0C0C0C0;
const double EPS = 1e-8;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;
typedef long long ll;
typedef vector<ll> vll;
typedef vector<vll> vvll;

int cmp(double x, double y = 0, double tol = EPS) {
	return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}

inline void scanint(int &x) {
	auto gc = getchar_unlocked;
	int c = gc();
	x = 0;
	int neg = 0;
	for (;((c < '0' || c > '9') && c != '-'); c = gc());
	if (c == '-') {neg = 1; c = gc();}
	for (;c >= '0' && c <= '9'; c = gc()) {x = 10*x + c - '0';}
	if (neg) x = -x;
}
////////////////////////////////////////////////////////////////////////////////
// Computes GCD of x and y
//

int gcd(int x, int y) { return y ? gcd(y, x % y) : abs(x); }

////////////////////////////////////////////////////////////////////////////////
// Computes lcm of x and y
//

ll lcm(int x, int y) {
	if (x && y) return abs(x) / gcd(x, y) * ll(abs(y));
	else return ll(abs(x | y));
}

////////////////////////////////////////////////////////////////////////////////
// Is n prime? "O(sqrt(n)/3)"
//

bool is_prime(int n) {
	if (n < 0) return is_prime(-n);
	if (n < 5 || n % 2 == 0 || n % 3 == 0) return (n == 2 || n == 3);
	int maxP = sqrt(n) + 2;
	for (int p = 5; p < maxP; p += 6)
		if (n % p == 0 || n % (p+2) == 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Returns prime number factorization of n. "O(sqrt(n)/3)"
//

#include <map>
typedef map<int, int> prime_map;

void squeeze(prime_map& M, int& n, int p) { for (; n % p == 0; n /= p) M[p]++; }

prime_map factor(int n) {
	prime_map M;
	if (n < 0) return factor(-n);
	if (n < 2) return M;
	squeeze(M, n, 2); squeeze(M, n, 3);
	int maxP = sqrt(n) + 2;
	for (int p = 5; p < maxP; p += 6) {
		squeeze(M, n, p); squeeze(M, n, p+2);
	}
	if (n > 1) M[n]++;
	return M;
}

////////////////////////////////////////////////////////////////////////////////
// Bezout's theorem ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Finds a and b such that a*x + b*y == gcd(x,y)
// Works in O(log(x+y))
// To find inv(x) modulo y do: inv = find_bezout(x, y, y).first;

typedef pair<ll, ll> bezout;

bezout find_bezout(ll x, ll y, ll m) {
	if (y == 0) return bezout(1, 0);
	bezout u = find_bezout(y, x % y, m);
	return bezout(u.second, mod(u.first - (x/y) * u.second, m));
}

////////////////////////////////////////////////////////////////////////////////
// Finds smallest non-negative solution of a*x = b (mod m).
//
// Returns -1 if there is no solution.
// Let s be the solution found (if != -1). To find other solutions do:
//    mod(s + k*(m/d), m) for k = 1,...,d-1, where d = gcd(a, m)
// runs in O(log m)

int solve_mod(int a, int b, int m) {
	if (m < 0) return solve_mod(a, b, -m);
	a = mod(a, m); b = mod(b, m);
	bezout t = find_bezout(a, m, m);
	int d = (t.first*a) % m;
	if (b % d) return -1;
	else return mod(t.first * (b / d), m);
}

////////////////////////////////////////////////////////////////////////////////
// Pascal Triangle /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const int SIZE = 30;

int C[SIZE][SIZE];

void comp_pascal() {
	memset(C, 0, sizeof(C));
	for (int i = 0; i < SIZE; i++) {
		C[i][0] = C[i][i] = 1;
		for (int j = 1; j < i; j++)
			C[i][j] = C[i-1][j-1] + C[i-1][j];
	}
}

////////////////////////////////////////////////////////////////////////////////
// Fast Exponentiation /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// runs in O(log b)

ll power(ll a, ll b, ll m) {
	if (b == 0) return 1;
	ll x = power(a, b/2, m);
	if (b % 2 == 0) return (x*x) % m;
	else return (((a*x) % m)*x) % m;
}

////////////////////////////////////////////////////////////////////////////////
// Sieve ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// runs in O(n log log n)
// returns a vector with the largest prime divisor of each number

vi sieve(int N) {
	vi ans(N, -1); ans[0] = 0; ans[1] = 1;
	FU(i, 2, N) if (ans[i] == -1)
		for (int j = i; j < N; j += i) ans[j] = i;
	return ans;
}

// Returns all the primes less than N in O(N log log N)
vi primes(int N) {
	auto div = sieve(N);
	vi ans;
	FU(i, 2, N) if (div[i] == i) ans.pb(i);
	return ans;
}

// Computes phi[i] for all numbers < N in O(N log log N)
vi calc_phi(int N) {
	auto div = sieve(N);
	vi ans(N);
	ans[1] = 1;
	FU(i, 2, N) {
		int p = div[i], x = i/p;
		if (div[x] == p) ans[i] = ans[x] * p;
		else ans[i] = ans[x] * (p - 1);
	}
	return ans;
}

// Computes d[i] (number of divisors) for all numbers < N in O(N log log N)
vi calc_d(int N) {
	auto div = sieve(N);
	vi ans(N);
	ans[1] = 1;
	FU(i, 2, N) {
		int p = div[i], x = i / p;
		if (div[x] != p) ans[i] = 1;
		else ans[i] = ans[x] + 1;
	}
	FU(i, 2, N) {
		int p = div[i], x = i / p;
		if (ans[i] == 1) ans[i] = ans[x] * 2;
		else ans[i] = (ans[x] / ans[i]) * (ans[i] + 1);
	}
	return ans;
}

// Computes mu[i] (Mobius function) for all numbers < N in O(N log log N)
vi calc_mu(int N) {
	auto div = sieve(N);
	vi ans(N);
	ans[1] = 1;
	FU(i, 2, N) {
		int p = div[i], x = i / p;
		if (div[x] == p) ans[i] = 0;
		else ans[i] = -ans[x];
	}
	return ans;
}

// discrete log (solves a^x = b (mod N)) in O(sqrt(N)). Returns -1 if no solution
#include <unordered_map>
ll discrete_log(ll a, ll b, ll N) {
	ll d = gcd(a, N);
	if (b % d) return -1;
	a /= d; b /= d; N /= d;
	ll sqN = 0;
	for (; sqN*sqN < N; sqN++);
	WATCH(N); WATCH(sqN);
	ll giant = power(find_bezout(a, N, N).first, sqN, N);
	WATCH(giant);
	unordered_map<ll, ll> baby;
	ll tmp = 1;
	fu(i, sqN) {
		if (!baby.count(tmp)) baby[tmp] = i;
		tmp = (a*tmp) % N;
	}
	for (auto x : baby) PRINT("baby[%lld] = %lld\n", x.first, x.second);
	fu(i, sqN) {
		if (baby.count(b)) return baby[b] + i*sqN;
		b = (giant*b) % N;
	}
	return -1;
}

int main() {
	ll P, B, N;
	while (scanf("%lld %lld %lld", &P, &B, &N) != EOF) {
		ll ans = discrete_log(B, N, P);
		if (ans == -1) printf("no solution\n");
		else printf("%lld\n", ans);
	}
	return 0;
}

