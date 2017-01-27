struct simplex { // start: 7d174118d91eb5593c22995dca80cbc5
  int m, n, p, q;
  double s;
  dvet x, y, sx, sy, c;
  ivet N, B;
  mat AT;
  linsys F;

  simplex() {}
  // start: d15c194b4fc4610838c6b313f9dbe507
  simplex(dvet c): c(c), m(0), n(c.m), y(-c) {
    N.m = sy.m = AT.m = n;
    for (int j = 0; j < n; j++) {
      sy[j] = 1. + rand() / double(RAND_MAX);
      N[j] = j;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Adiciona a restrição A*x <= b.
  //
  void constraint(dvet a, double b) { // start: 9f737a109e3d79a8dc3187c0f7432ac1
    for (int j = 0; j < n; j++) AT[j][m] = a[j];
    AT[AT.m++][AT.n++] = 1;
    for (int k = 0; k < AT.m; k++) AT[k].m = AT.n;
    for (int i = 0; i < B.m; i++) b -= a[B[i]] * x[i];
    x[x.m++] = b;
    sx[sx.m++] = 1e-2 * (1. + rand() / double(RAND_MAX));
    B[B.m++] = n + m++;
  }

  // start: 5861445f89c0604edeb597b20d866fd7
  void find_entering(int m, dvet& x, dvet& sx, int& p, int& q) {
    for (int i = 0; i < m; i++) {
      double t = - x[i] / sx[i];
      if (cmp(sx[i]) > 0 && cmp(t, s) > 0) {
        s = t; p = -1; q = i;
      }
    }
  }

  // start: b7cc0a56548af207d7fb4f997f5c3ed8
  int find_leaving(int m, dvet& x, dvet& dx, dvet& sx) {
    int k = -1;
    double r = 0.;
    for (int i = 0; i < m; i++) {
      if (cmp(x[i]) == 0 && cmp(dx[i]) == 0) continue;
      double t = dx[i] / (x[i] + s * sx[i]);
      if (cmp(t, r) > 0) { r = t; k = i; }
    }
    return k;
  }

  void pivot(dvet& x, dvet& dx, int q) {
    double t = x[q] / dx[q]; x -= dx * t; x[q] = t;
  }
  double solve(dvet& r) { // start: bb56d7dff67e75bffe328e16ddc02162
    dvet dx, dy;
    while (true) {
      s = 0.; p = -1; q = -1;
      find_entering(m, x, sx, p, q);
      find_entering(n, y, sy, q, p);
      if (cmp(s) == 0) break;
      F.compile(AT % B);
      if (p != -1) {
        dx = (~F).solve(AT[N[p]]);
        q = find_leaving(m, x, dx, sx);
        if (q == -1) return INFINITY;
        dvet eq(m);
        eq[q] = -1;
        dy = (AT % N) * F.solve(eq);
      } else {
        dvet eq(m);
        eq[q] = -1;
        dy = (AT % N) * F.solve(eq);
        p = find_leaving(n, y, dy, sy);
        if (p == -1) return -INFINITY;
        dx = (~F).solve(AT[N[p]]);
      }
      pivot(x, dx, q); pivot(sx, dx, q);
      pivot(y, dy, p); pivot(sy, dy, p);
      swap(N[p], B[q]);
    }
    r = dvet(n);
    for (int i = 0; i < m; i++)
      if (B[i] < n) r[B[i]] = x[i];
    return c * r;
  }
};
