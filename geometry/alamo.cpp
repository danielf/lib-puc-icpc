#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>

#include <algorithm>
#include <utility>
#include <iostream>

using namespace std;

#define TRACE(x...)
#define PRINT(x...) TRACE(printf(x))
#define WATCH(x) TRACE(cout << #x" = " << x << "\n")

#define _inline(f...) f() __attribute__((always_inline)); f
#define _foreach(it, b, e) for (typeof(b) it = (b); it != (e); it++)
#define foreach(x...) _foreach(x)
#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()

const double EPS = 1e-10;

_inline(int cmp)(double x, double y = 0, double tol = EPS) {
	return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
#include <vector>

struct point {
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
};

point point::pivot;

double abs(point p) { return hypot(p.x, p.y); }
double arg(point p) { return atan2(p.y, p.x); }

typedef vector<point> polygon;

ostream& operator <<(ostream& o, polygon T) {
	o << "polygon[";
	for (unsigned i = 0; i < T.size(); i++) {
		if (i) o << ", ";
		o << T[i];
	}
	return o << "]";
}

_inline(int ccw)(point p, point q, point r) {
	return cmp((p - r) % (q - r));
}

_inline(double angle)(point p, point q, point r) {
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

int in_poly(point p, polygon& T) {
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

bool radial_lt(point p, point q) {
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

double poly_area(polygon& T) {
	double s = 0; int n = T.size();
	for (int i = 0; i < n; i++)
		s += T[i] % T[(i+1) % n];
	return s / 2;
}

////////////////////////////////////////////////////////////////////////////////
// Encontra o ponto de interseção das retas pq e rs.
//

point line_intersect(point p, point q, point r, point s) {
	point a = q - p, b = s - r, c = point(p % q, r % s);
	return point(point(a.x, b.x) % c, point(a.y, b.y) % c) / (a % b);
}

////////////////////////////////////////////////////////////////////////////////
// Encontra o menor círculo que contém todos os pontos dados.
//

typedef pair<point, double> circle;

bool in_circle(circle C, point p){
	return cmp(abs(p - C.first), C.second) <= 0;
}

point circumcenter(point p, point q, point r) {
	point a = p - r, b = q - r, c = point(a * (p + r) / 2, b * (q + r) / 2);
	return point(c % point(a.y, b.y), point(a.x, b.x) % c) / (a % b);
}

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
#include <queue>                        // apenas para fluxos

const int VT = 1010;
const int AR = VT * VT;
const int NULO = -1;
const int INF = 1000000000;             // apenas para fluxo máximo

struct graph {

	////////////////////////////////////////////////////////////////////////////
	// Definições compartilhadas
	//

	int dest[2 * AR];                   // "2 *" apenas para CFC
	int adj[VT][2 * VT];                // "2 *" apenas para fluxos e CFC
	int nadj[VT], nvt, nar;

	////////////////////////////////////////////////////////////////////////////
	// Definições específicas para fluxos
	//

	int cap[AR], fluxo[AR], ent[VT];

	_inline(int orig)(int a) {return dest[inv(a)]; }
	_inline(int inv)(int a) { return a ^ 0x1; }
	_inline(int capres)(int a) { return cap[a] - fluxo[a]; }

	////////////////////////////////////////////////////////////////////////////
	// Definições específicas para fluxo máximo
	//

	int padj[VT], lim[VT], nivel[VT], qtd[VT];

	queue<int> fila;

	////////////////////////////////////////////////////////////////////////////
	// Definições específicas para fluxo a custo mínimo
	//

	int imb[VT], marc[VT], delta;
	double custo[AR], pot[VT], dist[VT];

	priority_queue< pair<double, int> > heap;

	_inline(double custores)(int a) {
		return custo[a] - pot[orig(a)] + pot[dest[a]];
	}

	////////////////////////////////////////////////////////////////////////////
	// Definição específica para conexidade
	//

	int prof[VT];

	////////////////////////////////////////////////////////////////////////////
	// Definições específicas para pontos de articulação e pontes
	//

	char part[VT], ponte[AR];
	int menor[VT], npart, nponte;

	////////////////////////////////////////////////////////////////////////////
	// Definições específicas para componentes fortemente conexas
	//

	int ord[VT], comp[VT], repcomp[VT], nord, ncomp;

	_inline(int transp)(int a) { return (a & 0x1); }

	////////////////////////////////////////////////////////////////////////////
	// Definições específicas para 2 SAT
	//

	_inline(int verd)(int v) { return 2 * v + 1; }
	_inline(int falso)(int v) { return 2 * v; }

	////////////////////////////////////////////////////////////////////////////
	// Funções compartilhadas
	//

	////////////////////////////////////////////////////////////////////////////
	// Inicializa o grafo
	//

	void init(int n = 0) {
		nvt = n;
		nar = 0;
		memset(nadj, 0, sizeof(nadj));
		memset(imb, 0, sizeof(imb));    // apenas para FCM
	}

	////////////////////////////////////////////////////////////////////////////
	// Adiciona uma aresta ao grafo
	//
	// "int u" apenas para Fluxos; "double c" apenas para FCM
	//

	void aresta(int i, int j, int u = 0, double c = 0) {
		custo[nar] = c;                 // Apenas para FCM
		cap[nar] = u;                   // Apenas para fluxos
		dest[nar] = j;
		adj[i][nadj[i]++] = nar++;

		custo[nar] = -c;                // Apenas para FCM
		cap[nar] = 0;                   // Apenas para fluxos
		dest[nar] = i;
		adj[j][nadj[j]++] = nar++;
	}

	////////////////////////////////////////////////////////////////////////////
	// Funções específicas para fluxo máximo
	//

	void revbfs(int ini, int fim) {
		int i, no, viz, ar;

		memset(nivel, NULO, sizeof(nivel));
		memset(qtd, 0, sizeof(qtd));
		while (!fila.empty()) fila.pop();

		nivel[fim] = 0;
		fila.push(fim);

		while (!fila.empty()) {
			no = fila.front(); fila.pop();
			qtd[nivel[no]]++;

			for (i = 0; i < nadj[no]; i++) {
				ar = adj[no][i]; viz = dest[ar];
				if (cap[ar] == 0 && nivel[viz] == NULO) {
					nivel[viz] = nivel[no] + 1;
					fila.push(viz);
				}
			}
		}
	}

	int admissivel(int no) {
		while (padj[no] < nadj[no]) {
			int ar = adj[no][padj[no]];
			if (nivel[no] == nivel[dest[ar]] + 1 && capres(ar) > 0) return ar;
			padj[no]++;
		}
		padj[no] = 0;
		return NULO;
	}

	int retrocede (int no) {
		int i, ar, viz, menor = NULO;
		if (--qtd[nivel[no]] == 0) return NULO;

		for (i = 0; i < nadj[no]; i++) {
			ar = adj[no][i]; viz = dest[ar];
			if (capres(ar) <= 0) continue;
			if (menor == NULO || nivel[viz] < nivel[menor]) menor = viz;
		}

		if (menor != NULO) nivel[no] = nivel[menor];
		qtd[++nivel[no]]++;
		return ((ent[no] == NULO) ? no : orig(ent[no]));
	}

	int avanca (int no, int ar) {
		int viz = dest[ar];
		ent[viz] = ar;
		lim[viz] = min (lim[no], capres(ar));
		return viz;
	}

	int aumenta (int ini, int fim) {
		int ar, no = fim, fmax = lim[fim];
		while (no != ini)       {
			fluxo[ar = ent[no]] += fmax;
			fluxo[inv(ar)] -= fmax;
			no = orig(ar);
		}
		return fmax;
	}

	////////////////////////////////////////////////////////////////////////////
	// Função específica para fluxo a custo mínimo
	//
	// Algoritmo de Dijkstra: O(m * log n)
	//

	void dijkstra(int ini) {
		int i, j, k, a;
		double d;

		memset(ent, NULO, sizeof(ent));
		memset(marc, 0, sizeof(marc));
		while (!heap.empty()) heap.pop();

		for (i = 0; i < nvt; i++) dist[i] = INFINITY;
		heap.push(make_pair( -(dist[ini] = 0.0), ini));

		while (!heap.empty()) {
			i = heap.top().second; heap.pop();
			if (marc[i]) continue; marc[i] = 1;
			for (k = 0; k < nadj[i]; k++) {
				a = adj[i][k]; j = dest[a]; d = dist[i] + custores(a);
				if (capres(a) >= delta && cmp(d, dist[j]) < 0) {
					heap.push(make_pair( -(dist[j] = d), j));
					ent[j] = a;
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// Função específica para pontos de articulação e pontes
	//

	int dfs_partponte(int no, int pai) {
		int i, ar, viz, nf = 0;

		for (i = 0; i < nadj[no]; i++) {
			ar = adj[no][i]; viz = dest[ar];

			if (prof[viz] == NULO) {
				menor[viz] = prof[viz] = prof[no] + 1;
				dfs_partponte(viz, no); nf++;

				if (menor[viz] >= prof[no]) {
					part[no] = 1;
					if (menor[viz] == prof[viz]) ponte[ar] = 1;
				}
				else menor[no] = min(menor[no], menor[viz]);
			}
			else if (viz != pai) menor[no] = min(menor[no], prof[viz]);
		}

		return nf;
	}

	////////////////////////////////////////////////////////////////////////////
	// Funções específicas para componentes fortemente conexas
	//
	// Ordenação Topológica (duas primeiras funções)
	//

	void dfs_topsort(int no) {
		for (int i = 0; i < nadj[no]; i++) {
			int ar = adj[no][i], viz = dest[ar];
			if (!transp(ar) && prof[viz] == NULO) {
				prof[viz] = prof[no] + 1; dfs_topsort(viz);
			}
		}
		ord[--nord] = no;
	}

	void topsort() {
		memset(prof, NULO, sizeof(prof));
		nord = nvt;

		for (int i = 0; i < nvt; i++) 
			if (prof[i] == NULO) {
				prof[i] = 0; dfs_topsort (i);
			}
	}

	void dfs_compfortcon(int no) {
		comp[no] = ncomp;
		for (int i = 0; i < nadj[no]; i++) {
			int ar = adj[no][i], viz = dest[ar];
			if (transp(ar) && comp[viz] == NULO) dfs_compfortcon(viz);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// Função específica para 2 SAT
	//
	// Adiciona ao grafo as arestas correspondentes à cláusula
	// ((x = valx) ou (y = valy))
	//

	void clausula(int x, bool valx, int y, bool valy) {
		int hipA, teseA, hipB, teseB;

		if (valx) {
			hipA = falso(x);
			teseB = verd(x);
		} else {
			hipA = verd(x);
			teseB = falso(x);
		}

		if (valy) {
			hipB = falso(y);
			teseA = verd(y);
		} else {
			hipB = verd(y);
			teseA = falso(y);
		}

		aresta(hipA, teseA);
		aresta(hipB, teseB);
	}

	////////////////////////////////////////////////////////////////////////////
	// Fluxo máximo: O(n^2 * m)
	//

	int maxflow(int ini, int fim) {
		int ar, no = ini, fmax = 0;

		memset(fluxo, 0, sizeof(fluxo));
		memset(padj, 0, sizeof(padj));

		revbfs(ini, fim);
		lim[ini] = INF;
		ent[ini] = NULO;

		while (nivel[ini] < nvt && no != NULO) {
			if ((ar = admissivel(no)) == NULO) no = retrocede(no);
			else if ((no = avanca(no, ar)) == fim) {
				fmax += aumenta(ini, fim);
				no = ini;
			}
		}
		return fmax;
	}

	////////////////////////////////////////////////////////////////////////////
	// Fluxo a custo mínimo: O(m^2 * log n * log U)
	//
	// Parâmetros globais específicos: imb
	//

	double mincostflow() {
		int a, i, j, k, l, U = 0;
		double C = 0.0;

		memset(pot, 0, sizeof(pot));
		memset(fluxo, 0, sizeof(fluxo));

		for (a = 0; a < nar; a++) {
			if (cmp(custo[a]) > 0) C += custo[a];
			U = max(cap[a], U);
		}
		for (i = 0; i < nvt; i++) U = max(imb[i], max(-imb[i], U));
		for (delta = 0x40000000; delta > U; delta >>= 1);

		imb[nvt] = nadj[nvt] = 0; U *= 2 * nvt; C *= 2;
		for (i = 0; i < nvt; i++) {
			aresta(i, nvt, U, C);
			aresta(nvt, i, U, C);
		}
		nvt++;

		while (delta >= 1) {
			for (a = 0; a < nar ; a++) {
				i = orig(a); j = dest[a];
				if (delta <= capres(a) && capres(a) < 2 * delta &&
						cmp(custores(a)) < 0) {
					fluxo[inv(a)] -= capres(a);
					imb[i] -= capres(a);
					imb[j] += capres(a);
					fluxo[a] = cap[a];
				}
			}

			while (1) {
				for (k = 0; k < nvt && imb[k] < delta; k++);
				for (l = nvt - 1; l >= 0 && imb[l] > -delta; l--);
				if (k == nvt || l < 0) break;

				dijkstra(k);
				for (i = 0; i < nvt; i++) pot[i] -= dist[i];
				for (a = ent[l]; a != NULO; a = ent[orig(a)])  {
					fluxo[a] += delta;
					fluxo[inv(a)] -= delta;
				}
				imb[k] -= delta;
				imb[l] += delta;
			}
			delta >>= 1;
		}

		C = 0.0;
		for (a = 0; a < nar; a++) if (fluxo[a] > 0) C += fluxo[a] * custo[a];
		return C;
	}

	////////////////////////////////////////////////////////////////////////////
	// Encontra os pontos de articulação e as pontes
	//

	void partponte() {
		memset(part, 0, sizeof(part));
		memset(ponte, 0, sizeof(ponte));
		memset(prof, NULO, sizeof(prof));
		memset(menor, NULO, sizeof(menor));
		npart = nponte = 0;

		for (int i = 0; i < nvt; i++)
			if (prof[i] == NULO) {
				menor[i] = prof[i] = 0;
				if (dfs_partponte(i, NULO) < 2) part[i] = 0;
			}
		for (int i = 0; i < nvt; i++) if (part[i]) npart++;
		for (int i = 0; i < nar; i++) if (ponte[i]) nponte++;
	}

	////////////////////////////////////////////////////////////////////////////
	// Encontra as componentes fortemente conexas
	//

	int compfortcon() {
		memset(comp, NULO, sizeof(comp));
		ncomp = 0;
		topsort();

		for (int i = 0; i < nvt; i++)
			if (comp[ord[i]] == NULO) {
				repcomp[ncomp] = ord[i];
				dfs_compfortcon(ord[i]);
				ncomp++;
			}

		return ncomp;
	}

	////////////////////////////////////////////////////////////////////////////
	// Decide se a conjunção das cláusulas pode ser satisfeita.
	//

	int twosat(int nvar) {
		compfortcon();
		for (int i = 0; i < nvar; i++)
			if (comp[verd(i)] == comp[falso(i)])
				return 0;
		return 1;
	}
};

#include <list>
#include <set>

const int TAM = 2000;

typedef pair<point, point> segment;
typedef pair<int, int> barrier;

struct field {
	int n, m;
	point v[TAM];
	barrier b[TAM];
	list<int> e[TAM];

	field(): n(0), m(0) {}

	void clear() {
		for (int i = 0; i < n; i++) e[i].clear();
		n = m = 0;
	}
	
	_inline(int ccw)(int a, int b, int c) { return ::ccw(v[a], v[b], v[c]); }

	void make_barrier(int i, int j) {
		e[i].push_back(m); e[j].push_back(m);
		b[m++] = barrier(i, j);
	}

	//////////////////////////////////////////////////////////////////////////////
	// Remove os casos degenerados de um campo.
	//

	void normalize() {
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

	void pmfc(graph& G) {
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
template <class T>
struct index_lt {
	T& v;
	index_lt(T& v): v(v) {}
	_inline(bool operator ())(int i, int j) {
		return (v[i] != v[j]) ? (v[i] < v[j]) : (i < j);
	}
};
template <class T> index_lt<T> make_index_lt(T& v) { return index_lt<T>(v); }

int safe_gets(char*& s) { // depois de usar, free(s);
	return scanf("%a[^\r\n]%*[\r\n]", &s);
}
#include <map>

typedef pair<int, int> edge;

struct triangulation: public map<edge, int> {
	edge sym(edge e)   { return edge(e.second, e.first); }
	edge lnext(edge e) { return edge(e.second, (*this)[e]); }
	edge lprev(edge e) { return edge((*this)[e], e.first); }
	edge dnext(edge e) { return lprev(sym(lprev(e))); }
	edge dprev(edge e) { return lnext(sym(lnext(e))); }

	void new_tri(edge e, int r) {
		if (count(e)) { erase(lnext(e)); erase(lprev(e)); }
		(*this)[e] = r; (*this)[lnext(e)] = e.first; (*this)[lprev(e)] = e.second;
	}

	// Digite esta função apenas para triangulações com restrições.
	void bdsm(field& F, edge e) {
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

	void triangulate(field& F) {
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
