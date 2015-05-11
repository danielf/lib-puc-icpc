struct graph {
  //////////////////////////////////////////////////////////////////////////////
  // Shared part. Also known as: You will need this!
  //
  vi dest;  // use sz(dest) as nar
  vvi adj;  // use sz(adj) as nvt
  int inv(int a) { return a ^ 1; }
  graph(int n = 0) {
    _ini = _end = -1; // only for flows
    adj.resize(n);
    imb.resize(n);
  }
  // Adds an arc to the graph. u is capacity, c is cost.
  // u is only needed on flows, and c only on min-cost-flow
  int arc(int i, int j, int u = 0, double c = 0) {
    dest.pb(j);
    adj[i].pb(sz(dest)-1);
    dest.pb(i);
    adj[j].pb(sz(dest)-1);
    cap.pb(u); // For both flows
    cap.pb(0);
    cost.pb(c); // Only for min cost flow
    cost.pb(-c);
    return sz(dest)-2;
  }
  //////////////////////////////////////////////////////////////////////////////
  // For both flows!!
  //

  vi cap, flow;
  int _ini, _end;   // ini, end of last maxflow

  int orig(int a) { return dest[inv(a)]; }
  int capres(int a) { return cap[a] - flow[a]; }

  //////////////////////////////////////////////////////////////////////////////
  // Max Flow! - Dinic O(n^2 * m) incremental
  // don't call maxflow with ini == end
  //

  vi d, curAdj;

  bool MFbfs(int s, int t) {
    d = vi(sz(adj), INF);
    curAdj = vi(sz(adj));
    d[s] = 0;
    queue<int> Q; Q.push(s);
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      for (auto i : adj[u]) {
        int v = dest[i];
        if (capres(i) > 0 && d[v] == INF) {
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

  int maxflow(int ini, int end) {
    if (_ini != ini || _end != end) {
      flow = vi(sz(dest));
      _ini = ini;
      _end = end;
    }
    while (MFbfs(ini, end))
      while (MFdfs(ini, end, INF));
    int F = 0;
		for (int a : adj[ini]) F += flow[a];
    return F;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Min Cost Flow! - O(m^2 * log n * log U) incremental
  // Don't forget to specify the [imb]
  // look at [imb] for feasibility
  //

  vi imb;
  vd cost, pot;
  int delta;

  double rescost(int a) {
    return cost[a] + pot[orig(a)] - pot[dest[a]];
  }

  bool dijkstra() {
    priority_queue<pair<double, pair<int, int> > > q;
    vi ent(sz(adj), -2);
    vd dist(sz(adj), INF);
    fu(u, sz(adj)) if (imb[u] >= delta)
      q.push(make_pair(0.0, make_pair(u, -1)));

    while (!q.empty()) {
      int u = q.top().second.first, f = q.top().second.second;
      double d = -q.top().first; q.pop();
      if (ent[u] != -2) continue; dist[u] = d; ent[u] = f;
			for (int a : adj[u]) if (capres(a) >= delta)
        q.push(make_pair(-(dist[u] + rescost(a)), make_pair(dest[a], a)));
    }

    fu(u, sz(adj)) if (ent[u] != -2 && imb[u] <= -delta) {
      fu(v, sz(adj)) pot[v] += dist[v];
      for (int a = ent[u]; a != -1; a = ent[orig(a)]) {
        flow[a] += delta;
        flow[inv(a)] -= delta;
        imb[dest[a]] += delta;
        imb[orig(a)] -= delta;
      }
      return true;
    }
    return false;
  }

  double mincostflow() {
    pot.resize(sz(adj));
    flow.resize(sz(dest));
    for (delta = 0x40000000; delta > 0; delta /= 2) {
      fu(a, sz(dest)) {
        int u = orig(a), v = dest[a];
        if (capres(a) >= delta && cmp(rescost(a)) < 0) {
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


  //////////////////////////////////////////////////////////////////////////////
  // Both Bridges/Articulation points and to Strongly Connected Components
  //

  vi depth;

  //////////////////////////////////////////////////////////////////////////////
  // Bridges and articulation points - O(n+m)
  //

  vector<bool> artp, bridge;
  vi least;

  int dfs_artpbridge(int u, int ent) {
    int nf = 0;
		for (int a : adj[u]) {
      int v = dest[a];
      if (depth[v] == -1) {
        least[v] = depth[v] = depth[u] + 1;
        dfs_artpbridge(v, a); nf++;

        if (least[v] >= depth[u]) {
          artp[u] = true;
          if (least[v] == depth[v]) bridge[a] = bridge[inv(a)] = true;
        } else least[u] = min(least[u], least[v]);
      }
      else if (inv(a) != ent) least[u] = min(least[u], depth[v]);
    }
    return nf;
  }

  void artpbridge() {
    artp = vector<bool>(sz(adj), false);
    bridge = vector<bool>(sz(dest), false);
    depth = vi(sz(adj), -1);
    least = vi(sz(adj), -1);
    fu(i, sz(adj)) if (depth[i] == -1) {
        least[i] = depth[i] = 0;
        if (dfs_artpbridge(i, -1) < 2) artp[i] = false;
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Strongly Connected Components - O(n+m)
  // see [rep] for results
  //

  vi ord, rep;

  int transp(int a) { return (a & 0x1); }

  void dfs_topsort(int u) {
		for (int a : adj[u]) {
      int v = dest[a];
      if (!transp(a) && depth[v] == -1) {
        depth[v] = depth[u] + 1;
        dfs_topsort(v);
      }
    }
    ord.pb(u);
  }

  void dfs_compfortcon(int u, int ent) {
    rep[u] = ent;
		for (int a : adj[u]) {
      int v = dest[a];
      if (transp(a) && rep[v] == -1) dfs_compfortcon(v, ent);
    }
  }

  void compfortcon() {
    depth = vi(sz(adj), -1);
    ord.clear();
    fu(u, sz(adj)) if (depth[u] == -1) {
        depth[u] = 0;
        dfs_topsort(u);
    }

    rep = vi(sz(adj), -1);
    for (int i = sz(adj)-1; i >= 0; i--) if (rep[ord[i]] == -1)
      dfs_compfortcon(ord[i], ord[i]);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Bipartite Matching
  // see [match] for results
  // match[i] = matching of "i", -1 if unmatched
  // left hand side = [0, k-1], and right hand side = [k, N-1]
  
  vi match;
  int dfs_match(int node, int m) {
    if (match[node] == m) return 0;
    match[node] = m;
		for (int a : adj[node]) {
      int j = dest[a];
      if (match[j] == -1 || dfs_match(match[j], m)) {
        match[j] = node;
        return 1;
      }
    }
    return 0;
  }

  int bipartite_match(int k) {
    int N = sz(adj);
    match = vi(N, -1); // during dfs, match for LHS acts as mark
    int ans = 0;
    fu(i, k) ans += dfs_match(i, i);
		fu(i, k) match[i] = -1; // fix match for LHS
    FU(i, k, N) if (match[i] != -1) match[match[i]] = i;
    return ans;
  }
};
