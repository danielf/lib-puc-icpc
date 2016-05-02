////////////////////////////////////////////////////////////////////////////////
// Computes GCD of x and y
//

// start: fb18639d2794d319264de4bed0a94178  -
ll gcd(ll x, ll y) { return y ? gcd(y, x % y) : abs(x); }
// end

////////////////////////////////////////////////////////////////////////////////
// Computes lcm of x and y
//

// start: 20633bef871da761493195249aa706d8  -
ll lcm(ll x, ll y) {
    if (x && y) return abs(x) / gcd(x, y) * ll(abs(y));
    else return ll(abs(x | y));
}
// end

////////////////////////////////////////////////////////////////////////////////
// Is n prime? "O(sqrt(n)/3)"
//

// start: 0e329987c0925fd83b84616b9e93f773  -
bool is_prime(ll n) {
    if (n < 0) return is_prime(-n);
    if (n < 5 || n % 2 == 0 || n % 3 == 0) return (n == 2 || n == 3);
    ll maxP = sqrt(n) + 2;
    for (ll p = 5; p < maxP; p += 6)
        if (n % p == 0 || n % (p+2) == 0) return false;
    return true;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Returns prime number factorization of n. "O(sqrt(n)/3)"
//

// start: 8199052eafe6802e1b50023ee29cbbb2  -
typedef unordered_map<ll, int> prime_map;

void squeeze(prime_map& M, ll& n, ll p) { for (; n % p == 0; n /= p) M[p]++; }

prime_map factor(ll n) {
    prime_map M;
    if (n < 0) return factor(-n);
    if (n < 2) return M;
    squeeze(M, n, 2); squeeze(M, n, 3);
    ll maxP = sqrt(n) + 2;
    for (ll p = 5; p < maxP; p += 6) {
        squeeze(M, n, p); squeeze(M, n, p+2);
    }
    if (n > 1) M[n]++;
    return M;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Bezout's theorem ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Finds a and b such that a*x + b*y == gcd(x,y)
// Works in O(log(x+y))
// To find inv(x) modulo y do: inv = find_bezout(x, y, y).first;

// start: 4ec93a4fcc761f46a7c9a73856607036  -
typedef pair<ll, ll> bezout;

bezout find_bezout(ll x, ll y, ll m) {
    if (y == 0) return bezout(1, 0);
    bezout u = find_bezout(y, x % y, m);
    return bezout(u.second, mod(u.first - (x/y) * u.second, m));
}
// end

////////////////////////////////////////////////////////////////////////////////
// Finds smallest non-negative solution of a*x = b (mod m).
//
// Returns -1 if there is no solution.
// Let s be the solution found (if != -1). To find other solutions do:
//    mod(s + k*(m/d), m) for k = 1,...,d-1, where d = gcd(a, m)
// runs in O(log m)

// start: 9ee4ba79297f44ffa1c0f425cf9e03ea  -
ll solve_mod(ll a, ll b, ll m) {
    if (m < 0) return solve_mod(a, b, -m);
    a = mod(a, m); b = mod(b, m);
    bezout t = find_bezout(a, m, m);
    ll d = (t.first*a) % m;
    if (b % d) return -1;
    else return mod(t.first * (b / d), m);
}
// end

////////////////////////////////////////////////////////////////////////////////
// Pascal Triangle /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// start: 9c2b980c8b5634ff3974635b045943e6  -
const int SIZE = 30;

ll C[SIZE][SIZE];

void comp_pascal() {
    memset(C, 0, sizeof(C));
    for (int i = 0; i < SIZE; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
}
// end

////////////////////////////////////////////////////////////////////////////////
// Fast Exponentiation /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// runs in O(log b)

// start: 251063753d950341411eb9b21d6fd610  -
ll power(ll a, ll b) {
    if (b == 0) return 1;
    ll x = power(a, b/2);
    if (b % 2 == 0) return x*x;
    else return a*x*x;
}

ll power(ll a, ll b, ll mod) {
    if (b == 0) return 1;
    ll x = power(a, b/2, mod);
    if (b % 2 == 0) return (x*x) % mod;
    else return (((a*x) % mod)*x) % mod;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Sieve ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// runs in O(n log log n)
// returns a vector with the largest prime divisor of each number

// start: f7d74d75b5bc8645db0472ccab1a507f  -
vi sieve(int N) {
    vi ans(N, -1); ans[0] = 0; ans[1] = 1;
		for (int i = 2; i < N; i++) if (ans[i] == -1)
        for (int j = i; j < N; j += i) ans[j] = i;
    return ans;
}
// end

// Returns all the primes less than N in O(N log log N)
// start: 2e5ed3f358b75dc88bb5effc62042be6  -
vi primes(int N) {
    auto div = sieve(N);
    vi ans;
		for (int i = 2; i < N; i++) if (div[i] == i)
			ans.push_back(i);
    return ans;
}
// end

// Computes phi[i] for all numbers < N in O(N log log N)
// start: 8197adf0945339715dec7986c4084706  -
vi calc_phi(int N) {
    auto div = sieve(N);
    vi ans(N);
    ans[1] = 1;
		for (int i = 2; i < N; i++) {
        int p = div[i], x = i/p;
        if (div[x] == p) ans[i] = ans[x] * p;
        else ans[i] = ans[x] * (p - 1);
    }
    return ans;
}
// end

// Computes d[i] (number of divisors) for all numbers < N in O(N log log N)
// start: b0f70f0fabe63a29d727a301c3489e8d  -
vi calc_d(int N) {
    auto div = sieve(N);
    vi ans(N);
    ans[1] = 1;
		for (int i = 2; i < N; i++) {
        int p = div[i], x = i / p;
        if (div[x] != p) ans[i] = 1;
        else ans[i] = ans[x] + 1;
    }
		for (int i = 2; i < N; i++) {
        int p = div[i], x = i / p;
        if (ans[i] == 1) ans[i] = ans[x] * 2;
        else ans[i] = (ans[x] / ans[i]) * (ans[i] + 1);
    }
    return ans;
}
// end

// Computes mu[i] (Mobius function) for all numbers < N in O(N log log N)
// start: dc4e635669e22d2530af8cb064522b0e  -
vi calc_mu(int N) {
    auto div = sieve(N);
    vi ans(N);
    ans[1] = 1;
		for (int i = 2; i < N; i++) {
        int p = div[i], x = i / p;
        if (div[x] == p) ans[i] = 0;
        else ans[i] = -ans[x];
    }
    return ans;
}
// end

////////////////////////////////////////////////////////////////////////////////
// Discrete Log ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// discrete log (solves a^x = b (mod N)) in O(sqrt(N)). Returns -1 if no solution
// start: f8f49957a46d86d55fc57bb3a8e8a3af  -
ll discrete_log(ll a, ll b, ll N) {
    ll d = gcd(a, N);
    if (b % d) return -1;
    a /= d; b /= d; N /= d;
    ll sqN = 0;
//		 start: 8221f1a3ce1dc260b1abf20d69dfd2d2  -
    for (; sqN*sqN < N; sqN++);
    ll giant = power(find_bezout(a, N, N).first, sqN, N);
    unordered_map<ll, ll> baby;
    ll tmp = 1;
    for (int i = 0; i < sqN; i++) {
        if (!baby.count(tmp)) baby[tmp] = i;
        tmp = (a*tmp) % N;
    }
// 		end
    for (int i = 0; i < sqN; i++) {
        if (baby.count(b)) return baby[b] + i*sqN;
        b = (giant*b) % N;
    }
    return -1;
}
// end
