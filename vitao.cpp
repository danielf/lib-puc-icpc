struct cmp3 {
	static double x;
	const vector<segment>& V;
	cmp3(const vector<segment>& V) : V(V) {}
	bool operator() (int i, int j) {
		double y1 = -INFINITY, y2 = -INFINITY;
		fu(k, 3) {
			double pi = (x - V[3*i+k].first.x)/(V[3*i+k].second.x - V[3*i+k].first.x);
			double yi = pi*(V[3*i+k].second.y - V[3*i+k].first.y) + V[3*i+k].first.y;
			double pj = (x - V[3*j+k].first.x)/(V[3*j+k].second.x - V[3*j+k].first.x);
			double yj = pj*(V[3*j+k].second.y - V[3*j+k].first.y) + V[3*j+k].first.y;
			if (cmp(pi) >= 0 && cmp(pi, 1) <= 0) y1 = max(y1, yi);
			if (cmp(pj) >= 0 && cmp(pj, 1) <= 0) y2 = max(y2, yj);
		}
		return make_pair(y1, i) < make_pair(y2, j);
	}
};
double cmp3::x = 0;
const int LV = 18;

bool intriangle(const vector<segment> &V, int i, point p) {
	double ym = INFINITY, yM = -INFINITY;
	double x = p.x;
	fu(k, 3) {
		double pi = (x - V[3*i+k].first.x)/(V[3*i+k].second.x - V[3*i+k].first.x);
		double yi = pi*(V[3*i+k].second.y - V[3*i+k].first.y) + V[3*i+k].first.y;
		if (cmp(pi) >= 0 && cmp(pi, 1) <= 0) {
			ym = min(ym, yi);
			yM = max(yM, yi);
		}
	}
	return cmp(p.y, ym) >= 0 && cmp(p.y, yM) <= 0;
}

int doit(const vector<segment> &V) {
	const int N = V.size() / 3;
	typedef pair<double, int> event;
	vector<event> events;
	fu(i, N) {
		double xl = INFINITY, xr = -INFINITY;
		fu(k, 3) {
			xl = min(xl, V[3*i+k].first.x);
			xr = max(xr, V[3*i+k].first.x);
		}
		events.emplace_back(xl, i);
		events.emplace_back(xr, ~i);
	}
	vector<int> depth(N+1, 0);
	depth[N] = 0;
	sort(all(events));
	cmp3 __(V);
	set<int, cmp3> S(__);
	vector<set<int, cmp3>::iterator> iterators(N, S.end());
	vector<array<int, LV>> lca(N+1);
	fu(i, LV) lca[N][i] = N;

	for(event &E : events){
		cmp3::x = E.first;
		int inx = E.second;
		if (inx >= 0) { // insert
			cmp3::x += EPS;
			auto it = S.insert(inx).first;
			iterators[inx] = it;
			++it;
			int parent = N;
			if (it != S.end()) {
				point p = V[3*inx].first;
				p = min(p, V[3*inx+1].first);
				p = min(p, V[3*inx+2].first);
				int cur = *it;
				if (intriangle(V, cur, p)) parent = cur;
				else for (int k = LV - 1; k >= 0; k--) {
					int pcur = lca[cur][k];
					if (pcur == N || intriangle(V, pcur, p)) parent = pcur;
					else cur = pcur;
				}
			}
			depth[inx] = depth[parent] + 1;
			lca[inx][0] = parent;
			FU(i, 1, LV) lca[inx][i] = lca[lca[inx][i-1]][i-1];
		} else { // erase
			inx = ~inx;
			S.erase(iterators[inx]);
		}
	}
	return 1 + *max_element(all(depth));
}
