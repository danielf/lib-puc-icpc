#include <queue> // Apenas para Fluxos

const int VT = 1010;
const int AR = VT * VT;

struct grafo {

	//////////////////////////////////////////////////////////////////////////////
	// Definições compartilhadas.
	//

	int dest[2 * AR]; // "2 *" apenas para CFC.
	int adj[VT][2 * VT]; // "2 *" apenas para Fluxos e CFC.
	int nadj[VT], nvt, nar;

	inline int inv(int a) { return a ^ 0x1; } // Apenas para Fluxos e PP.

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Fluxos.
	//

	int cap[AR], fluxo[AR], ent[VT];

	inline int orig(int a) { return dest[inv(a)]; }
	inline int capres(int a) { return cap[a] - fluxo[a]; }

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Fluxo Máximo.
	//

	int padj[VT], lim[VT], nivel[VT], qtd[VT];

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Fluxo a Custo Mínimo.
	//

	int imb[VT], marc[VT], delta;
	double custo[AR], pot[VT], dist[VT];

	inline double custores(int a) {
		return custo[a] - pot[orig(a)] + pot[dest[a]];
	}

	//////////////////////////////////////////////////////////////////////////////
	// Definição específica para Conexidade.
	//

	int prof[VT];

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Pontos de Articulação e Pontes.
	//

	char part[VT], ponte[AR];
	int menor[VT], npart, nponte;

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para Componentes Fortemente Conexas.
	//

	int ord[VT], comp[VT], repcomp[VT], nord, ncomp;

	inline int transp(int a) { return (a & 0x1); }

	//////////////////////////////////////////////////////////////////////////////
	// Definições específicas para 2 SAT.
	//

	inline int verd(int v) { return 2 * v + 1; }
	inline int falso(int v) { return 2 * v; }

	//////////////////////////////////////////////////////////////////////////////
	// Funções compartilhadas.
	//

	//////////////////////////////////////////////////////////////////////////////
	// Inicializa o grafo.
	//

	void inic(int n = 0) { // start: 48c71c16bf67ffb97a444b716e6677b5
		nvt = n;
		nar = 0;
		memset(nadj, 0, sizeof(nadj));
		memset(imb, 0, sizeof(imb)); // Apenas para FCM
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// Adiciona uma aresta ao grafo.
	//
	// "int u" apenas para Fluxos; "double c" apenas para FCM.
	//

	// start: 67d1557ea04aeec3a29cbb8daf5b1336
	int aresta(int i, int j, int u = 0, double c = 0) {
		int ar = nar;
	
		custo[nar] = c; // Apenas para FCM.
		cap[nar] = u; // Apenas para Fluxos.
		dest[nar] = j;
		adj[i][nadj[i]++] = nar++;

		custo[nar] = -c; // Apenas para FCM.
		cap[nar] = 0; // Apenas para Fluxos.
		dest[nar] = i;
		adj[j][nadj[j]++] = nar++;

		return ar;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Funções específicas para Fluxo Máximo.
	//

	// start: c374dc80b1c1cb7d9e14faf1839c7dc0
	void revbfs(int ini, int fim) {
		int i, no, viz, ar;

		queue<int> fila;
		memset(nivel, NULO, sizeof(nivel));
		memset(qtd, 0, sizeof(qtd));

		nivel[fim] = 0; fila.push(fim);

		while (!fila.empty()) {
			no = fila.front(); fila.pop();
			qtd[nivel[no]]++;

			for (i = 0; i < nadj[no]; i++) {
				ar = adj[no][i]; viz = dest[ar];
				if (cap[ar] == 0 && nivel[viz] == NULO) {
					nivel[viz] = nivel[no] + 1; fila.push(viz);
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

	int retrocede(int no) {
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
	// end: c374dc80b1c1cb7d9e14faf1839c7dc0

	//////////////////////////////////////////////////////////////////////////////
	// Função específica para Fluxo a Custo Mínimo.
	//
	// Algoritmo de Dijkstra: O(m * log n)
	//

	void dijkstra(int ini) { // start: f3d1a16bb01ee4f46e44f03310f18b59
		int i, j, k, a;
		double d;

		priority_queue<pair<double, int> > heap;
		memset(ent, NULO, sizeof(ent));
		memset(marc, 0, sizeof(marc));

		for (i = 0; i < nvt; i++) dist[i] = INFINITY;
		heap.push(make_pair(dist[ini] = 0.0, ini));

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

	//////////////////////////////////////////////////////////////////////////////
	// Função específica para Pontos de Articulação e Pontes.
	//

	// start: 5e9f0f653ac8f4cf4ab7edbc6738bda6
	int dfs_partponte(int no, int ent) {
		int i, ar, viz, nf = 0;

		for (i = 0; i < nadj[no]; i++) {
			ar = adj[no][i]; viz = dest[ar];

			if (prof[viz] == NULO) {
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

	// start: f9e30d5f0035d75466e0b1b7496e91f9
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
				prof[i] = 0; dfs_topsort(i);
			}
	}

	void dfs_compfortcon(int no) {
		comp[no] = ncomp;
		for (int i = 0; i < nadj[no]; i++) {
			int ar = adj[no][i], viz = dest[ar];
			if (transp(ar) && comp[viz] == NULO) dfs_compfortcon(viz);
		}
	}
	// end: f9e30d5f0035d75466e0b1b7496e91f9

	//////////////////////////////////////////////////////////////////////////////
	// Função específica para 2 SAT.
	//
	// Adiciona ao grafo as arestas correspondentes a clausula
	// ((x = valx) ou (y = valy))
	//

	// start: 2228d040e8f84fc8ca0e7928a6440ba0
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

	int maxflow(int ini, int fim) { // start: 7cdb2ec5d82189460e7ee6d9652683d3
		int ar, no = ini, fmax = 0;

		memset(fluxo, 0, sizeof(fluxo));
		memset(padj, 0, sizeof(padj));

		revbfs(ini, fim);
		lim[ini] = INF; ent[ini] = NULO;

		while (nivel[ini] < nvt && no != NULO) {
			if ((ar = admissivel(no)) == NULO) no = retrocede(no);
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
	// (Nota: Lembrar de testar viabilidade da rede com o nó artificial gerado)

	double mincostflow() { // start: e9ff00a38b57cf4d0c8594bc0bd12eb6
		int a, i, j, k, l, U = 0;
		double C = 0.;

		memset(pot, 0, sizeof(pot));
		memset(fluxo, 0, sizeof(fluxo));

		for (a = 0; a < nar ; a++) {
			if (cmp(custo[a]) > 0) C += custo[a];
			U = max(cap[a], U);
		}
		for (i = 0; i < nvt; i++) U = max(imb[i], max(-imb[i], U));
		for (delta = 0x40000000; delta > U; delta /= 2);

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
				for (a = ent[l]; a != NULO; a = ent[orig(a)])  {
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

	void partponte() { // start: 3446aa62c39e51799769355c6d7c48c3
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
		nponte /= 2;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Encontra as Componentes Fortemente Conexas.
	//

	int compfortcon() { // start: 1fc82eb8cbe082e662db25ab7d74d272
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
	//////////////////////////////////////////////////////////////////////////////
	// Decide se a conjunção das cláusulas pode ser satisfeita.
	// (Nota 1: requer código de componente fortemente conexa)
	// (Nota 2: lembrar de inicializar o grafo com 2*num_variaveis (x_i e ~x_i))

	int twosat(int nvar) { // start: 2ac831ced98087e2e9a19b2c986b3261
		compfortcon();
		for (int i = 0; i < nvar; i++)
			if (comp[verd(i)] == comp[falso(i)])
				return 0;
		return 1;
	}
};
