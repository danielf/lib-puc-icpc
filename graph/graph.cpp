struct graph {

  //////////////////////////////////////////////////////////////////////////////
  // Shared part. Also known as: You will need this!
  //

  vi dest;  // use sz(dest) as nar
  vvi adj;  // use sz(adj) as nvt

  int inv(int a) { return a ^ 0x1; }

  // no need to init graph, vertices are added on the fly
  // if need to clear graph, just build a new one

  // Adds an arc to the graph. u is capacity, c is cost.
  // u is only needed on flows, and c only on min-cost-flow
  // delete u or c in signature if necessary
  int arc(int i, int j, int u = 0, double c = 0) {
    adj.resize(max(max(i+1, j+1), sz(adj)));
    dest.pb(j);
    adj[i].pb(sz(dest)-1);
    dest.pb(i);
    adj[j].pb(sz(dest)-1);

    // For both flows
    cap.pb(u);
    cap.pb(0);
    // Only for min cost flow
    cost.pb(c);
    cost.pb(-c);
    imb.resize(sz(adj));

    return sz(dest)-2;
  }

  //////////////////////////////////////////////////////////////////////////////
  // For both flows!!
  //

  vi cap, flow;
  int _ini, _end;   // ini, end of last maxflow or mincostflow run

  int orig(int a) { return dest[inv(a)]; }
  int capres(int a) { return cap[a] - flow[a]; }

  //////////////////////////////////////////////////////////////////////////////
  // Max Flow! - Dinic O(n^2 * m)
  // RI: flow is always feasible

  vi d, curAdj;

  bool MFbfs(int s, int t) {
    d = vi(sz(adj), INF);
    curAdj = vi(sz(adj), 0);
    d[s] = 0;
    queue<int> Q; Q.push(s);
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      forall(i, adj[u]) {
        int v = dest[*i];
        if (capres(*i) > 0 && d[v] == INF) {
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
      if (d[v] != d[u]+1 || capres(ar) == 0) continue;
      int tmpF = MFdfs(v, t, min(f, capres(ar)));
      if (tmpF) {
        flow[ar] += tmpF;
        flow[inv(ar)] -= tmpF;
        return tmpF;
      }
    }
    return 0;
  }

  // don't call maxflow with ini == end
  int maxflow(int ini, int end) {
    if (_ini != ini || _end != end) {
      flow = vi(sz(dest), 0);
      _ini = ini;
      _end = end;
    }
    while (MFbfs(ini, end))
      while (MFdfs(ini, end, INF));
    int maxFlow = 0;
    forall(a, adj[ini]) maxFlow += flow[*a];
    return maxFlow;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Min Cost Flow! - Capacity Scaling O(m^2 * log n * log U)
  //
  // Don't forget to specify the imb

  vector<double> cost;
  vi imb;
  // UPDATED UNTIL HERE
  /*
  vi imb, mark, ent;
  int delta;
  vector<double> pot, dist;

  double rescost(int a) {
    return cost[a] - pot[orig(a)] + pot[dest[a]];
  }

  void dijkstra(int ini) {
    int i, j, k, a;
    double d;

    priority_queue<pair<double, int> > heap;
    ent = vi(nvt, -1);
    mark = vi(nvt);
    dist = vector<double>(nvt, INFINITY);
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

    pot = vector<double>(nvt);

    fu(a, nar) {
      if (cmp(cost[a]) > 0) C += cost[a];
      U = max(cap[a], U);
    }
    fu(i, nvt) U = max(imb[i], max(-imb[i], U));
    for (delta = 0x40000000; delta > U; delta /= 2);

    imb.resize(nvt + 1);
    adj.resize(nvt + 1);
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

  vi depth;

  //////////////////////////////////////////////////////////////////////////////
  // Bridges and articulation points - O(n+m)
  //

  vector<bool> artp, bridge;
  vi least;
  int nartp, nbridge;

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
    artp = vi(nvt, false);
    bridge = vi(nar, false);
    depth = vi(nvt, -1);
    least = vi(nvt, -1);
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

  vi ord, comp, repcomp;
  int nord, ncomp;

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
    depth = vi(nvt, -1);
    ord = vi(nvt);
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
    comp = vi(nvt, -1);
    repcomp = vi(nvt);
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
  */
};
