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
