int main() {
	int n, m, h = 0;
	polygon T;
	while (scanf(" %d", &n) != EOF && n > 0) {
		printf("Instancia %d\n", ++h);
		T.clear();
		T.reserve(n);
		for (int i = 0; i < n; i++) {
			point p;
			scanf(" %lf %lf", &p.x, &p.y);
			T.push_back(p);
		}
		point flag;
		scanf(" %lf %lf", &flag.x, &flag.y);
		int flag_inside = in_poly(flag, T);
		scanf(" %d", &m);
		for (int i = 0; i < m; i++) {
			point p;
			scanf(" %lf %lf", &p.x, &p.y);
			int inside = in_poly(p, T);
			bool defender = (inside == -1) ||
			                (flag_inside ? (inside == 1) : (inside == 0));
			printf("soldado %d %s\n", i + 1, \
			       defender ? "defender" : "espanhol");
		}
		printf("\n");
	}
}
