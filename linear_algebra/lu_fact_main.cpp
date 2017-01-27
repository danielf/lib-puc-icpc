int main() {
  int m, n;
  while (scanf(" %d %d", &m, &n) != EOF && m > 0 && n > 0) {
    mat A(m, n);
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        scanf(" %lf", &A[i][j]);
      }
    }
    dvet b(m);
    for (int i = 0; i < m; i++) {
      scanf(" %lf", &b[i]);
    }
    linsys F(A);
    dvet x = F.solve(b);
    printf("(");
    for (int i = 0; i < x.m; i++) {
      if (i > 0) printf(", ");
      printf("%.2f", cmp(x[i], 0, .5e-2) == 0 ? 0 : x[i]);
    }
    printf(")\n");
  }
  return 0;
}
