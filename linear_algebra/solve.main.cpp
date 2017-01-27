int main() {
  int h = 0, i, j, m, n;
  while (scanf(" %d %d", &n, &m) && m > 0 && n > 0) {

    dvet c(n);
    for (i = 0; i < n; i++) {
      scanf(" %lf", &c[i]);
    }
    simplex S(c);

    for (i = 0; i < m; i++) {
      dvet a(n);
      double b;
      for (j = 0; j < n; j++) {
        scanf(" %lf", &a[j]);
      }
      scanf(" %lf", &b);
      S.constraint(a, b);
    }

    dvet x;
    double z = S.solve(x);

    printf("#%d: %.2lf:", ++h, cmp(z, 0, .5e-2) ? z : 0);
    for (i = 0; i < n; i++)
      printf(" %.2lf", cmp(x[i], 0, .5e-2) ? x[i] : 0);
    printf("\n");
  }
  return 0;
}
