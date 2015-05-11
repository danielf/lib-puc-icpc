#include "../template.cpp"
#include "lca.cpp"

int main() {
	int n;
	scanf("%d", &n);
	vi parent(n);
	fu(_, n-1) {
		int a, b;
		scanf("%d %d", &a, &b); // a, b \in [0, n-1]
		parent[b] = a;
	}
	parent[0] = 0;
	LCA lca(parent, 0);
	int Q;
	scanf("%d", &Q);
	while (Q--) {
		int a, b;
		scanf("%d %d", &a, &b);
		printf("%d\n", lca.get(a, b));
	}
	return 0;
}
