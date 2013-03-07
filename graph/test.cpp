#include "../modelo.cpp"
#include "graph.cpp"

int main() {
  graph g;
  g.init(2);
  g.arc(0, 1, 2, 3.0);
  g.imb[0] = -1;
  g.imb[1] = 1;
  cout << g.mincostflow() << endl;
}
