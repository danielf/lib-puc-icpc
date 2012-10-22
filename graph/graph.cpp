const int VT = 410; // Max number of vertices

struct graph {

	//////////////////////////////////////////////////////////////////////////////
	// Shared part. Also known as: You will need this!
	//

	vector<int> dest;
	vector<int> adj[VT];
	int nvt, nar;

	int inv(int a) { return a ^ 0x1; }


	// Initializes the graph
	void init(int n) {
		nvt = n;
		nar = 0;
		fu(i, VT) adj[i].clear();
		memset(imb, 0, sizeof(imb)); // Only for min-cost-flow
		dest.clear(); cap.clear(); flow.clear(); cost.clear();
	}
	
	// Adds an arc to the graph. u is capacity, c is cost.
	// u is only needed on flows, and c only on min-cost-flow
	// Returns an identifier to the edge.
	int arc(int i, int j, int u = 0, double c = 0) {
		int ar = nar;
		cost.pb(c);
		cap.pb(u);
		dest.pb(j);
		adj[i].pb(nar++);

		cost.pb(-c);
		cap.pb(0);
		dest.pb(i);
		adj[j].pb(nar++);
		return ar;
	}

	//////////////////////////////////////////////////////////////////////////////
	// For both flows!!
	//

	vector<int> cap, flow;
	int ent[VT];

	int orig(int a) { return dest[inv(a)]; }
	int capres(int a) { return cap[a] - flow[a]; }

	//////////////////////////////////////////////////////////////////////////////
	// Max Flow!
	//

	int d[VT];
	int curAdj[VT];

	bool MFbfs(int s, int t) {
		memset(d, INF, sizeof(d));
		memset(curAdj, 0, sizeof(curAdj));
		d[s] = 0;
		queue<int> Q; Q.push(s);
		while (!Q.empty()) {
			int u = Q.front(); Q.pop();
			forall(i, adj[u]) {
				int v = dest[*i];
				if (cap[*i] > 0 && d[v] == INF) {
					d[v] = d[u] + 1; Q.push(v);
				}
			}
		}
		return d[t] != INF;
	}

	int MFdfs(int u, int t, int f) {
		if (u == t) return f;
		for(int &i = curAdj[u]; i < adj[u].size(); ++i) {
			int ar = adj[u][i], v = dest[ar];
			if (d[v] != d[u]+1 || cap[ar] == 0) continue;
			int tmpF = MFdfs(v, t, min(f, cap[ar]));
			if (tmpF) {
				cap[ar] -= tmpF;
				cap[inv(ar)] += tmpF;
				flow[ar] += tmpF;
				flow[inv(ar)] -= tmpF;
				return tmpF;
			}
		}
		return 0;
	}

	int maxflow(int ini, int end) {
		int maxFlow = 0;
		flow.resize(nar, 0);
		while (MFbfs(ini, end)) {
			int flow = 0;
			while ((flow=MFdfs(ini, end, INF))) maxFlow += flow;
		}
		return maxFlow;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Min Cost Flow! - O(m^2 * log n * log U)
	//
	// Don't forget to specify the imb

	int imb[VT], mark[VT], delta;
	double pot[VT], dist[VT];
	vector<double> cost;

	double rescost(int a) {
		return cost[a] - pot[orig(a)] + pot[dest[a]];
	}

	void dijkstra(int ini) {
		int i, j, k, a;
		double d;

		priority_queue<pair<double, int> > heap;
		memset(ent, -1, sizeof(ent));
		memset(mark, 0, sizeof(mark));

		fu(i, nvt) dist[i] = INFINITY;
		heap.push(make_pair(dist[ini] = 0.0, ini));

		while (!heap.empty()) {
			i = heap.top().second; heap.pop();
			if (mark[i]) continue; mark[i] = 1;
			forall(k, adj[i]) {
				a = *k; j = dest[a]; d = dist[i] + rescost(a);
				if (capres(a) >= delta && cmp(d, dist[j]) < 0) {
					heap.push(make_pair( -(dist[j] = d), j));
					ent[j] = a;
				}
			}
		}
	}


	double mincostflow() {
		int k, l, U = 0;
		double C = 0.;

		memset(pot, 0, sizeof(pot));

		fu(a, nar) {
			if (cmp(cost[a]) > 0) C += cost[a];
			U = max(cap[a], U);
		}
		fu(i, nvt) U = max(imb[i], max(-imb[i], U));
		for (delta = 0x40000000; delta > U; delta /= 2);

		imb[nvt] = 0 ; U *= 2 * nvt; C *= 2; adj[nvt].clear();
		fu(i, nvt) {
			arc(i, nvt, U, C);
			arc(nvt, i, U, C);
		}

		flow.clear();
		fu(i, nar) flow.pb(0);
		nvt++;

		while (delta >= 1) {
			fu(a, nar) {
				int i = orig(a), j = dest[a];
				if (delta <= capres(a) && capres(a) < 2 * delta &&
				    cmp(rescost(a)) < 0) {
					flow[inv(a)] -= capres(a);
					imb[i] -= capres(a); imb[j] += capres(a);
					flow[a] = cap[a];
				}
			}

			while (true) {
				for (k = 0 ; k < nvt && imb[k] < delta; k++);
				for (l = nvt - 1 ; l >= 0 && imb[l] > -delta; l--);
				if (k == nvt || l < 0) break;

				dijkstra(k);
				fu(i, nvt) pot[i] -= dist[i];
				for (int a = ent[l]; a != -1; a = ent[orig(a)])  {
					flow[a] += delta; flow[inv(a)] -= delta;
				}
				imb[k] -= delta; imb[l] += delta;
			}
			delta /= 2;
		}

		C = 0.;
		fu(a, nar) if (flow[a] > 0) C += flow[a] * cost[a];
		return C;
	}


	//////////////////////////////////////////////////////////////////////////////
	// Both Bridges/Articulation points and to Strongly Connected Components
	//

	int depth[VT];

	//////////////////////////////////////////////////////////////////////////////
	// Bridges and articulation points - O(n+m)
	//

	vector<bool> artp, bridge;
	int least[VT], nartp, nbridge;

	int dfs_artpbridge(int node, int ent) {
		int i, ar, neigh, nf = 0;

		forall(i, adj[node]) {
			ar = *i; neigh = dest[ar];

			if (depth[neigh] == -1) {
				least[neigh] = depth[neigh] = depth[node] + 1;
				dfs_artpbridge(neigh, ar); nf++;

				if (least[neigh] >= depth[node]) {
					artp[node] = true;
					if (least[neigh] == depth[neigh]) bridge[ar] = bridge[inv(ar)] = 1;
				}
				else least[node] = min(least[node], least[neigh]);
			}
			else if (inv(ar) != ent) least[node] = min(least[node], depth[neigh]);
		}
		return nf;
	}

	void partponte() {
		artp.resize(nvt, false);
		bridge.resize(nar, false);
		memset(depth, -1, sizeof(depth));
		memset(least, -1, sizeof(least));
		nartp = nbridge = 0;

		fu(i, nvt) if (depth[i] == -1) {
				least[i] = depth[i] = 0;
				if (dfs_artpbridge(i, -1) < 2) artp[i] = false;
		}
		nartp = count(all(artp), true);
		nbridge = count(all(bridge), true)/2;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Strongly Connected Components - O(n+m)
	//

	int ord[VT], comp[VT], repcomp[VT], nord, ncomp;

	int transp(int a) { return (a & 0x1); }

	void dfs_topsort(int node) {
		forall(i, adj[node]) {
			int ar = *i, neigh = dest[ar];
			if (!transp(ar) && depth[neigh] == -1) {
				depth[neigh] = depth[node] + 1; dfs_topsort(neigh);
			}
		}
		ord[--nord] = node;
	}

	void topsort() {
		memset(depth, -1, sizeof(depth));
		nord = nvt;
		fu(i, nvt) if (depth[i] == -1) {
				depth[i] = 0; dfs_topsort(i);
		}
	}

	void dfs_compfortcon(int node) {
		comp[node] = ncomp;
		forall(i, adj[node]) {
			int ar = *i, neigh = dest[ar];
			if (transp(ar) && comp[neigh] == -1) dfs_compfortcon(neigh);
		}
	}
	
	int compfortcon() {
		memset(comp, -1, sizeof(comp));
		ncomp = 0;
		topsort();

		fu(i, nvt) if (comp[ord[i]] == -1) {
			repcomp[ncomp] = ord[i];
			dfs_compfortcon(ord[i]);
			ncomp++;
		}
		return ncomp;
	}

	//////////////////////////////////////////////////////////////////////////////
	// 2-Sat - O(n+m)
	// Needs strongly connected components!
	// Graph needs to be initialized with n = 2*number of vars
	//

	int tru(int v) { return 2 * v + 1; }
	int fals(int v) { return 2 * v; }

	void clause(int x, bool valx, int y, bool valy) {
		int lhsA, rhsA, lhsB, rhsB;

		if (valx) { lhsA = fals(x); rhsB = tru(x); }
		else { lhsA = tru(x); rhsB = fals(x); }

		if (valy) { lhsB = fals(y); rhsA = tru(y); }
		else { lhsB = tru(y); rhsA = fals(y); }

		arc(lhsA, rhsA);
		arc(lhsB, rhsB);
	}

	bool twosat(int nvar) {
		compfortcon();
		fu(i, nvar) if (comp[tru(i)] == comp[fals(i)]) return false;
		return true;
	}
};
