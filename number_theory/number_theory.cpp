////////////////////////////////////////////////////////////////////////////////
// Computes GCD of x and y
//

ll gcd(ll x, ll y) { return y ? gcd(y, x % y) : abs(x); }

////////////////////////////////////////////////////////////////////////////////
// Computes lcm of x and y
//

ll lcm(ll x, ll y) {
  if (x && y) return abs(x) / gcd(x, y) * ll(abs(y));
  else return ll(abs(x | y));
}

////////////////////////////////////////////////////////////////////////////////
// Is n prime? "O(sqrt(n)/3)"
//

bool is_prime(ll n) {
  if (n < 0) return is_prime(-n);
  if (n < 5 || n % 2 == 0 || n % 3 == 0) return (n == 2 || n == 3);
  ll maxP = sqrt(n) + 2;
  for (ll p = 5; p < maxP; p += 6)
    if (n % p == 0 || n % (p+2) == 0) return false;
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// Returns prime number factorization of n. "O(sqrt(n)/3)"
//

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
//  mod(s + k*(m/d), m) for k = 1,...,d-1, where d = gcd(a, m)
// runs in O(log m)

ll solve_mod(ll a, ll b, ll m) {
  if (m < 0) return solve_mod(a, b, -m);
  a = mod(a, m); b = mod(b, m);
  bezout t = find_bezout(a, m, m);
  ll d = (t.first*a) % m;
  if (b % d) return -1;
  else return mod(t.first * (b / d), m);
}

////////////////////////////////////////////////////////////////////////////////
// Fast Exponentiation /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// runs in O(log b)

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

////////////////////////////////////////////////////////////////////////////////
// Sieve ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// runs in O(n log log n)
// returns a vector with the largest prime divisor of each number

vi sieve(int N) {
  vi ans(N, -1); ans[0] = 0; ans[1] = 1;
  for (int i = 2; i < N; i++) if (ans[i] == -1)
    for (int j = i; j < N; j += i) ans[j] = i;
  return ans;
}

// Returns all the primes less than N in O(N log log N)
vi primes(int N) {
  auto div = sieve(N);
  vi ans;
  for (int i = 2; i < N; i++) if (div[i] == i)
    ans.push_back(i);
  return ans;
}

// Computes phi[i] for all numbers < N in O(N log log N)
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

// Computes d[i] (number of divisors) for all numbers < N in O(N log log N)
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

// Computes mu[i] (Mobius function) for all numbers < N in O(N log log N)
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

////////////////////////////////////////////////////////////////////////////////
// Discrete Log ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// discrete log (solves a^x = b (mod N)) in O(sqrt(N)). Returns -1 if no solution
ll discrete_log(ll a, ll b, ll N) {
  ll d = gcd(a, N);
  if (b % d) return -1;
  a /= d; b /= d; N /= d;
  ll sqN = 0;
  for (; sqN*sqN < N; sqN++);
  ll giant = power(find_bezout(a, N, N).first, sqN, N);
  unordered_map<ll, ll> baby;
  ll tmp = 1;
  for (int i = 0; i < sqN; i++) {
    if (!baby.count(tmp)) baby[tmp] = i;
    tmp = (a*tmp) % N;
  }
  for (int i = 0; i < sqN; i++) {
    if (baby.count(b)) return baby[b] + i*sqN;
    b = (giant*b) % N;
  }
  return -1;
}
