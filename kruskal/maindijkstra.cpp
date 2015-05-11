#include "../template.cpp"
#include "dijkstra.cpp"

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	graph g(n);
	while (m--) {
		int a, b, c;
		scanf("%d %d %d", &a, &b, &c); // a, b \in [0, n-1]
		g[a].pb({b, c});
		g[b].pb({a, c});
	}
	int init;
	scanf("%d", &init);
	auto dist = dijkstra(g, init);
	fu(i, n) {
		printf("%d: %d\n", i, dist.first[i]);
		int j = i;
		while (j != init) {
			printf("%d ", j);
			j = dist.second[j];
		}
		printf("%d\n", init);
	}
	return 0;
}
