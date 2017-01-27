int main() {
  int n, h = 0;
  vector<point> T;
  TRACE(setbuf(stdout, NULL));
  while (scanf(" %d", &n) != EOF && n > 0) {
    T.clear();
    for (int i = 0; i < n; i++) {
      point p;
      scanf(" %lf %lf", &p.x, &p.y);
      T.push_back(p);
    }
    circle c = spanning_circle(T);
    printf("Instancia %d\n", ++h);
    printf("%.2lf %.2lf %.2lf\n", c.first.x, c.first.y, c.second);
    printf("\n");
  }
  return 0;
}
