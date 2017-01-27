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
// Union-find
int P[10010], H[10010];
void init(int N) {
  fu(i, N) {P[i] = i; H[i] = 0;}
}
int rep(int i) {
  if (P[i] != i) P[i] = rep(P[i]);
  return P[i];
}
bool unio(int a, int b) { // returns if union was successful
  a = rep(a); b = rep(b);
  if (a == b) return false;
  if (H[a] < H[b]) P[a] = b;
  else P[b] = a;
  if (H[a] == H[b]) H[a]++;
  return true;
}
// kruskal
typedef double cost; // change to int if necessary
pair<cost, pair<int, int> > edges[1000010]; // (w, (i,j))
cost kruskal(int N, int M) {
  init(N);
  cost ans = 0;
  sort(edges, edges+M);
  fu(i, M) if (unio(edges[i].second.first, edges[i].second.second))
    ans += edges[i].first;
  return ans;
}

pair<double, double> pos[110];
int main() {
  int T;
  scanf("%d", &T);
  fu(t, T) {
    int N;
    scanf("%d", &N);
    fu(i, N) scanf("%lf %lf", &pos[i].first, &pos[i].second);
    int m = 0;
    fu(i, N) fu(j, i) edges[m++] = make_pair(hypot(pos[i].second - pos[j].second, pos[i].first - pos[j].first), make_pair(i,j));
    if (t) printf("\n");
    printf("%.2f\n", kruskal(N, m));
  }
  return 0;
}
