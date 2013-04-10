#include "../header.cpp"
#include "graph.cpp"

int main() {
  graph g(2);
  g.arc(0, 1, 12, 3.0);
  g.imb[0] = 10;
  g.imb[1] = -10;
  WATCH(g.mincostflow());
  g.arc(0, 1, 5, 1.0);
  WATCH(g.mincostflow());
  /*
  fu(i, sz(g.flow)) if (g.flow[i] > 0) {
    printf("f(%d, %d) = %d\n", g.orig(i), g.dest[i], g.flow[i]);
  }
  */
}
