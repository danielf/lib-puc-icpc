#include <vector>

struct point { // start: 413879820b25ba789fc4bb50112049fa
	double x, y;
	point(double x = 0, double y = 0): x(x), y(y) {}

	point operator +(point q) { return point(x + q.x, y + q.y); }
	point operator -(point q) { return point(x - q.x, y - q.y); }
	point operator *(double t) { return point(x * t, y * t); }
	point operator /(double t) { return point(x / t, y / t); }
	double operator *(point q) { return x * q.x + y * q.y; }
	double operator %(point q) { return x * q.y - y * q.x; }

	int cmp(point q) const {
		if (int t = ::cmp(x, q.x)) return t;
		return ::cmp(y, q.y);
	}
	bool operator ==(point q) const { return cmp(q) == 0; }
	bool operator !=(point q) const { return cmp(q) != 0; }
	bool operator < (point q) const { return cmp(q) <  0; }
	
	friend ostream& operator <<(ostream& o, point p) {
		return o << "(" << p.x << ", " << p.y << ")";
	}
	
	static point pivot;
}; // end: 413879820b25ba789fc4bb50112049fa

point point::pivot;

double abs(point p) { return hypot(p.x, p.y); }
double arg(point p) { return atan2(p.y, p.x); }

typedef vector<point> polygon;

inline int ccw(point p, point q, point r) {
	return cmp((p - r) % (q - r));
}

inline double angle(point p, point q, point r) {
	point u = p - q, v = r - q;
	return atan2(u % v, u * v);
}

////////////////////////////////////////////////////////////////////////////////
// Decide se q está sobre o segmento fechado pr.
// 

bool between(point p, point q, point r) {
	return ccw(p, q, r) == 0 && cmp((p - q) * (r - q)) <= 0;
}

////////////////////////////////////////////////////////////////////////////////
// Decide se os segmentos fechados pq e rs têm pontos em comum.
//

// start: f4f340136d8a306387ae55ab610c8d3b
bool seg_intersect(point p, point q, point r, point s) {
	point A = q - p, B = s - r, C = r - p, D = s - q;
	int a = cmp(A % C) + 2 * cmp(A % D);
	int b = cmp(B % C) + 2 * cmp(B % D);
	if (a == 3 || a == -3 || b == 3 || b == -3) return false;
	if (a || b || p == r || p == s || q == r || q == s) return true;
	int t = (p < r) + (p < s) + (q < r) + (q < s);
	return t != 0 && t != 4;
}

////////////////////////////////////////////////////////////////////////////////
// Calcula a distância do ponto r ao segmento pq.
// 

// start: ba743bfad8dc2b93f3279ca3c4f81c36
double seg_distance(point p, point q, point r) {
	point A = r - q, B = r - p, C = q - p;
	double a = A * A, b = B * B, c = C * C;
	if (cmp(b, a + c) >= 0) return sqrt(a);
	else if (cmp(a, b + c) >= 0) return sqrt(b);
	else return fabs(A % B) / sqrt(c);
}

////////////////////////////////////////////////////////////////////////////////
// Classifica o ponto p em relação ao polígono T.
// 
// Retorna 0, -1 ou 1 dependendo se p está no exterior, na fronteira
// ou no interior de T, respectivamente.
// 

int in_poly(point p, polygon& T) { // start: 6c0f6be91a4c1ae192a1e0c14ef8b4cd
	double a = 0; int N = T.size();
	for (int i = 0; i < N; i++) {
		if (between(T[i], p, T[(i+1) % N])) return -1;
		a += angle(T[i], p, T[(i+1) % N]);
	}
	return cmp(a) != 0;
}
////////////////////////////////////////////////////////////////////////////////
// Comparação radial.
//

bool radial_lt(point p, point q) { // start: 0ba3640bb5232244bc3107fd3fd6642f
	point P = p - point::pivot, Q = q - point::pivot;
	double R = P % Q;
	if (cmp(R)) return R > 0;
	return cmp(P * P, Q * Q) < 0;
}

////////////////////////////////////////////////////////////////////////////////
// Determina o fecho convexo de um conjunto de pontos no plano.
//
// Destrói a lista de pontos T.
// 

// start: a7e1f8afc8a060530a83051335a145c7
polygon convex_hull(vector<point>& T) {
	int j = 0, k, n = T.size(); polygon U(n);

	point::pivot = *min_element(all(T));
	sort(all(T), radial_lt);
	for (k = n-2; k >= 0 && ccw(T[0], T[n-1], T[k]) == 0; k--);
	reverse((k+1) + all(T));

	for (int i = 0; i < n; i++) {
		// troque o >= por > para manter pontos colineares
		while (j > 1 && ccw(U[j-1], U[j-2], T[i]) >= 0) j--;
		U[j++] = T[i];
	}
	U.erase(j + all(U));
	return U;
}

////////////////////////////////////////////////////////////////////////////////
// Calcula a área orientada do polígono T.
//

double poly_area(polygon& T) { // start: cf6031c39d9b21ae744c7324a96c5903
	double s = 0; int n = T.size();
	for (int i = 0; i < n; i++)
		s += T[i] % T[(i+1) % n];
	return s / 2;
}

////////////////////////////////////////////////////////////////////////////////
// Encontra o ponto de interseção das retas pq e rs.
// (Nota: causa divisão por zero se forem paralelas!)

// start: 93b5d67b7a884a1b2c740dc7433a7e68
point line_intersect(point p, point q, point r, point s) {
	point a = q - p, b = s - r, c = point(p % q, r % s);
	return point(point(a.x, b.x) % c, point(a.y, b.y) % c) / (a % b);
}





////////////////////////////////////////////////////////////////////////////////
// Encontra o menor círculo que contém todos os pontos dados.
//

typedef pair<point, double> circle;

// start: 7860868d50092aa0538fdf7b36b8f715
bool in_circle(circle C, point p){
	return cmp(abs(p - C.first), C.second) <= 0;
}

// start: 3028444d6729168187b966e9c984bab3
point circumcenter(point p, point q, point r) {
	point a = p - r, b = q - r, c = point(a * (p + r) / 2, b * (q + r) / 2);
	return point(c % point(a.y, b.y), point(a.x, b.x) % c) / (a % b);
}

// start: 37087a20364e8a888b128348f9524d90
circle spanning_circle(vector<point>& T) {
	int n = T.size();
	random_shuffle(all(T));
	circle C(point(), -INFINITY);
	for (int i = 0; i < n; i++) if (!in_circle(C, T[i])) {
		C = circle(T[i], 0);
		for (int j = 0; j < i; j++) if (!in_circle(C, T[j])) {
			C = circle((T[i] + T[j]) / 2, abs(T[i] - T[j]) / 2);
			for (int k = 0; k < j; k++) if (!in_circle(C, T[k])) {
				point o = circumcenter(T[i], T[j], T[k]);
				C = circle(o, abs(o - T[k]));
			}
		}
	}
	return C;
}

////////////////////////////////////////////////////////////////////////////////
// Determina o polígono interseção dos dois polígonos convexos P e Q.
//
// Tanto P quanto Q devem estar orientados positivamente.
//

// start: 2b72988c43ca7f4f8541c099821f2343
polygon poly_intersect(polygon& P, polygon& Q) {
	int m = Q.size(), n = P.size();
	int a = 0, b = 0, aa = 0, ba = 0, inflag = 0;
	polygon R;
	while ((aa < n || ba < m) && aa < 2*n && ba < 2*m) {
		point p1 = P[a], p2 = P[(a+1) % n], q1 = Q[b], q2 = Q[(b+1) % m];
		point A = p2 - p1, B = q2 - q1;
		int cross = cmp(A % B), ha = ccw(p2, q2, p1), hb = ccw(q2, p2, q1);
		if (cross == 0 && ccw(p1, q1, p2) == 0 && cmp(A * B) < 0) {
			if (between(p1, q1, p2)) R.push_back(q1);
			if (between(p1, q2, p2)) R.push_back(q2);
			if (between(q1, p1, q2)) R.push_back(p1);
			if (between(q1, p2, q2)) R.push_back(p2);
			if (R.size() < 2) return polygon();
			inflag = 1; break;
		} else if (cross != 0 && seg_intersect(p1, p2, q1, q2)) {
			if (inflag == 0) aa = ba = 0;
			R.push_back(line_intersect(p1, p2, q1, q2));
			inflag = (hb > 0) ? 1 : -1;
		}
		if (cross == 0 && hb < 0 && ha < 0) return R;
		bool t = cross == 0 && hb == 0 && ha == 0;
		if (t ? (inflag == 1) : (cross >= 0) ? (ha <= 0) : (hb > 0)) {
			if (inflag == -1) R.push_back(q2);
			ba++; b++; b %= m;
		} else {
			if (inflag == 1) R.push_back(p2);
			aa++; a++; a %= n;
		}
	}
	if (inflag == 0) {
		if (in_poly(P[0], Q)) return P;
		if (in_poly(Q[0], P)) return Q;
	}
	R.erase(unique(all(R)), R.end());
	if (R.size() > 1 && R.front() == R.back()) R.pop_back();
	return R;
}

////////////////////////////////////////////////////////////////////////////////
// Campos /////////////////////////////////////////////////////////// 15 min. //
////////////////////////////////////////////////////////////////////////////////

#include <list>
#include <set>

const int TAM = 2000;

typedef pair<point, point> segment;
typedef pair<int, int> barrier;

struct field { // start: 741665c3e39940d3c1242972af8c6166
	int n, m;
	point v[TAM];
	barrier b[TAM];
	list<int> e[TAM];

	field(): n(0), m(0) {}

	void clear() {
		for (int i = 0; i < n; i++) e[i].clear();
		n = m = 0;
	}

	inline int ccw(int a, int b, int c) { return ::ccw(v[a], v[b], v[c]); }

	void make_barrier(int i, int j) { // start: 58ad7be331c400ba31b059756e3e2420
		e[i].push_back(m); e[j].push_back(m);
		b[m++] = barrier(i, j);
	}

	//////////////////////////////////////////////////////////////////////////////
	// Remove os casos degenerados de um campo.
	//

	void normalize() { // start: 3778d835941f89004081fedc5fdadf3b
		set<segment> T; set<point> U;
		for (int i = 0; i < n; i++) make_barrier(i, i);
		for (int i = 0; i < m; i++) {
			point p = v[b[i].first], q = v[b[i].second];
			set<point> S;
			S.insert(p); S.insert(q);
			for (int j = 0; j < m; j++) {
				point r = v[b[j].first], s = v[b[j].second];
				if (r == p || r == q || s == p || s == q) continue;
				if (cmp((q - p) % (s - r)) == 0) {
					if (between(p, r, q)) S.insert(r);
					if (between(p, s, q)) S.insert(s);
				} else if (seg_intersect(p, q, r, s)) {
					S.insert(line_intersect(p, q, r, s));
				}
			}
			foreach (st, all(S)) {
				if (st != S.begin()) T.insert(segment(p, *st));
				U.insert(p = *st);
			}
		}
		clear();
		foreach (it, all(U)) v[n++] = *it;
		foreach (it, all(T)) {
			int i = lower_bound(v, v+n, it->first) - v;
			int j = lower_bound(v, v+n, it->second) - v;
			make_barrier(i, j);
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// Algoritmo de Poggi-Moreira-Fleischman-Cavalcante.
	//
	// Determina um grafo que contém todas as arestas de um eventual menor
	// caminho entre pontos do grafo.
	//

	void pmfc(graph& G) { // start: 6bd274792b086b791644cf5067cb8ba2
		int sel[TAM][2], active[TAM];
		for (int i = 0; i < n; i++) {
			vector< pair<double, int> > T;
			foreach (it, all(e[i])) {
				int j = b[*it].first + b[*it].second - i;
				T.push_back(make_pair(arg(v[j] - v[i]), j));
			}
			sort(all(T));
			if (T.empty()) T.push_back(make_pair(0, i));
			active[i] = 0;
			int p = T.size();
			for (int j = 0; j < p; j++) {
				sel[i][0] = T[j].second; sel[i][1] = T[(j+1) % p].second;
				if (ccw(sel[i][0], sel[i][1], i) <= 0) {
					active[i] = 1; break;
				}
			}
		}
		G.init(n);
		for (int i = 0; i < n; i++) for (int j = 0; j < i; j++) {
			if (!active[i] || !active[j]) continue;
			if (ccw(i, j, sel[i][0]) * ccw(i, j, sel[i][1]) == -1 || \
			    ccw(i, j, sel[j][0]) * ccw(i, j, sel[j][1]) == -1)
				continue;
			for (int k = 0; k < m; k++) {
				int org = b[k].first, dest = b[k].second;
				if (org == i || org == j || dest == i || dest == j) continue;
				if (seg_intersect(v[i], v[j], v[org], v[dest])) goto PROX;
			}
			G.aresta(i, j, 1, abs(v[j] - v[i]));
PROX: ;
		}
	}
};

////////////////////////////////////////////////////////////////////////////////
// Triangulações //////////////////////////////////////////////////// 15 min. //
////////////////////////////////////////////////////////////////////////////////

// Depende da struct field.

#include <map>

typedef pair<int, int> edge;

// start: 0e4c068ffa53d0b3e14b724630ea71d5
struct triangulation: public map<edge, int> {
	edge sym(edge e)   { return edge(e.second, e.first); }
	edge lnext(edge e) { return edge(e.second, (*this)[e]); }
	edge lprev(edge e) { return edge((*this)[e], e.first); }
	edge dnext(edge e) { return lprev(sym(lprev(e))); }
	edge dprev(edge e) { return lnext(sym(lnext(e))); }

	void new_tri(edge e, int r) { // start: bd7b82cfb3d9b84a061d514a2b70fe30
		if (count(e)) { erase(lnext(e)); erase(lprev(e)); }
		(*this)[e] = r; (*this)[lnext(e)] = e.first; (*this)[lprev(e)] = e.second;
	}

	// Digite esta função apenas para triangulações com restrições.
	void bdsm(field& F, edge e) { // start: 0ffc50c144104b1ccf3363a3f1c4a780
		int a, b, c, d, org = e.first, dest = e.second, topo = 0;
		edge xt;
		edge pilha[TAM];
		if (count(e)) return;
		for (iterator it = lower_bound(edge(org, 0)); ; it++) {
			xt = lnext(it->first); a = xt.first, b = xt.second;
			if (b < 0) continue;
			if (seg_intersect(F.v[a], F.v[b], F.v[org], F.v[dest])) break;
		}
		while (xt.second != dest) {
			pilha[topo++] = xt; xt = sym(xt);
			xt = F.ccw(org, dest, (*this)[xt]) >= 0 ? lnext(xt) : lprev(xt);
			while (topo > 0) {
				edge ee = pilha[topo-1];
				a = ee.first; b = ee.second;
				c = (*this)[ee]; d = (*this)[sym(ee)];
				if (F.ccw(d, c, b) >= 0 || F.ccw(c, d, a) >= 0) break;
				erase(ee); erase(sym(ee)); xt = edge(d, c);
				new_tri(xt, a); new_tri(sym(xt), b);
				topo--;
				xt = F.ccw(org, dest, d) >= 0 ? lprev(xt) : lnext(sym(xt));
			}
		}
	}
	void triangulate(field& F) { // start: 262fff10182c34db956f8784bc61d686
		int J[TAM], i, k, topo = 0;
		edge pilha[TAM];
		clear();
		for (int i = 0; i < F.n; i++) J[i] = i;
		sort(J, J + F.n, make_index_lt(F.v));
		for (i = 2; i < F.n; i++) if (k = F.ccw(J[0], J[1], J[i])) break;
		if (i >= F.n) return;
		for (int j = 1; j < i; j++) {
			edge e(J[j-1], J[j]);
			new_tri(e, (k > 0) ? J[i] : -1);
			new_tri(sym(e), (k > 0) ? -1 : J[i]);
		}
		edge lb(J[i], J[(k > 0) ? i-1 : 0]), ub(J[(k > 0) ? 0 : i-1], J[i]);
		for (i++; i < F.n; i++) {
			while (F.ccw(lb.first, lb.second, J[i]) >= 0) lb = dprev(lb);
			while (F.ccw(ub.first, ub.second, J[i]) >= 0) ub = dnext(ub);
			for (edge e = dnext(lb); e != ub; e = dnext(e)) pilha[topo++] = e;
			while (topo > 0) new_tri(pilha[--topo], J[i]);
			edge e(-1, J[i]);
			new_tri(e, lb.first); new_tri(sym(e), ub.second);
			lb = lnext(e); ub = dnext(lb);
		}
		// Digite esta linha somente para triangulações com restrições.
		for (i = 0; i < F.m; i++) {
			bdsm(F, edge(F.b[i].first, F.b[i].second));
		}
	}
};
