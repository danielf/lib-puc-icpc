typedef pair<int, int> edge;
struct triangulation: public map<edge, int> {
// start: 02d719f21025dbba2667c0d768278d9b  -
	edge sym(edge e)   { return edge(e.second, e.first); }
	edge lnext(edge e) { return edge(e.second, (*this)[e]); }
	edge lprev(edge e) { return edge((*this)[e], e.first); }
	edge dnext(edge e) { return lprev(sym(lprev(e))); }
	edge dprev(edge e) { return lnext(sym(lnext(e))); }
// end
// start: bd7b82cfb3d9b84a061d514a2b70fe30  -
	void new_tri(edge e, int r) {
		if (count(e)) { erase(lnext(e)); erase(lprev(e)); }
		(*this)[e] = r; (*this)[lnext(e)] = e.first; (*this)[lprev(e)] = e.second;
	}
// end
	// Only needs this function in case you have restrictions
// start: 43f890d105a13253bdaf909c98d4cb7b  -
	void bdsm(field& F, edge e) {
		int a, b, c, d, org = e.first, dest = e.second;
		edge xt;
		stack<edge> pilha;
		if (count(e)) return;
		for (auto it = lower_bound(edge(org, 0)); ; it++) {
			xt = lnext(it->first); a = xt.first, b = xt.second;
			if (b < 0) continue;
			if (seg_intersect(F.v[a], F.v[b], F.v[org], F.v[dest])) break;
		}
// 		start: 575c2666a9c1415b357c02462341e7e8  -
		while (xt.second != dest) {
			pilha.push(xt); xt = sym(xt);
			xt = F.ccw(org, dest, (*this)[xt]) >= 0 ? lnext(xt) : lprev(xt);
			while (!pilha.empty()) {
				edge ee = pilha.top();
				a = ee.first; b = ee.second;
				c = (*this)[ee]; d = (*this)[sym(ee)];
				if (F.ccw(d, c, b) >= 0 || F.ccw(c, d, a) >= 0) break;
				erase(ee); erase(sym(ee)); xt = edge(d, c);
				new_tri(xt, a); new_tri(sym(xt), b);
				pilha.pop();
				xt = F.ccw(org, dest, d) >= 0 ? lprev(xt) : lnext(sym(xt));
			}
		}
// 		end
	}
// end
// start: da517fbac11ebd63a18490399d0c5235  -
// start: d24e04c38f8f9391790bd716f3655350  - without the last 3 lines
	void triangulate(field& F) {
		vector<int> J(F.n);
		int i, k;
		stack<edge> pilha;
		clear();
// 		start: 3a70ce0f34c15a16ea7967cfbfee014f  -
		for (int i = 0; i < F.n; i++) J[i] = i;
		sort(all(J), [&](int a, int b) {
			 return F.v[a] < F.v[b];
			 });
		for (i = 2; i < F.n; i++) if (k = F.ccw(J[0], J[1], J[i])) break;
		if (i >= F.n) return;
		for (int j = 1; j < i; j++) {
			edge e(J[j-1], J[j]);
			new_tri(e, (k > 0) ? J[i] : -1);
			new_tri(sym(e), (k > 0) ? -1 : J[i]);
		}
// 		end
		edge lb(J[i], J[(k > 0) ? i-1 : 0]), ub(J[(k > 0) ? 0 : i-1], J[i]);
// 		start: 6026008724d26f274ae2a1ab6c1e06c9  -
		for (i++; i < F.n; i++) {
			while (F.ccw(lb.first, lb.second, J[i]) >= 0) lb = dprev(lb);
			while (F.ccw(ub.first, ub.second, J[i]) >= 0) ub = dnext(ub);
			for (edge e = dnext(lb); e != ub; e = dnext(e)) pilha.push(e);
			while (!pilha.empty()) {
				new_tri(pilha.top(), J[i]);
				pilha.pop();
			}
			edge e(-1, J[i]);
			new_tri(e, lb.first); new_tri(sym(e), ub.second);
			lb = lnext(e); ub = dnext(lb);
		}
// 		end
		// Only type the following if you have restrictions in your triangulation.
		for (i = 0; i < F.m; i++) {
			bdsm(F, edge(F.b[i].first, F.b[i].second));
		}
	}
// end
// end
};
