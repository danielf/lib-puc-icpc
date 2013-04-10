#include "../header.cpp"
#include "graph.cpp"

void test_mincostflow() {
  graph g(2);
  g.arc(0, 1, 12, 3.0);
  g.imb[0] = 10;
  g.imb[1] = -10;
  WATCH(g.mincostflow());
  g.arc(0, 1, 5, 1.0);
  WATCH(g.mincostflow());
  fu(i, sz(g.flow)) if (g.flow[i] > 0) {
    printf("f(%d, %d) = %d\n", g.orig(i), g.dest[i], g.flow[i]);
  }
}

void test_artpbridge() {
  graph g(7);
  g.arc(0, 1);
  g.arc(0, 2);
  g.arc(1, 2);
  g.arc(1, 3);
  g.arc(1, 4);
  g.arc(2, 5);
  g.arc(3, 4);
  g.arc(5, 6);
  g.partponte();
  fu(u, sz(g.artp))
    printf("node %d artp: %s\n", u, g.artp[u]?"yes":"no");
  fu(a, sz(g.bridge)) if (!g.transp(a))
    printf("arc (%d, %d) bridge: %s\n", g.orig(a), g.dest[a], g.bridge[a]?"yes":"no");
}

void test_scc() {
  graph g(4);
  g.arc(0, 3);
  g.arc(3, 0);
  g.arc(1, 2);
  g.arc(2, 1);
  g.arc(1, 0);
  g.compfortcon();
  fu(u, sz(g.rep))
    printf("node %d with rep %d\n", u, g.rep[u]);
}

void test_2sat() {
  graph g(4);
  g.clause(g.fals(0), g.fals(0));
  g.clause(g.tru(0), g.tru(0));
  WATCH(g.twosat());
}

int main() {
  test_2sat();
}
