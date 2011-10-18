#include <queue>

point border[1010], inform[1010], rc[40];
double rr[40];
int B, N, M;
int valid[1010];
set<edge> filled;

field F;
triangulation T;

void build_field() {
	F.clear();
	for (int i = 0; i < M; i++) {
		F.v[i] = rc[i];
		for (int j = 0; j < i; j++) {
			if (cmp(abs(rc[i] - rc[j]), rr[i] + rr[j]) <= 0)
				F.make_barrier(i, j);
		}
	}
	F.v[M] = point(2000, 2000);
	F.n = M + 1;
}

void flood_fill() {
	if (T.empty()) return;
	filled.clear();
	queue<edge> Q;
	edge e = T.begin()->first;
	Q.push(e);
	filled.insert(e);
	while (!Q.empty()) {
		e = Q.front(); Q.pop();

		if (!filled.count(T.lnext(e))) {
			Q.push(T.lnext(e));
			filled.insert(T.lnext(e));
		}

		if (e.first == -1) continue;

		edge f = T.sym(e);
		if (find(F.b, F.b + F.m, e) == F.b + F.m &&
		    find(F.b, F.b + F.m, f) == F.b + F.m &&
		    !filled.count(f)) {
			Q.push(f);
			filled.insert(f);
		}
	}
}

void find_informers() {
	for (int i = 0; i < N; i++) {
		valid[i] = 1;
		for (int j = 0; j < M; j++) {
			if (cmp(abs(rc[j] - inform[i]), rr[j]) <= 0) {
				valid[i] = 0;
				goto MORREU;
			}
		}
		foreach (it, all(T)) {
			int p = it->first.first;
			int q = it->first.second;
			int r = it->second;
			edge e = it->first;
			if (r < p || r < q) continue;
			if (p == -1 || q == -1 || r == -1) continue;
			if (ccw(F.v[p], F.v[q], inform[i]) >= 0 && \
			    ccw(F.v[q], F.v[r], inform[i]) >= 0 && \
			    ccw(F.v[r], F.v[p], inform[i]) >= 0) {
				if (filled.count(e)) {
					valid[i] = 0;
					goto MORREU;
				}
				break;
			}
		}
MORREU:;
	}
}

int main() {
	while (true) {
		scanf(" %d", &B);
		for (int i = 0; i < B; i++) {
			scanf(" %lf %lf", &border[i].x, &border[i].y);
		}
		scanf(" %d", &N);
		for (int i = 0; i < N; i++) {
			scanf(" %lf %lf", &inform[i].x, &inform[i].y);
		}
		scanf(" %d", &M);
		for (int i = 0; i < M; i++) {
			scanf(" %lf %lf %lf", &rc[i].x, &rc[i].y, &rr[i]);
		}
		if (B == 0 && N == 0 && M == 0) break;
		build_field();
		F.normalize();
		T.triangulate(F);
		flood_fill();
		find_informers();
		int best = -1; double bestD = -1.;
		for (int i = 0; i < N; i++) {
			if (!valid[i]) continue;
			double curD = 1e100;
			for (int j = 0; j < B; j++) {
				double t = seg_distance(border[j], border[(j+1) % B], inform[i]);
				curD = min(t, curD);
			}
			if (cmp(curD, bestD) > 0) {
				best = i;
				bestD = curD;
			}
		}
		if (best == -1) {
			printf("Mission impossible\n");
		}
		else {
			printf("Contact informer %d\n", best + 1);
		}
	}
	return 0;
}
