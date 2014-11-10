////////////////////////////////////////////////////////////////////////////////
// Computes GCD of x and y
//

int gcd(int x, int y) { return y ? gcd(y, x % y) : abs(x); }

////////////////////////////////////////////////////////////////////////////////
// Computes lcm of x and y
//

uint64_t lcm(int x, int y) {
	if (x && y) return abs(x) / gcd(x, y) * uint64_t(abs(y));
	else return uint64_t(abs(x | y));
}

////////////////////////////////////////////////////////////////////////////////
// Is n prime? "O(sqrt(n)/6)"
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
// Returns prime number factorization of n. "O(sqrt(n)/6)"
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
//

typedef pair<int, int> bezout;

bezout find_bezout(int x, int y) {
	if (y == 0) return bezout(1, 0);
	bezout u = find_bezout(y, x % y);
	return bezout(u.second, u.first - (x/y) * u.second);
}

////////////////////////////////////////////////////////////////////////////////
// Finds smallest non-negative solution of a*x = b (mod m).
//
// Returns -1 if there is no solution.
// Let s be the solution found (if != -1). To find other solutions do:
//    s + k*(m/d) for k = 1,...,d-1, where d = gcd(a, m)
//

int mod(int x, int m) { return x % m + (x < 0) ? m : 0; }

int solve_mod(int a, int b, int m) {
	if (m < 0) return solve_mod(a, b, -m);
	if (a < 0 || a >= m || b < 0 || b >= m)
		return solve_mod(mod(a, m), mod(b, m), m);
	bezout t = find_bezout(a, m);
	int d = t.first * a + t.second * m;
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
	for (int i = 0; i < TAM; i++) {
		C[i][0] = C[i][i] = 1;
		for (int j = 1; j < i; j++)
			C[i][j] = C[i-1][j-1] + C[i-1][j];
	}
}
