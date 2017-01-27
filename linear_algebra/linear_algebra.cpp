const int TAM = 100;

struct ivet { // start: 747b379fe4b8e112552e1d093715c278
  int m, u[TAM];

  ivet(int m = 0): m(m) {
    for (int i = 0; i < m; i++) u[i] = i;
  }
  int& operator [](int i) { return u[i]; }

  ivet operator ~() {
    ivet v(m);
    for (int i = 0; i < m; i++) v[u[i]] = i;
    return v;
  }
};

struct dvet { // start: ccdca382b418bc5c59ddb48dcae1f57f
  int m; double u[TAM];

  dvet(int m = 0): m(m) {
    memset(u, 0, sizeof(u));
  }
  double& operator [](int i) { return u[i]; }

  dvet operator %(ivet p) {
    dvet r(p.m);
    for (int i = 0; i < p.m; i++) r[i] = u[p[i]];
    return r;
  }

  dvet& operator +=(dvet v) {
    for (int i = 0; i < m; i++) u[i] += v[i];
    return *this;
  }
  dvet operator +(dvet v) { dvet r = v; return r += *this; }
  dvet& operator -=(dvet v) {
    for (int i = 0; i < m; i++) u[i] -= v[i];
    return *this;
  }
  dvet operator *(double t) {
    dvet r(m);
    for (int i = 0; i < m; i++) r[i] = u[i] * t;
    return r;
  }
  dvet operator -() {
    dvet r = *this;
    for (int i = 0; i < m; i++) r[i] = -r[i];
    return r;
  }
  double operator *(dvet v) {
    double r = 0;
    for (int i = 0; i < m; i++) r += u[i] * v[i];
    return r;
  }
};

struct mat { // start: a029cd5c14f17260c81ddd1843fee779
  int m, n; dvet u[TAM];

  mat(int m = 0, int n = 0): m(m), n(n) {
    for (int i = 0; i < m; i++) u[i] = dvet(n);
  }
  dvet& operator [](int i) { return u[i]; }

  mat operator %(ivet p) {
    mat r(p.m, n);
    for (int i = 0; i < p.m; i++) r[i] = u[p[i]];
    return r;
  }
  mat operator ~() {
    mat r(n, m);
    for (int j = 0; j < n; j++)
      for (int i = 0; i < m; i++)
        r[j][i] = u[i][j];
    return r;
  }

  dvet operator *(dvet v) {
    dvet r(m);
    for (int i = 0; i < m; i++) r[i] = u[i] * v;
    return r;
  }
};

////////////////////////////////////////////////////////////////////////////////
// Sistemas lineares ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct linsys { // start: 5f18f344922331d27137e24d6f9b04c7
  ivet P, Q; dvet D; mat L, U;
  int m, n, r;

  void compile (const mat& A) { // start: 98c076ec8e309fd25e8faedb0da81129
    m = A.m; n = A.n;
    P = ivet(m); L = mat(m); D = dvet(); U = A; Q = ivet(n);
    for (r = 0; r < min(m, n); r++) {
      double best = 0; int p, q;
      for (int i = r; i < m; i++) for (int j = r; j < n; j++)
        if (cmp(fabs(U[i][j]), best) > 0)
          { p = i; q = j; best = fabs(U[i][j]); }
      if (cmp(best) == 0) break;
      swap(P[r], P[p]); swap(U[r], U[p]); swap(L[r], L[p]);
      swap(Q[r], Q[q]);
      for (int i = 0; i < m; i++) swap(U[i][r], U[i][q]);
      D[r] = 1 / U[r][r];
      U[r] = U[r] * D[r];
      for (int i = r + 1; i < m; i++) {
        L[i][r] = U[i][r] * D[r];
        U[i] -= U[r] * U[i][r];
      }
      for (int i = r; i < m; i++) U[i][r] = 0;
    }
    for (int i = 0; i < m; i++) L[i].m = r;
    L.n = D.m = U.m = r;
  }

  //////////////////////////////////////////////////////////////////////
  // Encontra uma solução do sistema A * x = b.
  //
  // x.m = 0 caso o sistema seja impossível.
  //

  dvet solve(dvet b) { // start: 6eaec57c7ee31557c847764a1046ed73
    dvet x = b % P;
    for (int i = 0; i < m; i++) x[i] -= L[i] * x;
    for (int i = 0; i < r; i++) x[i] *= D[i];
    for (int i = r; i < m; i++) if (cmp(x[i]) != 0) return dvet();
    x.m = n;
    for (int i = r - 1; i >= 0; i--) x[i] -= U[i] * x;
    x = x % ~Q;
    return x;
  }

  //////////////////////////////////////////////////////////////////////
  // Retorna a fatoração LU de ~A.
  //

  linsys operator ~() { // start: 2c550e512eebad82af0706f5349d08af
    linsys F;
    F.P = Q; F.Q = P; F.D = D; F.L = ~U; F.U = ~L;
    F.m = n; F.n = m; F.r = r;
    return F;
  }
};
