struct graph {

  //////////////////////////////////////////////////////////////////////////////
  // Shared part. Also known as: You will need this!
  //

  vi dest;  // use sz(dest) as nar
  vvi adj;  // use sz(adj) as nvt

  int inv(int a) { return a ^ 0x1; }

  graph(int n = 0) {
    adj.resize(n);
    imb.resize(n);
  }

  // Adds an arc to the graph. u is capacity, c is cost.
  // u is only needed on flows, and c only on min-cost-flow
  // delete u or c in signature if necessary
  int arc(int i, int j, int u = 0, double c = 0) {
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
    curAdj = vi(sz(adj));
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
      flow = vi(sz(dest));
      _ini = ini;
      _end = end;
    }
    while (MFbfs(ini, end))
      while (MFdfs(ini, end, INF));
    int F = 0;
    forall(a, adj[ini]) F += flow[*a];
    return F;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Min Cost Flow! - O(m^2 * log n * log U)
  //
  // Don't forget to specify the imb

  vi imb;
  vd cost, pot;
  int delta;

  double rescost(int a) {
    return cost[a] + pot[orig(a)] - pot[dest[a]];
  }

  bool dijkstra() {
    PRINT("imb:"); fu(i, sz(adj)) PRINT(" %d", imb[i]); PRINT("\n");
    PRINT("pot:"); fu(i, sz(adj)) PRINT(" %.2f", pot[i]); PRINT("\n");
    priority_queue<pair<double, pair<int, int> > > q;
    vi ent(sz(adj));
    vd dist(sz(adj), inf);
    fu(u, sz(adj)) if (imb[u] >= delta)
      q.push(make_pair(0.0, make_pair(u, -1)));

    while (!q.empty()) {
      int u = q.top().second.first, f = q.top().second.second;
      double d = -q.top().first; q.pop();
      if (cmp(dist[u], inf) != 0) continue; dist[u] = d; ent[u] = f;
      forall(a, adj[u]) if (capres(*a) >= delta)
        q.push(make_pair(-(dist[u] + rescost(*a)), make_pair(dest[*a], *a)));
    }
    fu(u, sz(adj)) if (cmp(dist[u], inf) != 0 && imb[u] <= -delta) {
      fu(v, sz(adj)) pot[v] += dist[v];
      PRINT("push:");
      for (int a = ent[u]; a != -1; a = ent[orig(a)]) {
        PRINT(" (%d,%d)", orig(a), dest[a]);
        flow[a] += delta;
        flow[inv(a)] -= delta;
        imb[dest[a]] += delta;
        imb[orig(a)] -= delta;
      }
      PRINT("\n");
      return true;
    }
    return false;
  }

  double mincostflow() {
    pot.resize(sz(adj));
    flow.resize(sz(dest));
    for (delta = 0x40000000; delta > 0; delta /= 2) {
      PRINT("\nPhase delta = %d\n", delta);
      fu(a, sz(dest)) {
        int u = orig(a), v = dest[a];
        if (capres(a) >= delta && cmp(rescost(a)) < 0) {
          PRINT("Saturated f(%d, %d) = %d\n", u, v, cap[a]);
          imb[u] -= capres(a);
          imb[v] += capres(a);
          flow[inv(a)] -= capres(a);
          flow[a] += capres(a);
        }
      }
      while (dijkstra());
    }
    double C = 0.0;
    fu(a, sz(dest)) if (flow[a] > 0) C += flow[a] * cost[a];
    return C;
  }


  /*
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
    artp.resize(nvt, false);
    bridge.resize(nar, false);
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
