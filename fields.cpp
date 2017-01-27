// start: f9e54f3281994194a926f1d2321b0307  - whole file
typedef pair<point, point> segment;
typedef pair<int, int> barrier;

struct field {
  int n, m;
  vector<point> v;
  vector<barrier> b;
  vector<list<int> > e;
// start: 218f0c471f98553fc60a536353ae8934  -
  void clear(int n) {
    n = n;
    m = 0;
    e.resize(n);
    for (int i = 0; i < n; i++) e[i].clear();
  }
// end
// start: 02ad9bdd5d29850e9bffc84559557fb7  -
  field(vector<point> &v) : n(v.size()), v(v), m(0) {
    e.resize(n);
  }
  inline int ccw(int a, int b, int c) { return ::ccw(v[a], v[b], v[c]); }
  void make_barrier(int i, int j) {
    e[i].push_back(m); e[j].push_back(m);
    b.push_back(barrier(i, j));
  }
// end
  //////////////////////////////////////////////////////////////////////////////
  // Removes degenerate cases
// start: cf43ceadb7acfd7d834cde4ac9fef7b8  -
  void normalize() {
    set<segment> T; set<point> U;
    for (int i = 0; i < n; i++) make_barrier(i, i);
    for (int i = 0; i < m; i++) {
      point p = v[b[i].first], q = v[b[i].second];
      set<point> S;
      S.insert(p); S.insert(q);
//     start: 607a96bdcc52920a615a589a4908cd79  -
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
//     end
      for (auto st = S.begin(); st != S.end(); ++st) {
        if (st != S.begin()) T.insert(segment(p, *st));
        U.insert(p = *st);
      }
    }
    clear(U.size());
    v = vector<point>(U.begin(), U.end());
    for (auto &x : T) {
      int i = lower_bound(all(v), x.first) - v.begin();
      int j = lower_bound(all(v), x.second) - v.begin();
      make_barrier(i, j);
    }
  }
// end

  //////////////////////////////////////////////////////////////////////////////
  // Algoritmo de Poggi-Moreira-Fleischman-Cavalcante.
  //
  // Creates a graph that contains all the edges of an eventual shortest
  // path between points of the field.
  //

  graph<> pmfc() {
    vector<array<int, 2>> sel(n);
    vector<int> active(n);
//     start: e221a5180930f82e53ab0df6ea4d0b0d  -
    for (int i = 0; i < n; i++) {
      vector<pair<double, int>> T;
      for (auto &x : e[i]) {
        int j = b[x].first + b[x].second - i;
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
//     end
    graph<> G(n);
//     start: d85f003300fbef044bba21de34a1ce02  -
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
      G.arc(i, j, 1, norm(v[j] - v[i]));
PROX: ;
    }
//     end
  }
};
// end
