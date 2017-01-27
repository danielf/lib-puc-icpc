#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<double> vd;

#define TRACE(x...) x
#define PRINT(x...) TRACE(printf(x))
#define WATCH(x) TRACE(cout << #x << " = " << x << endl)

#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()

#define _FOR(it, b, e) for (typeof(b) it = (b); it != (e); ++it)
#define foreach(x...) _FOR(x)
#define fu(i, a) foreach(i, 0, a)
#define forall(i, v) foreach(i, all(v))

#define MSET(c, v) memset(c, v, sizeof(c)

#define pb push_back
#define sz(c) int((c).size())

const int INF = 0x3F3F3F3F; const int NEGINF = 0xC0C0C0C0;
const int NULO = -1; const double EPS = 1e-10;
const double inf = 1e9;

inline int cmp(double x, double y = 0, double tol = EPS) {
  return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
#include <vector>

struct point {
  double x, y;
  point(double x = 0, double y = 0): x(x), y(y) {}

  point operator +(point q) { return point(x + q.x, y + q.y); }
  point operator -(point q) { return point(x - q.x, y - q.y); }
  point operator *(double t) { return point(x * t, y * t); }
  point operator /(double t) { return point(x / t, y / t); }
  double operator *(point q) { return x * q.x + y * q.y; }
  double operator %(point q) { return x * q.y - y * q.x; }

  int cmp(point q) const {
    if (int t = ::cmp(x, q.x)) return t;
    return ::cmp(y, q.y);
  }
  bool operator ==(point q) const { return cmp(q) == 0; }
  bool operator !=(point q) const { return cmp(q) != 0; }
  bool operator < (point q) const { return cmp(q) <  0; }

  friend ostream& operator <<(ostream& o, point p) {
    return o << "(" << p.x << ", " << p.y << ")";
  }

  static point pivot; // only needed for convex hull
};

point point::pivot; // only needed for convex hull

double abs(point p) { return hypot(p.x, p.y); }
double arg(point p) { return atan2(p.y, p.x); }

typedef vector<point> polygon;

// decides type of turn pqr. 1 if left, -1 if right, 0 if straight
inline int ccw(point p, point q, point r) {
  return cmp((p - r) % (q - r));
}

// angle of turn pqr
inline double angle(point p, point q, point r) {
  point u = p - q, v = r - q;
  return atan2(u % v, u * v);
}

////////////////////////////////////////////////////////////////////////////////
// Is q inside [p,r]? (all segments are closed)
//

bool between(point p, point q, point r) {
  return ccw(p, q, r) == 0 && cmp((p - q) * (r - q)) <= 0;
}

////////////////////////////////////////////////////////////////////////////////
// Do segments [p,q] and [r,s] have an intersection?
//

bool seg_intersect(point p, point q, point r, point s) {
  point A = q - p, B = s - r, C = r - p, D = s - q;
  int a = cmp(A % C) + 2 * cmp(A % D);
  int b = cmp(B % C) + 2 * cmp(B % D);
  if (a == 3 || a == -3 || b == 3 || b == -3) return false;
  if (a || b || p == r || p == s || q == r || q == s) return true;
  int t = (p < r) + (p < s) + (q < r) + (q < s);
  return t != 0 && t != 4;
}

////////////////////////////////////////////////////////////////////////////////
// Distance from r to [p,q]
//

double seg_distance(point p, point q, point r) {
  point A = r - q, B = r - p, C = q - p;
  double a = A * A, b = B * B, c = C * C;
  if (cmp(b, a + c) >= 0) return sqrt(a);
  else if (cmp(a, b + c) >= 0) return sqrt(b);
  else return fabs(A % B) / sqrt(c);
}

////////////////////////////////////////////////////////////////////////////////
// Classifies p with respect of polygon T (not necessarily convex)
//
// Returns -1 if on border, 0 if outside, 1 if inside
//

int in_poly(point p, polygon& T) {
  double a = 0; int N = T.size();
  for (int i = 0; i < N; i++) {
    if (between(T[i], p, T[(i+1) % N])) return -1;
    a += angle(T[i], p, T[(i+1) % N]);
  }
  return cmp(a) != 0;
}
////////////////////////////////////////////////////////////////////////////////
// Convex Hull! O(nlogn), *alters the input T*
//

bool radial_lt(point p, point q) {
  point P = p - point::pivot, Q = q - point::pivot;
  double R = P % Q;
  if (cmp(R)) return R > 0;
  return cmp(P * P, Q * Q) < 0;
}

polygon convex_hull(vector<point>& T) {
  int j = 0, k, n = T.size(); polygon U(n);

  point::pivot = *min_element(all(T));
  sort(all(T), radial_lt);
  // If colinear points are part of hull, insert next two lines
  //for (k = n-2; k >= 0 && ccw(T[0], T[n-1], T[k]) == 0; k--);
  //reverse((k+1) + all(T));

  for (int i = 0; i < n; i++) {
    // If colinear points are part, change >= to >
    while (j > 1 && ccw(U[j-1], U[j-2], T[i]) >= 0) j--;
    U[j++] = T[i];
  }
  U.erase(j + all(U));
  return U;
}

////////////////////////////////////////////////////////////////////////////////
// Computes the (oriented) area of T. Positive if counterclockwise
//

double poly_area(polygon& T) {
  double s = 0; int n = T.size();
  for (int i = 0; i < n; i++)
    s += T[i] % T[(i+1) % n];
  return s / 2;
}

////////////////////////////////////////////////////////////////////////////////
// Finds intersection between lines (p,q) and (r,s)
// (Warning: Divides by zero if parallel! Will return NaN or INF in this case)

point line_intersect(point p, point q, point r, point s) {
  point a = q - p, b = s - r, c = point(p % q, r % s);
  return point(point(a.x, b.x) % c, point(a.y, b.y) % c) / (a % b);
}





////////////////////////////////////////////////////////////////////////////////
// Find the smallest circle containing all the points in expected time O(N)
//

typedef pair<point, double> circle; // center, radius

bool in_circle(circle C, point p) { // is point p in circle C?
  return cmp((p - C.first)*(p - C.first), C.second*C.second) <= 0;
}

// Given three (not colinear) points, finds center of circle containing them
point circumcenter(point p, point q, point r) {
  point a = p - r, b = q - r, c = point(a * (p + r) / 2, b * (q + r) / 2);
  return point(c % point(a.y, b.y), point(a.x, b.x) % c) / (a % b);
}

circle spanning_circle(vector<point>& T) {
  int n = T.size();
  random_shuffle(all(T)); // Extremely important!
  circle C(point(), -INFINITY);
  for (int i = 0; i < n; i++) if (!in_circle(C, T[i])) {
    C = circle(T[i], 0);
    for (int j = 0; j < i; j++) if (!in_circle(C, T[j])) {
      C = circle((T[i] + T[j]) / 2, abs(T[i] - T[j]) / 2);
      for (int k = 0; k < j; k++) if (!in_circle(C, T[k])) {
        point o = circumcenter(T[i], T[j], T[k]);
        C = circle(o, abs(o - T[k]));
      }
    }
  }
  return C;
}

////////////////////////////////////////////////////////////////////////////////
// Find the intersection of two CONVEX polygons in LINEAR TIME
//
// P and Q should be in counterclockwise order (their areas should be positive)

polygon poly_intersect(polygon& P, polygon& Q) {
  int m = Q.size(), n = P.size();
  int a = 0, b = 0, aa = 0, ba = 0, inflag = 0;
  polygon R;
  while ((aa < n || ba < m) && aa < 2*n && ba < 2*m) {
    point p1 = P[a], p2 = P[(a+1) % n], q1 = Q[b], q2 = Q[(b+1) % m];
    point A = p2 - p1, B = q2 - q1;
    int cross = cmp(A % B), ha = ccw(p2, q2, p1), hb = ccw(q2, p2, q1);
    if (cross == 0 && ccw(p1, q1, p2) == 0 && cmp(A * B) < 0) {
      if (between(p1, q1, p2)) R.push_back(q1);
      if (between(p1, q2, p2)) R.push_back(q2);
      if (between(q1, p1, q2)) R.push_back(p1);
      if (between(q1, p2, q2)) R.push_back(p2);
      if (R.size() < 2) return polygon();
      inflag = 1; break;
    } else if (cross != 0 && seg_intersect(p1, p2, q1, q2)) {
      if (inflag == 0) aa = ba = 0;
      R.push_back(line_intersect(p1, p2, q1, q2));
      inflag = (hb > 0) ? 1 : -1;
    }
    if (cross == 0 && hb < 0 && ha < 0) return R;
    bool t = cross == 0 && hb == 0 && ha == 0;
    if (t ? (inflag == 1) : (cross >= 0) ? (ha <= 0) : (hb > 0)) {
      if (inflag == -1) R.push_back(q2);
      ba++; b++; b %= m;
    } else {
      if (inflag == 1) R.push_back(p2);
      aa++; a++; a %= n;
    }
  }
  if (inflag == 0) {
    if (in_poly(P[0], Q)) return P;
    if (in_poly(Q[0], P)) return Q;
  }
  R.erase(unique(all(R)), R.end());
  if (R.size() > 1 && R.front() == R.back()) R.pop_back();
  return R;
}

////////////////////////////////////////////////////////////////////////////////
// Fields //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <set>

const int SIZE = 2000;

typedef pair<point, point> segment;
typedef pair<int, int> barrier;

struct field {
  int n, m;
  point v[SIZE];
  barrier b[SIZE];
  list<int> e[SIZE];

  field(): n(0), m(0) {}

  void clear() {
    for (int i = 0; i < n; i++) e[i].clear();
    n = m = 0;
  }

  inline int ccw(int a, int b, int c) { return ::ccw(v[a], v[b], v[c]); }

  void make_barrier(int i, int j) {
    e[i].push_back(m); e[j].push_back(m);
    b[m++] = barrier(i, j);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Removes degenerate cases
  //

  void normalize() {
    set<segment> T; set<point> U;
    for (int i = 0; i < n; i++) make_barrier(i, i);
    for (int i = 0; i < m; i++) {
      point p = v[b[i].first], q = v[b[i].second];
      set<point> S;
      S.insert(p); S.insert(q);
      for (int j = 0; j < m; j++) {
        point r = v[b[j].first], s = v[b[j].second];
        if (r == p || r == q || s == p || s == q) continue;
        if (cmp((q - p) % (s - r)) == 0) {
          if (between(p, r, q)) S.insert(r);
          if (between(p, s, q)) S.insert(s);
        } else if (seg_intersect(p, q, r, s)) {
          S.insert(line_intersect(p, q, r, s));
        }
      }
      foreach (st, all(S)) {
        if (st != S.begin()) T.insert(segment(p, *st));
        U.insert(p = *st);
      }
    }
    clear();
    foreach (it, all(U)) v[n++] = *it;
    foreach (it, all(T)) {
      int i = lower_bound(v, v+n, it->first) - v;
      int j = lower_bound(v, v+n, it->second) - v;
      make_barrier(i, j);
    }
  }
};

////////////////////////////////////////////////////////////////////////////////
// Triangulation ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <map>

typedef pair<int, int> edge;

struct triangulation: public map<edge, int> {
  edge sym(edge e)   { return edge(e.second, e.first); }
  edge lnext(edge e) { return edge(e.second, (*this)[e]); }
  edge lprev(edge e) { return edge((*this)[e], e.first); }
  edge dnext(edge e) { return lprev(sym(lprev(e))); }
  edge dprev(edge e) { return lnext(sym(lnext(e))); }

  void new_tri(edge e, int r) {
    if (count(e)) { erase(lnext(e)); erase(lprev(e)); }
    (*this)[e] = r; (*this)[lnext(e)] = e.first; (*this)[lprev(e)] = e.second;
  }

  void triangulate(field& F) {
    int J[SIZE], i, k, topo = 0;
    edge pilha[SIZE];
    clear();
    for (int i = 0; i < F.n; i++) J[i] = i;
    sort(J, J + F.n, make_index_lt(F.v));
    for (i = 2; i < F.n; i++) if (k = F.ccw(J[0], J[1], J[i])) break;
    if (i >= F.n) return;
    for (int j = 1; j < i; j++) {
      edge e(J[j-1], J[j]);
      new_tri(e, (k > 0) ? J[i] : -1);
      new_tri(sym(e), (k > 0) ? -1 : J[i]);
    }
    edge lb(J[i], J[(k > 0) ? i-1 : 0]), ub(J[(k > 0) ? 0 : i-1], J[i]);
    for (i++; i < F.n; i++) {
      while (F.ccw(lb.first, lb.second, J[i]) >= 0) lb = dprev(lb);
      while (F.ccw(ub.first, ub.second, J[i]) >= 0) ub = dnext(ub);
      for (edge e = dnext(lb); e != ub; e = dnext(e)) pilha[topo++] = e;
      while (topo > 0) new_tri(pilha[--topo], J[i]);
      edge e(-1, J[i]);
      new_tri(e, lb.first); new_tri(sym(e), ub.second);
      lb = lnext(e); ub = dnext(lb);
    }
  }
};
struct graph {

  //////////////////////////////////////////////////////////////////////////////
  // Shared part. Also known as: You will need this!
  //

  vi dest;  // use sz(dest) as nar
  vvi adj;  // use sz(adj) as sz(adj)

  int inv(int a) { return a ^ 0x1; }

  graph(int n = 0) {
    _ini = _end = -1; // only for flows
    adj.resize(n);
    imb.resize(n);
  }

  // Adds an arc to the graph. u is capacity, c is cost.
  // u is only needed on flows, and c only on min-cost-flow
  int arc(int i, int j, int u = 0, double c = 0) {
    dest.pb(j);
    adj[i].pb(sz(dest)-1);
    dest.pb(i);
    adj[j].pb(sz(dest)-1);

    // For both flows
    cap.pb(u);
    cap.pb(0);
    // Only for min cost flow
    cost.pb(c);
    cost.pb(-c);

    return sz(dest)-2;
  }

  //////////////////////////////////////////////////////////////////////////////
  // For both flows!!
  //

  vi cap, flow;
  int _ini, _end;   // ini, end of last maxflow

  int orig(int a) { return dest[inv(a)]; }
  int capres(int a) { return cap[a] - flow[a]; }

  //////////////////////////////////////////////////////////////////////////////
  // Max Flow! - Dinic O(n^2 * m) incremental
  // don't call maxflow with ini == end
  //

  vi d, curAdj;

  bool MFbfs(int s, int t) {
    d = vi(sz(adj), INF);
    curAdj = vi(sz(adj));
    d[s] = 0;
    queue<int> Q; Q.push(s);
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      forall(i, adj[u]) {
        int v = dest[*i];
        if (capres(*i) > 0 && d[v] == INF) {
          d[v] = d[u] + 1; Q.push(v);
        }
      }
    }
    return d[t] != INF;
  }

  int MFdfs(int u, int t, int f) {
    if (u == t) return f;
    for(int &i = curAdj[u]; i < adj[u].size(); ++i) {
      int ar = adj[u][i], v = dest[ar];
      if (d[v] != d[u]+1 || capres(ar) == 0) continue;
      int tmpF = MFdfs(v, t, min(f, capres(ar)));
      if (tmpF) {
        flow[ar] += tmpF;
        flow[inv(ar)] -= tmpF;
        return tmpF;
      }
    }
    return 0;
  }

  int maxflow(int ini, int end) {
    if (_ini != ini || _end != end) {
      flow = vi(sz(dest));
      _ini = ini;
      _end = end;
    }
    while (MFbfs(ini, end))
      while (MFdfs(ini, end, INF));
    int F = 0;
    forall(a, adj[ini]) F += flow[*a];
    return F;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Min Cost Flow! - O(m^2 * log n * log U) incremental
  // Don't forget to specify the [imb]
  // look at [imb] for feasibility
  //

  vi imb;
  vd cost, pot;
  int delta;

  double rescost(int a) {
    return cost[a] + pot[orig(a)] - pot[dest[a]];
  }

  bool dijkstra() {
    priority_queue<pair<double, pair<int, int> > > q;
    vi ent(sz(adj), -2);
    vd dist(sz(adj), INF);
    fu(u, sz(adj)) if (imb[u] >= delta)
      q.push(make_pair(0.0, make_pair(u, -1)));

    while (!q.empty()) {
      int u = q.top().second.first, f = q.top().second.second;
      double d = -q.top().first; q.pop();
      if (ent[u] != -2) continue; dist[u] = d; ent[u] = f;
      forall(a, adj[u]) if (capres(*a) >= delta)
        q.push(make_pair(-(dist[u] + rescost(*a)), make_pair(dest[*a], *a)));
    }

    fu(u, sz(adj)) if (ent[u] != -2 && imb[u] <= -delta) {
      fu(v, sz(adj)) pot[v] += dist[v];
      for (int a = ent[u]; a != -1; a = ent[orig(a)]) {
        flow[a] += delta;
        flow[inv(a)] -= delta;
        imb[dest[a]] += delta;
        imb[orig(a)] -= delta;
      }
      return true;
    }
    return false;
  }

  double mincostflow() {
    pot.resize(sz(adj));
    flow.resize(sz(dest));
    for (delta = 0x40000000; delta > 0; delta /= 2) {
      fu(a, sz(dest)) {
        int u = orig(a), v = dest[a];
        if (capres(a) >= delta && cmp(rescost(a)) < 0) {
          imb[u] -= capres(a);
          imb[v] += capres(a);
          flow[inv(a)] -= capres(a);
          flow[a] += capres(a);
        }
      }
      while (dijkstra());
    }
    double C = 0.0;
    fu(a, sz(dest)) if (flow[a] > 0) C += flow[a] * cost[a];
    return C;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Both Bridges/Articulation points and to Strongly Connected Components
  //

  vi depth;

  //////////////////////////////////////////////////////////////////////////////
  // Bridges and articulation points - O(n+m)
  //

  vector<bool> artp, bridge;
  vi least;

  int dfs_artpbridge(int u, int ent) {
    int nf = 0;
    forall(a, adj[u]) {
      int v = dest[*a];
      if (depth[v] == -1) {
        least[v] = depth[v] = depth[u] + 1;
        dfs_artpbridge(v, *a); nf++;

        if (least[v] >= depth[u]) {
          artp[u] = true;
          if (least[v] == depth[v]) bridge[*a] = bridge[inv(*a)] = true;
        } else least[u] = min(least[u], least[v]);
      }
      else if (inv(*a) != ent) least[u] = min(least[u], depth[v]);
    }
    return nf;
  }

  void partponte() {
    artp = vector<bool>(sz(adj), false);
    bridge = vector<bool>(sz(dest), false);
    depth = vi(sz(adj), -1);
    least = vi(sz(adj), -1);
    fu(i, sz(adj)) if (depth[i] == -1) {
        least[i] = depth[i] = 0;
        if (dfs_artpbridge(i, -1) < 2) artp[i] = false;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Strongly Connected Components - O(n+m)
  // see [rep] for results
  //

  vi ord, rep;

  int transp(int a) { return (a & 0x1); }

  void dfs_topsort(int u) {
    forall(a, adj[u]) {
      int v = dest[*a];
      if (!transp(*a) && depth[v] == -1) {
        depth[v] = depth[u] + 1;
        dfs_topsort(v);
      }
    }
    ord.pb(u);
  }

  void dfs_compfortcon(int u, int ent) {
    rep[u] = ent;
    forall(a, adj[u]) {
      int v = dest[*a];
      if (transp(*a) && rep[v] == -1) dfs_compfortcon(v, ent);
    }
  }

  void compfortcon() {
    depth = vi(sz(adj), -1);
    ord.clear();
    fu(u, sz(adj)) if (depth[u] == -1) {
        depth[u] = 0;
        dfs_topsort(u);
    }

    rep = vi(sz(adj), -1);
    for (int i = sz(adj)-1; i >= 0; i--) if (rep[ord[i]] == -1)
      dfs_compfortcon(ord[i], ord[i]);
  }
};
#include <list>
#include <set>

const int TAM = 2000;

typedef pair<point, point> segment;
typedef pair<int, int> barrier;

struct field {
  int n, m;
  point v[TAM];
  barrier b[TAM];
  list<int> e[TAM];

  field(): n(0), m(0) {}

  void clear() {
    for (int i = 0; i < n; i++) e[i].clear();
    n = m = 0;
  }

  inline int ccw(int a, int b, int c) { return ::ccw(v[a], v[b], v[c]); }

  void make_barrier(int i, int j) {
    e[i].push_back(m); e[j].push_back(m);
    b[m++] = barrier(i, j);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Remove os casos degenerados de um campo.
  //

  void normalize() {
    set<segment> T; set<point> U;
    for (int i = 0; i < n; i++) make_barrier(i, i);
    for (int i = 0; i < m; i++) {
      point p = v[b[i].first], q = v[b[i].second];
      set<point> S;
      S.insert(p); S.insert(q);
      for (int j = 0; j < m; j++) {
        point r = v[b[j].first], s = v[b[j].second];
        if (r == p || r == q || s == p || s == q) continue;
        if (cmp((q - p) % (s - r)) == 0) {
          if (between(p, r, q)) S.insert(r);
          if (between(p, s, q)) S.insert(s);
        } else if (seg_intersect(p, q, r, s)) {
          S.insert(line_intersect(p, q, r, s));
        }
      }
      foreach (st, all(S)) {
        if (st != S.begin()) T.insert(segment(p, *st));
        U.insert(p = *st);
      }
    }

    clear();
    foreach (it, all(U)) v[n++] = *it;
    foreach (it, all(T)) {
      int i = lower_bound(v, v+n, it->first) - v;
      int j = lower_bound(v, v+n, it->second) - v;
      make_barrier(i, j);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Algoritmo de Poggi-Moreira-Fleischman-Cavalcante.
  //
  // Determina um grafo que contém todas as arestas de um eventual menor
  // caminho entre pontos do grafo.
  //

  void pmfc(graph& G) {
    int sel[TAM][2], active[TAM];
    for (int i = 0; i < n; i++) {
      vector< pair<double, int> > T;
      foreach (it, all(e[i])) {
        int j = b[*it].first + b[*it].second - i;
        T.push_back(make_pair(arg(v[j] - v[i]), j));
      }
      sort(all(T));
      if (T.empty()) T.push_back(make_pair(0, i));
      active[i] = 0;
      int p = T.size();
      for (int j = 0; j < p; j++) {
        sel[i][0] = T[j].second; sel[i][1] = T[(j+1) % p].second;
        if (ccw(sel[i][0], sel[i][1], i) <= 0) {
          active[i] = 1; break;
        }
      }
    }
    G.init(n);
    for (int i = 0; i < n; i++) for (int j = 0; j < i; j++) {
      if (!active[i] || !active[j]) continue;
      if (ccw(i, j, sel[i][0]) * ccw(i, j, sel[i][1]) == -1 || \
          ccw(i, j, sel[j][0]) * ccw(i, j, sel[j][1]) == -1)
        continue;
      for (int k = 0; k < m; k++) {
        int org = b[k].first, dest = b[k].second;
        if (org == i || org == j || dest == i || dest == j) continue;
        if (seg_intersect(v[i], v[j], v[org], v[dest])) goto PROX;
      }
      G.aresta(i, j, 1, abs(v[j] - v[i]));
PROX: ;
    }
  }
};
template <class T>
struct index_lt {
  T& v;
  index_lt(T& v): v(v) {}
  inline bool operator ()(int i, int j) {
    return (v[i] != v[j]) ? (v[i] < v[j]) : (i < j);
  }
};
template <class T> index_lt<T> make_index_lt(T& v) { return index_lt<T>(v); }

bool cmp_eq(double x, double y) { return cmp(x, y) == 0; }
bool cmp_lt(double x, double y) { return cmp(x, y) < 0; }

int safe_gets(char*& s) { // depois de usar, free(s);
  return scanf("%a[^\r\n]%*[\r\n]", &s);
}
#include <map>

typedef pair<int, int> edge;

struct triangulation: public map<edge, int> {
  edge sym(edge e)   { return edge(e.second, e.first); }
  edge lnext(edge e) { return edge(e.second, (*this)[e]); }
  edge lprev(edge e) { return edge((*this)[e], e.first); }
  edge dnext(edge e) { return lprev(sym(lprev(e))); }
  edge dprev(edge e) { return lnext(sym(lnext(e))); }

  void new_tri(edge e, int r) {
    if (count(e)) { erase(lnext(e)); erase(lprev(e)); }
    (*this)[e] = r; (*this)[lnext(e)] = e.first; (*this)[lprev(e)] = e.second;
  }

  // Digite esta função apenas para triangulações com restrições.
  void bdsm(field& F, edge e) {
    int a, b, c, d, org = e.first, dest = e.second, topo = 0;
    edge xt;
    edge pilha[TAM];
    if (count(e)) return;
    for (iterator it = lower_bound(edge(org, 0)); ; it++) {
      xt = lnext(it->first); a = xt.first, b = xt.second;
      if (b < 0) continue;
      if (seg_intersect(F.v[a], F.v[b], F.v[org], F.v[dest])) break;
    }
    while (xt.second != dest) {
      pilha[topo++] = xt; xt = sym(xt);
      xt = F.ccw(org, dest, (*this)[xt]) >= 0 ? lnext(xt) : lprev(xt);
      while (topo > 0) {
        edge ee = pilha[topo-1];
        a = ee.first; b = ee.second;
        c = (*this)[ee]; d = (*this)[sym(ee)];
        if (F.ccw(d, c, b) >= 0 || F.ccw(c, d, a) >= 0) break;
        erase(ee); erase(sym(ee)); xt = edge(d, c);
        new_tri(xt, a); new_tri(sym(xt), b);
        topo--;
        xt = F.ccw(org, dest, d) >= 0 ? lprev(xt) : lnext(sym(xt));
      }
    }
  }

  void triangulate(field& F) {
    int J[TAM], i, k, topo = 0;
    edge pilha[TAM];
    clear();
    for (int i = 0; i < F.n; i++) J[i] = i;
    sort(J, J + F.n, make_index_lt(F.v));
    for (i = 2; i < F.n; i++) if (k = F.ccw(J[0], J[1], J[i])) break;
    if (i >= F.n) return;
    for (int j = 1; j < i; j++) {
      edge e(J[j-1], J[j]);
      new_tri(e, (k > 0) ? J[i] : -1);
      new_tri(sym(e), (k > 0) ? -1 : J[i]);
    }
    edge lb(J[i], J[(k > 0) ? i-1 : 0]), ub(J[(k > 0) ? 0 : i-1], J[i]);
    for (i++; i < F.n; i++) {
      while (F.ccw(lb.first, lb.second, J[i]) >= 0) lb = dprev(lb);
      while (F.ccw(ub.first, ub.second, J[i]) >= 0) ub = dnext(ub);
      for (edge e = dnext(lb); e != ub; e = dnext(e)) pilha[topo++] = e;
      while (topo > 0) new_tri(pilha[--topo], J[i]);
      edge e(-1, J[i]);
      new_tri(e, lb.first); new_tri(sym(e), ub.second);
      lb = lnext(e); ub = dnext(lb);
    }
    // Digite esta linha somente para triangulações com restrições.
    for (i = 0; i < F.m; i++) {
      bdsm(F, edge(F.b[i].first, F.b[i].second));
    }
  }
};
int main() {
  while (1) {
    char *buf;
    scanf(" %a[^\n]", &buf);
    if (buf[0] == '#') {
      free(buf);
      break;
    }
    point p[4];
    sscanf(buf, " %lf %lf %lf %lf %lf %lf %lf %lf", \
      &p[0].x, &p[0].y, &p[1].x, &p[1].y, \
      &p[2].x, &p[2].y, &p[3].x, &p[3].y);
    bool r = seg_intersect(p[0], p[1], p[2], p[3]);
    printf("%s\n", r ? "true" : "false");
    free(buf);
  }
  return 0;
}
