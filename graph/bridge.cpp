#include "../template.cpp"
#include "graph.cpp"

int main() {
	graph g(4);
	g.arc(0, 1);
	g.arc(1, 2);
	g.arc(1, 3);
	g.arc(2, 3);
	g.artpbridge();
	for (bool b : g.artp) cout << b;
	cout << endl;
	for (bool b : g.bridge) cout << b;
	cout << endl;
	return 0;
}
