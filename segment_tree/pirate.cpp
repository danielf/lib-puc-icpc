#include "../template.cpp"
#include "segtree_lazy.cpp"

int main() {
	int T;
	scanf("%d", &T);
	fu(_42, T) {
		printf("Case %d:\n", _42+1);
		int M;
		scanf("%d", &M);
		vi V;
		while (M--) {
			char buf[60];
			int T2;
			scanf("%d", &T2);
			scanf(" %s", buf);
			fu(_, T2) for (int i = 0; buf[i]; ++i) V.pb(buf[i] - '0');
		}
		segtree_lazy<int, int> S(sz(V));
		S.setup(V);

		int q = 0;
		int Q;
		scanf("%d", &Q);
		while (Q--) {
			char c; int a, b;
			scanf(" %c %d %d", &c, &a, &b);
			if (c == 'F') S.update(a, b, 3);
			else if (c == 'E') S.update(a, b, 2);
			else if (c == 'I') S.update(a, b, 1);
			else printf("Q%d: %d\n", ++q, S.query(a, b));
		}
	}
	return 0;
}
