struct segtree {
  int B, E, C;
  segtree *L, *R;
  double len;

  int a, lbd, rbd; // só para union_perimeter()


  segtree(int b, int e): B(b), E(e), len(0), C(0), a(0), lbd(0), rbd(0) {
    if (E - B > 1) {
      int M = (B + E) / 2;
      L = new segtree(B, M);
      R = new segtree(M, E);
    } else if (E - B == 1) {
      L = new segtree(B, B);
      R = new segtree(E, E);
    } else L = R = NULL;
  }
  ~segtree() { delete L; delete R; }
  void insert(int b, int e) {
    if (e <= B || E <= b || B == E) return;
    if (b <= B && E <= e) C++;
    else { L->insert(b, e); R->insert(b, e); }
    update();
  }
  void erase(int b, int e) {
    if (e <= B || E <= b || B == E) return;
    if (b <= B && E <= e) C--;
    else { L->erase(b, e); R->erase(b, e); }
    update();
  }
  void update();
};

struct rect {
  double x1, y1, x2, y2;
  rect(double x1 = 0, double y1 = 0, double x2 = 0, double y2 = 0): \
    x1(x1), y1(y1), x2(x2), y2(y2) {}
};

const int TAM = 110;

double y[2 * TAM];

void segtree::update() {
  if (C) {
    len = y[E] - y[B];
    a = 2;
    lbd = rbd = 1;
  } else {
    len = L->len + R->len;
    a = L->a + R->a - 2 * L->rbd * R->rbd;
    lbd = L->lbd; rbd = R->rbd;
  }
}

double union_area(vector<rect>& R) {
  int n = R.size(); if (n == 0) return 0;
  vector< pair<double, int> > E;
  int m = 0;
  for (int i = 0; i < n; i++) {
    E.push_back(make_pair(R[i].x1, i));
    E.push_back(make_pair(R[i].x2, ~i));
    y[m++] = R[i].y1;
    y[m++] = R[i].y2;
  }
  sort(all(E)); sort(y, y + m); m = unique(y, y + m, cmp_eq) - y;
  double last = E[0].first, r = 0;
  segtree T(0, m-1);
  for (int i = 0; i < 2*n; i++) {
    int k = E[i].second; bool in = (k >= 0); if (!in) k = ~k;

    double dx = E[i].first - last, dy = T.len;
    r += dx * dy;

    int a = lower_bound(y, y + m, R[k].y1, cmp_lt) - y;
    int b = lower_bound(y, y + m, R[k].y2, cmp_lt) - y;
    if (in) T.insert(a, b);
    else T.erase(a, b);

    last += dx;
  }
  return r;
}

double union_perimeter(vector<rect>& R) {
  int n = R.size(); if (n == 0) return 0;
  vector< pair<double, int> > E;
  int m = 0;
  for (int i = 0; i < n; i++) {
    E.push_back(make_pair(R[i].x1, i));
    E.push_back(make_pair(R[i].x2, ~i));
    y[m++] = R[i].y1;
    y[m++] = R[i].y2;
  }
  sort(all(E)); sort(y, y + m); m = unique(y, y + m, cmp_eq) - y;
  double last = E[0].first, r = 0, dy = 0;
  segtree T(0, m-1);
  for (int i = 0; i < 2*n; i++) {
    int k = E[i].second; bool in = (k >= 0); if (!in) k = ~k;

    double dx = E[i].first - last;
    r += dx * T.a;

    int a = lower_bound(y, y + m, R[k].y1, cmp_lt) - y;
    int b = lower_bound(y, y + m, R[k].y2, cmp_lt) - y;
    if (in) T.insert(a, b);
    else T.erase(a, b);

    r += fabs(dy - T.len);
    dy = T.len;
    last += dx;
  }
  return r;
}
