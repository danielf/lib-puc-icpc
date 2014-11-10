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

#define _for(i, a, b) for (__typeof__(a) i = (a); i != (b); ++i)
#define foreach(x...) _for(x)
#define forall(i, v) foreach(i, all(v))
#define FU(i, a, b) for(typeof(a) i = (a); i < (b); ++i)
#define fu(i, n) FU(i, 0, n)

#define mset(c, v) memset(c, v, sizeof(c))
#define mod(a, b) ((((a)%(b))+(b))%(b))
#define pb push_back
#define sz(c) int((c).size())
const int INF = 0x3F3F3F3F; const int NEGINF = 0xC0C0C0C0;
const int NULO = -1; const double EPS = 1e-8;

typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<vi> vvi;

int cmp(double x, double y = 0, double tol = EPS) {
	return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
#include <string.h>

int aux[16][3][100010]; // First dimension should be log_2(MAX_DEGREE)
                        // third should be (2 * MAX_DEGREE)
int MOD; // All operations are done % MOD. Define the MOD
const int LIM = 35; // Magical constant, in case of TLE try playing with it

// It assumes that r[] is initialized with 0, p1[] and p2[] are normalized
// (i.e.: %MOD), g1 = degree(p1), g2 = degree(p2). Don't call with _lvl
// coefficient of x^i is in index i of the array

// Complexity: O(max(g1, g2) ^ lg_2 3) = O(max(g1,g2) ^ 1.58)
void karatsuba(int r[], int g1, int p1[], int g2, int p2[], int _lvl = 0) {
	if (g1 <= LIM || g2 <= LIM) { // if one is small, do O(g1g2)
		for (int i = 0; i <= g1; i++)
			for (int j = 0; j <= g2; j++)
				r[i+j] = (r[i+j] + p1[i]*p2[j])%MOD;
		return;
	}
	int degree = max(g1, g2);
	int degree2 = degree/2;

	// A0*B0
	karatsuba(r, degree2, p1, degree2, p2, _lvl+1);

	// A1*B1
	karatsuba(r+2*degree2+2, g1-degree2-1, p1+degree2+1, g2-degree2-1, p2+degree2+1, _lvl+1);

	// (A0+A1)*(B0+B1)
	for (int i = 0; i <= degree2; i++)
		aux[_lvl][0][i] = (p1[i] + ((i + degree2 + 1 <= g1)?p1[i+degree2+1]:0)) % MOD;
	for (int i = 0; i <= degree2; i++)
		aux[_lvl][1][i] = (p2[i] + ((i + degree2 + 1 <= g2)?p2[i+degree2+1]:0)) % MOD;
	memset(aux[_lvl][2], 0, (degree + 2)*sizeof(int));
	karatsuba(aux[_lvl][2], degree2, aux[_lvl][0], degree2, aux[_lvl][1], _lvl+1);

	// puts result in r[]
	for (int i = 0; i <= 2*degree2; i++)
		aux[_lvl][2][i] = (aux[_lvl][2][i] + 2 * MOD - r[i] - r[i+2*degree2+2]) % MOD;
	for (int i = 0; i <= 2*degree2; i++)
		r[degree2 + i + 1] = (r[degree2 + i + 1] + aux[_lvl][2][i]) % MOD;
}

