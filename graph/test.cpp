#include "../header.cpp"
#include "graph.cpp"

int main() {
  graph g;
  g.arc(0, 2, 12);
  g.arc(0, 1, 30);
  g.arc(1, 2, 10);
  WATCH(sz(g.dest));
  WATCH(sz(g.adj));
  WATCH(g.maxflow(0, 1));
  fu(i, sz(g.dest)) if (g.flow[i] > 0) {
    printf("f(%d, %d) = %d\n", g.orig(i), g.dest[i], g.flow[i]);
  }
  WATCH(g.maxflow(1, 2));
  fu(i, sz(g.dest)) if (g.flow[i] > 0) {
    printf("f(%d, %d) = %d\n", g.orig(i), g.dest[i], g.flow[i]);
  }
}
