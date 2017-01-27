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

// given a series of congruences x = y mod m,
// returns the value of y' for x = y' mod N, with N = m1*m2*...*mn
// or -1 if impossible (that is, gcd of m1,m2,...,mn is not 1)
//
// Runs in O(n*log(N))

typedef pair<long long int, long long int> bezout;

int gcd(int a, int b) {
  if (a < b) return gcd(b, a);
  if (b == 0) return a;
  return gcd(b, a%b);
}

int lcm(int a, int b) {
  return a*(b/gcd(a,b));
}

bezout find_bezout_mod(int x, int y, int m) {
  if (y == 0) return bezout(1, 0);
  bezout u = find_bezout_mod(y, x % y, m);
  return bezout(u.second, (u.first + m - ((x / y) * u.second) % m) % m);
}

int CRT(vector< pair<int,int> > &v) { // vector of (remainder, modulo)
  int e[v.size()], N = 1, g = 0;
  for (int i=0; i < (int)v.size(); i++) {
    N = lcm(N, v[i].second);
    g = gcd(g, v[i].second);
  }
  if (g != 1) return -1;

  for (int i=0; i < (int)v.size(); i++) {
    int ni = v[i].second, N_ni = N / ni;
    e[i] = 0;
    int d = find_bezout_mod(ni, N_ni, ni).second;
    e[i] = ((long long)d * N_ni) % N;
  }

  int x = 0;
  for (int i=0; i < (int)v.size(); i++) {
    x += ((long long)v[i].first * e[i]) % N;
    x %= N;
  }
  return x;
}

int main() {
  int a, b, c, n;
  int _ = 0;
  while (scanf("%d %d %d %d", &a, &b, &c, &n) && (a != -1 || b != -1 || c != -1 || n != -1) ) {
    _++;
    vector<pair<int, int> > V(3);
    V[0] = make_pair( ((a%23)+23)%23, 23);
    V[1] = make_pair( ((b%28)+28)%28, 28);
    V[2] = make_pair( ((c%33)+33)%33, 33);
    int K = CRT(V);
    int M = 23*28*33;
    int ans = (((K - n)%M)+M)%M;
    if (ans == 0) ans = M;
    printf("Case %d: the next triple peak occurs in %d days.\n", _, ans);
  }
  return 0;
}
