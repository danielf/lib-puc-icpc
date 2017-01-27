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
struct segtree {
  vi C; // counts how many times the interval is in

  vd len; // for union_perimeter and union_area
  vi a, lbd, rbd; // only for union_perimeter
  int N;
  segtree(int _N) : N(_N) { // segments in [0, N-1]
    int logN = 2; // very conservative, may use twice as much memory
    while (_N) {logN++; _N >>= 1;}
    int size = 2*(1<<logN) + 1;
    C = vi(size);
    len = vd(size);
    a = vi(size);
    lbd = vi(size);
    rbd = vi(size);
  }
  void ch(int b, int e, int B, int E, int delta, int inx = 1) {
    if (e <= B || E <= b || B == E) return;
    if (b <= B && E <= e) C[inx] += delta;
    else {
      ch(b, e, B, (B+E)/2, delta, 2*inx);
      ch(b, e, (B+E)/2, E, delta, 2*inx+1);
    }
    update(inx, B, E);
  }
  void insert(int b, int e) {
    ch(b, e, 0, N-1, 1);
  }
  void erase(int b, int e) {
    ch(b, e, 0, N-1, -1);
  }
  void update(int inx, int B, int E); // dependent on the algorithm
};

struct rect {
  double x1, y1, x2, y2;
  rect(double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0): \
    x1(x1), y1(y1), x2(x2), y2(y2) {}
};

vd y;

// both for area and perimeter
void segtree::update(int inx, int B, int E) {
  if (C[inx]) {
    len[inx] = y[E] - y[B];
    a[inx] = 2;
    lbd[inx] = rbd[inx] = 1;
  } else {
    len[inx] = len[2*inx] + len[2*inx + 1];
    a[inx] = a[2*inx] + a[2*inx+1] - 2*rbd[2*inx]*rbd[2*inx+1];
    lbd[inx] = lbd[2*inx];
    rbd[inx] = rbd[2*inx+1];
  }
}

bool cmp_eq(double x, double y) {return cmp(x,y) == 0;}
bool cmp_lt(double x, double y) {return cmp(x,y) < 0;}

double union_area(vector<rect>& R) {
  int n = R.size(); if (n == 0) return 0;
  vector< pair<double, int> > E;
  int m = 0;
  y.clear();
  for (int i = 0; i < n; i++) {
    E.push_back(make_pair(R[i].x1, i));
    E.push_back(make_pair(R[i].x2, ~i));
    y.pb(R[i].y1); y.pb(R[i].y2);
  }
  // Coordinate compression
  sort(all(E)); sort(all(y)); m = unique(all(y), cmp_eq) - y.begin();
  y.erase(m + all(y));
  double last = E[0].first, r = 0;
  segtree T(m);
  for (int i = 0; i < 2*n; i++) {
    int k = E[i].second; bool in = (k >= 0); if (!in) k = ~k;

    double dx = E[i].first - last, dy = T.len[1];
    r += dx * dy;

    int a = lower_bound(all(y), R[k].y1, cmp_lt) - y.begin();
    int b = lower_bound(all(y), R[k].y2, cmp_lt) - y.begin();
    if (in) T.insert(a, b);
    else T.erase(a, b);

    last += dx;
  }
  return r;
}

// start: 9c68317bfe979b572fa4ca44a5cbfffc
double union_perimeter(vector<rect>& R) {
  int n = R.size(); if (n == 0) return 0;
  vector< pair<double, int> > E;
  int m = 0;
  y.clear();
  for (int i = 0; i < n; i++) {
    E.push_back(make_pair(R[i].x1, i));
    E.push_back(make_pair(R[i].x2, ~i));
    y.pb(R[i].y1); y.pb(R[i].y2);
  }
  // Coordinate compression
  sort(all(E)); sort(all(y)); m = unique(all(y), cmp_eq) - y.begin();
  y.erase(m + all(y));
  double last = E[0].first, r = 0, dy = 0;
  segtree T(m);
  for (int i = 0; i < 2*n; i++) {
    int k = E[i].second; bool in = (k >= 0); if (!in) k = ~k;

    double dx = E[i].first - last;
    r += dx * T.a[1];

    int a = lower_bound(all(y), R[k].y1, cmp_lt) - y.begin();
    int b = lower_bound(all(y), R[k].y2, cmp_lt) - y.begin();
    if (in) T.insert(a, b);
    else T.erase(a, b);

    r += fabs(dy - T.len[1]);
    dy = T.len[1];
    last += dx;
  }
  return r;
}

int main() {
  int N;
  int _ = 1;
  while (scanf("%d", &N) && N) {
    vector<rect> V(N);
    fu(i, N) scanf("%lf %lf %lf %lf", &V[i].x1, &V[i].y1, &V[i].x2, &V[i].y2);
    printf("Test case #%d\nTotal explored area: %.2f\n\n", _++, union_area(V));
  }
  return 0;
}
