#include <queue> // Apenas para Fluxos

const int VT = 1010;
const int AR = VT * VT;

struct grafo {

	//////////////////////////////////////////////////////////////////////////////
	// Definições compartilhadas.
	//

	vector<int> dest;
	vector<int> adj[VT];
	int nvt, nar;

	int inv(int a) { return a ^ 0x1; } // Apenas para Fluxos e PP.

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Fluxos.
	//

	vector<int> cap, fluxo;
	int ent[VT];

	int orig(int a) { return dest[inv(a)]; }
	int capres(int a) { return cap[a] - fluxo[a]; }

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Fluxo Máximo.
	//

	int padj[VT], lim[VT], nivel[VT], qtd[VT];

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Fluxo a Custo Mínimo.
	//

	int imb[VT], marc[VT], delta;
	double pot[VT], dist[VT];
	vector<double> custo;

	double custores(int a) {
		return custo[a] - pot[orig(a)] + pot[dest[a]];
	}

	//////////////////////////////////////////////////////////////////////////////
	// Definição específica para Conexidade.
	//

	int prof[VT];

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Pontos de Articulação e Pontes.
	//

	char part[VT];
	char ponte[AR];
	int menor[VT], npart, nponte;

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Componentes Fortemente Conexas.
	//

	int ord[VT], comp[VT], repcomp[VT], nord, ncomp;

	int transp(int a) { return (a & 0x1); }

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para 2 SAT.
	//

	int verd(int v) { return 2 * v + 1; }
	int falso(int v) { return 2 * v; }

	//////////////////////////////////////////////////////////////////////////////
	// Funções compartilhadas.
	//

	//////////////////////////////////////////////////////////////////////////////
	// Inicializa o grafo.
	//

	void inic(int n = 0) {
		nvt = n;
		nar = 0;
		fu(i, VT) adj[i].clear();
		memset(imb, 0, sizeof(imb)); // Apenas para FCM
		dest.clear(); cap.clear(); fluxo.clear(); custo.clear();
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Adiciona uma aresta ao grafo.
	//
	// "int u" apenas para Fluxos; "double c" apenas para FCM.
	//

	int aresta(int i, int j, int u = 0, double c = 0) {
		int ar = nar;
	
		custo.pb(c);
		cap.pb(u);
		dest.pb(j);
		adj[i].pb(nar++);

		custo.pb(-c);
		cap.pb(0);
		dest.pb(i);
		adj[j].pb(nar++);
		return ar;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Funções específicas para Fluxo Máximo.
	//

	void revbfs(int ini, int fim) {
		int i, no, viz, ar;

		queue<int> fila;
		memset(nivel, -1, sizeof(nivel));
		memset(qtd, 0, sizeof(qtd));

		nivel[fim] = 0; fila.push(fim);

		while (!fila.empty()) {
			no = fila.front(); fila.pop();
			qtd[nivel[no]]++;

			for (i = 0; i < adj[no].size(); i++) {
				ar = adj[no][i]; viz = dest[ar];
				if (cap[ar] == 0 && nivel[viz] == -1) {
					nivel[viz] = nivel[no] + 1; fila.push(viz);
				}
			}
		}
	}

	int admissivel(int no) {
		while (padj[no] < adj[no].size()) {
			int ar = adj[no][padj[no]];
			if (nivel[no] == nivel[dest[ar]] + 1 && capres(ar) > 0) return ar;
			padj[no]++;
		}
		padj[no] = 0;
		return -1;
	}

	int retrocede(int no) {
		int i, ar, viz, menor = -1;
		if (--qtd[nivel[no]] == 0) return -1;

		for (i = 0; i < adj[no].size(); i++) {
			ar = adj[no][i]; viz = dest[ar];
			if (capres(ar) <= 0) continue;
			if (menor == -1 || nivel[viz] < nivel[menor]) menor = viz;
		}

		if (menor != -1) nivel[no] = nivel[menor];
		qtd[++nivel[no]]++;
		return ((ent[no] == -1) ? no : orig(ent[no]));
	}

	int avanca(int no, int ar) {
		int viz = dest[ar];
		ent[viz] = ar;
		lim[viz] = min(lim[no], capres(ar));
		return viz;
	}

	int aumenta(int ini, int fim) {
		int ar, no = fim, fmax = lim[fim];
		while (no != ini) {
			fluxo[ar = ent[no]] += fmax;
			fluxo[inv(ar)] -= fmax;
			no = orig(ar);
		}
		return fmax;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Função específica para Fluxo a Custo Mínimo.
	//
	// Algoritmo de Dijkstra: O(m * log n)
	//

	void dijkstra(int ini) {
		int i, j, k, a;
		double d;

		priority_queue<pair<double, int> > heap;
		memset(ent, -1, sizeof(ent));
		memset(marc, 0, sizeof(marc));

		for (i = 0; i < nvt; i++) dist[i] = INFINITY;
		heap.push(make_pair(dist[ini] = 0.0, ini));

		while (!heap.empty()) {
			i = heap.top().second; heap.pop();
			if (marc[i]) continue; marc[i] = 1;
			for (k = 0; k < adj[i].size(); k++) {
				a = adj[i][k]; j = dest[a]; d = dist[i] + custores(a);
				if (capres(a) >= delta && cmp(d, dist[j]) < 0) {
					heap.push(make_pair( -(dist[j] = d), j));
					ent[j] = a;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// Função específica para Pontos de Articulação e Pontes.
	//

	int dfs_partponte(int no, int ent) {
		int i, ar, viz, nf = 0;

		for (i = 0; i < adj[no].size(); i++) {
			ar = adj[no][i]; viz = dest[ar];

			if (prof[viz] == -1) {
				menor[viz] = prof[viz] = prof[no] + 1;
				dfs_partponte (viz, ar); nf++;

				if (menor[viz] >= prof[no]) {
					part[no] = 1;
					if (menor[viz] == prof[viz]) ponte[ar] = ponte[inv(ar)] = 1;
				}
				else menor[no] = min(menor[no], menor[viz]);
			}
			else if (inv(ar) != ent) menor[no] = min(menor[no], prof[viz]);
		}

		return nf;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Funções específicas para Componentes Fortemente Conexas.
	//
	// Ordenação Topológica (duas primeiras funções).
	//

	void dfs_topsort(int no) {
		for (int i = 0; i < adj[no].size(); i++) {
			int ar = adj[no][i], viz = dest[ar];
			if (!transp(ar) && prof[viz] == -1) {
				prof[viz] = prof[no] + 1; dfs_topsort(viz);
			}
		}
		ord[--nord] = no;
	}

	void topsort() {
		memset(prof, -1, sizeof(prof));
		nord = nvt;

		for (int i = 0; i < nvt; i++)
			if (prof[i] == -1) {
				prof[i] = 0; dfs_topsort(i);
			}
	}

	void dfs_compfortcon(int no) {
		comp[no] = ncomp;
		for (int i = 0; i < adj[no].size(); i++) {
			int ar = adj[no][i], viz = dest[ar];
			if (transp(ar) && comp[viz] == -1) dfs_compfortcon(viz);
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// Função específica para 2 SAT.
	//
	// Adiciona ao grafo as arestas correspondentes a clausula
	// ((x = valx) ou (y = valy))
	//

	void clausula(int x, bool valx, int y, bool valy) {
		int hipA, teseA, hipB, teseB;

		if (valx) { hipA = falso(x); teseB = verd(x); }
		else { hipA = verd(x); teseB = falso(x); }

		if (valy) { hipB = falso(y); teseA = verd(y); }
		else { hipB = verd(y); teseA = falso(y); }

		aresta(hipA, teseA);
		aresta(hipB, teseB);
	}

	//////////////////////////////////////////////////////////////////////////////
	// Fluxo Máximo: O(n^2 * m)
	//

	int maxflow(int ini, int fim) {
		int ar, no = ini, fmax = 0;

		for (int i = 0; i < nar; i++)
			fluxo.push_back(0);
		memset(padj, 0, sizeof(padj));

		revbfs(ini, fim);
		lim[ini] = INF; ent[ini] = -1;

		while (nivel[ini] < nvt && no != -1) {
			if ((ar = admissivel(no)) == -1) no = retrocede(no);
			else if ((no = avanca(no, ar)) == fim) {
				fmax += aumenta(ini, fim);
				no = ini;
			}
		}
		return fmax;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Fluxo a Custo Mínimo: O(m^2 * log n * log U)
	//
	// Parametro global específico: imb
	//

	double mincostflow() {
		int a, i, j, k, l, U = 0;
		double C = 0.;

		memset(pot, 0, sizeof(pot));

		for (a = 0; a < nar ; a++) {
			if (cmp(custo[a]) > 0) C += custo[a];
			U = max(cap[a], U);
		}
		for (i = 0; i < nvt; i++) U = max(imb[i], max(-imb[i], U));
		for (delta = 0x40000000; delta > U; delta /= 2);

		imb[nvt] = 0 ; U *= 2 * nvt; C *= 2; adj[nvt].clear();
		for (i = 0; i < nvt; i++) {
			aresta(i, nvt, U, C);
			aresta(nvt, i, U, C);
		}

		fluxo.clear();
		fu(i, nar) fluxo.pb(0);
		nvt++;

		while (delta >= 1) {
			for (a = 0; a < nar ; a++) {
				i = orig(a); j = dest[a];
				if (delta <= capres(a) && capres(a) < 2 * delta &&
				    cmp(custores(a)) < 0) {
					fluxo[inv(a)] -= capres(a);
					imb[i] -= capres(a); imb[j] += capres(a);
					fluxo[a] = cap[a];
				}
			}

			while (true) {
				for (k = 0 ; k < nvt && imb[k] < delta; k++);
				for (l = nvt - 1 ; l >= 0 && imb[l] > -delta; l--);
				if (k == nvt || l < 0) break;

				dijkstra(k);
				for (i = 0 ; i < nvt ; i++) pot[i] -= dist[i];
				for (a = ent[l]; a != -1; a = ent[orig(a)])  {
					fluxo[a] += delta; fluxo[inv(a)] -= delta;
				}
				imb[k] -= delta; imb[l] += delta;
			}
			delta /= 2;
		}

		C = 0.;
		for (a = 0; a < nar; a++) if (fluxo[a] > 0) C += fluxo[a] * custo[a];
		return C;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Encontra os Pontos de Articulação e as Pontes.
	//

	void partponte() {
		memset(part, 0, sizeof(part));
		memset(ponte, 0, sizeof(ponte));
		memset(prof, -1, sizeof(prof));
		memset(menor, -1, sizeof(menor));
		npart = nponte = 0;

		for (int i = 0; i < nvt; i++)
			if (prof[i] == -1) {
				menor[i] = prof[i] = 0;
				if (dfs_partponte(i, -1) < 2) part[i] = 0;
			}
		for (int i = 0; i < nvt; i++) if (part[i]) npart++;
		for (int i = 0; i < nar; i++) if (ponte[i]) nponte++;
		nponte /= 2;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Encontra as Componentes Fortemente Conexas.
	//

	int compfortcon() {
		memset(comp, -1, sizeof(comp));
		ncomp = 0;
		topsort();

		for (int i = 0; i < nvt; i++)
			if (comp[ord[i]] == -1) {
				repcomp[ncomp] = ord[i];
				dfs_compfortcon(ord[i]);
				ncomp++;
			}

		return ncomp;
	}
	//////////////////////////////////////////////////////////////////////////////
	// Decide se a conjunção das cláusulas pode ser satisfeita.
	//

	bool twosat(int nvar) {
		compfortcon();
		for (int i = 0; i < nvar; i++)
			if (comp[verd(i)] == comp[falso(i)]) return false;
		return true;
	}
};
