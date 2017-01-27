#undef TRACE
#define TRACE(x)

bool my_lt(const pair<double, int> x, const pair<double, int> y) {
  if (int t = cmp(x.first, y.first)) return t == 1;
  return x.second < y.second;
}

int main() {
  int h = 0;
  int n[3], c[3];
  polygon T[3];

  TRACE(setbuf(stdout, NULL));
  while (scanf(" %d", &n[0]) != EOF && n[0] > 0) {
    T[0].clear();
    T[1].clear();
    T[2].clear();
    scanf(" %d", &c[0]);
    for (int i = 0; i < n[0]; i++) {
      point p;
      scanf(" %lf %lf", &p.x, &p.y);
      T[0].push_back(p);
    }
    scanf(" %d %d", &n[1], &c[1]);
    for (int i = 0; i < n[1]; i++) {
      point p;
      scanf(" %lf %lf", &p.x, &p.y);
      T[1].push_back(p);
    }
    scanf(" %d %d", &n[2], &c[2]);
    for (int i = 0; i < n[2]; i++) {
      point p;
      scanf(" %lf %lf", &p.x, &p.y);
      T[2].push_back(p);
    }
    T[0] = convex_hull(T[0]);
    T[1] = convex_hull(T[1]);
    T[2] = convex_hull(T[2]);
    WATCH(T[0]);
    WATCH(T[1]);
    WATCH(T[2]);
    polygon inter01  = poly_intersect(T[0], T[1]);
    WATCH(inter01);
    polygon inter12  = poly_intersect(T[1], T[2]);
    WATCH(inter12);
    polygon inter20  = poly_intersect(T[2], T[0]);
    WATCH(inter20);
    polygon inter012 = poly_intersect(inter01, T[2]);
    WATCH(inter012);
    double cores[16];
    memset(cores, 0, sizeof(cores));
    double area012 = fabs(poly_area(inter012));
    WATCH(area012);
    double area01 = fabs(poly_area(inter01)) - area012;
    WATCH(area01);
    double area12 = fabs(poly_area(inter12)) - area012;
    WATCH(area12);
    double area20 = fabs(poly_area(inter20)) - area012;
    WATCH(area20);
    double area0 = fabs(poly_area(T[0])) - area01 - area20 - area012;
    WATCH(area0);
    double area1 = fabs(poly_area(T[1])) - area01 - area12 - area012;
    WATCH(area1);
    double area2 = fabs(poly_area(T[2])) - area12 - area20 - area012;
    WATCH(area2);
    cores[(c[0] + c[1] + c[2]) % 16] += area012;
    cores[(c[0] + c[1]) % 16] += area01;
    cores[(c[1] + c[2]) % 16] += area12;
    cores[(c[2] + c[0]) % 16] += area20;
    cores[(c[0]) % 16] += area0;
    cores[(c[1]) % 16] += area1;
    cores[(c[2]) % 16] += area2;
    WATCH(c[0]);
    WATCH(c[1]);
    WATCH(c[2]);
    pair<double, int> cor[16];
    for (int i = 0; i < 16; i++) {
      cor[i] = make_pair(cores[i], i);
    }
    sort(cor, cor + 16, my_lt);
    printf("Instancia %d\n", ++h);
    for (int i = 0; i < 16; i++) {
      if (cmp(cor[i].first) == 0) break;
      printf("%d %.2f\n", cor[i].second, cor[i].first);
    }
    printf("\n");
  }
  return 0;
}
