////////////////////////////////////////////////////////////////////////////////
// Calcula o maior divisor comum dos números x e y.
//

int gcd(int x, int y) { return y ? gcd(y, x % y) : abs(x); }

////////////////////////////////////////////////////////////////////////////////
// Calcula o mínimo múltiplo comum de a e b.
//

uint64_t lcm(int x, int y) { // start: c20184274646d96dc3f370a78e44b347
	if (x && y) return abs(x) / gcd(x, y) * uint64_t(abs(y));
	else return uint64_t(abs(x | y));
}

////////////////////////////////////////////////////////////////////////////////
// Decide se o inteiro n é primo.
//

bool is_prime(int n) { // start: 1d59b748d9410f82aa5fa23d87c3c246
	if (n < 0) return is_prime(-n);
	if (n < 5 || n % 2 == 0 || n % 3 == 0) return (n == 2 || n == 3);
	int maxP = sqrt(n) + 2;
	for (int p = 5; p < maxP; p += 6)
		if (n % p == 0 || n % (p+2) == 0) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Retorna a fatoração em números primos de abs(n).
//

#include <map>

typedef map<int, int> prime_map;

void squeeze(prime_map& M, int& n, int p) { for (; n % p == 0; n /= p) M[p]++; }

prime_map factor(int n) { // start: 4a7ea2e823d0c6fbcd7f5e5c79bd7431
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
// Teorema de Bézout ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Determina a e b tais que a * x + b * y == gcd(x, y).
//

typedef pair<int, int> bezout;

bezout find_bezout(int x, int y) { // start: 6e2192fec1e96bf033d5c1b76f72fbc7
	if (y == 0) return bezout(1, 0);
	bezout u = find_bezout(y, x % y);
	return bezout(u.second, u.first - (x/y) * u.second);
}

////////////////////////////////////////////////////////////////////////////////
// Acha a menor solução não-negativa de a*x = b (mod m).
//
// Retorna -1 se a congruência for impossível.
//

int mod(int x, int m) { return x % m + (x < 0) ? m : 0; }

int solve_mod(int a, int b, int m) { // start: 69d9721ece52130376af157ce9387a99
	if (m < 0) return solve_mod(a, b, -m);
	if (a < 0 || a >= m || b < 0 || b >= m)
		return solve_mod(mod(a, m), mod(b, m), m);
	bezout t = find_bezout(a, m);
	int d = t.first * a + t.second * m;
	if (b % d) return -1;
	else return mod(t.first * (b / d), m);
}

////////////////////////////////////////////////////////////////////////////////
// Triângulo de Pascal /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

const int TAM = 30;

int C[TAM][TAM];

void calc_pascal() { // start: 1ee7b30816427618a3c96c0de3d7e056
	memset(C, 0, sizeof(C));
	for (int i = 0; i < TAM; i++) {
		C[i][0] = C[i][i] = 1;
		for (int j = 1; j < i; j++)
			C[i][j] = C[i-1][j-1] + C[i-1][j];
	}
}
