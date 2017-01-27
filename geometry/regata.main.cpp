#include <set>

field F;
graph G;

double xi, yi, xf, yf;
int N;

struct dist_lt {
  bool operator ()(const int a, const int b) const {
    if (int u = cmp(G.dist[a], G.dist[b])) return u < 0;
    return a < b;
  }
};

int marc[300];

void dijkstra(int ini) {
  //set<int, dist_lt> Q;
  set<int, index_lt<double[1010]> > Q(make_index_lt(G.dist));
  //set<int> Q = set<int>(index_lt<double[1010]>(G.dist));

  Q.clear();
  memset(marc, 0, sizeof(marc));

  for (int i = 0; i < G.nvt; i++) {
    G.dist[i] = INFINITY;
  }

  G.dist[ini] = 0;
  Q.insert(ini); marc[ini] = 1;
  while (!Q.empty()) {
    int no = *Q.begin();
    Q.erase(no);
    for (int p = 0; p < G.nadj[no]; p++) {
      int ar = G.adj[no][p];
      int viz = G.dest[ar];
      double ndist = G.dist[no] + fabs(G.custo[ar]);
      if (!marc[viz]) {
        marc[viz] = 1;
        Q.insert(viz);
      }
      if (cmp(ndist, G.dist[viz]) < 0) {
        Q.erase(viz);
        G.dist[viz] = ndist;
        Q.insert(viz);
      }
    }
  }
}

int main() {
  while (scanf(" %lf %lf %lf %lf %d", &xi, &yi, &xf, &yf, &N) != EOF && \
         !(cmp(xi) == 0 && cmp(yi) == 0 && cmp(xf) == 0 && cmp(yf) == 0 && \
           N == 0)) {
    F.clear();
    F.v[F.n++] = point(xi, yi);
    F.v[F.n++] = point(xf, yf);
    for (int i = 0; i < N; i++) {
      double x[2], y[2];
      scanf(" %lf %lf %lf %lf", &x[0], &y[0], &x[1], &y[1]);
      F.v[F.n++] = point(x[0], y[0]);
      F.v[F.n++] = point(x[1], y[1]);
      F.make_barrier(F.n-2, F.n-1);
    }
    F.pmfc(G);
    dijkstra(0);
    printf("%.2f\n", G.dist[1]);
  }
  return 0;
}
