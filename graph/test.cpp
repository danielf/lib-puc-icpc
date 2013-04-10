#include "../header.cpp"
#include "graph.cpp"
#define TRACE(x...) x

int main() {
  graph g;
  g.init(3);
  g.arc(0, 2, 12);
  g.arc(0, 1, 30);
  g.arc(1, 2, 10);
  WATCH(sz(g.dest));
  WATCH(sz(g.adj));
  WATCH(g.maxflow(0, 1));
  fu(i, sz(g.flow)) if (g.flow[i] > 0) {
    printf("f(%d, %d) = %d\n", g.orig(i), g.dest[i], g.flow[i]);
  }
  WATCH(g.maxflow(0, 2));
  fu(i, sz(g.flow)) if (g.flow[i] > 0) {
    printf("f(%d, %d) = %d\n", g.orig(i), g.dest[i], g.flow[i]);
  }
}
