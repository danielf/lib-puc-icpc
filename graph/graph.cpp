// start: d41d8cd98f00b204e9800998ecf8427e  -
#define pb push_back
#define sz(x) ((int)(x).size())
#define FU(i, a, b) for (int i = a; i < b; ++i)
#define fu(i, b) FU(i, 0, b)
// end

template<typename COST = double> struct graph {
  //////////////////////////////////////////////////////////////////////////////
  // Shared part. Also known as: You will need this!
  //
// start: 1b2181c68a0cee286762394ec8506b2e  -
  vi dest;  // use sz(dest) as number of arcs
  vvi adj;  // use sz(adj) as number of vertices
  int inv(int a) { return a ^ 0x1; }
// end
// start: 46461a9ddddbaafe6d44ae7ce81ec01c  - with imb
// start: 7720228554f002689c4fc45120ffa7af  - without imb
  graph(int n = 0) {
    adj.resize(n);
    imb.resize(n); // only for mincostflow
  }
// end
// end
  // Adds an arc to the graph. u is capacity, c is cost.
  // u is only needed on flows, and c only on min-cost-flow
// start: 7ec4b4137a419019bd6e83e68f880db8  - mincostflow
// start: 483f9d7dbaec7948ad9f3a53a85079a9  - max flow
// start: e204ce7fc3f9f4d270ae04f62e73f65a  - everything else
  int arc(int i, int j, ll u = 1, COST c = 0) {
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
// end
// end
// end
  //////////////////////////////////////////////////////////////////////////////
  // For both flows!!
  //

// start: e30e4506be713772fb580830560dab4a  -
  vll cap, flow;

  int orig(int a) { return dest[inv(a)]; }
  ll capres(int a) { return cap[a] - flow[a]; }
// end

  //////////////////////////////////////////////////////////////////////////////
  // Max Flow! - Dinic O(n^2 * m)
  // don't call maxflow with ini == end
  //

  vi curAdj, d;

// start: 4fd3d488cb3ec42ab3e44913cf0d898b  -
  bool MFbfs(int s, int t) {
    d.assign(sz(adj), INT_MAX/2);
    curAdj = vi(sz(adj));
    d[s] = 0;
    queue<int> Q; Q.push(s);
    while (!Q.empty()) {
      int u = Q.front(); Q.pop();
      for (auto i : adj[u]) {
        int v = dest[i];
        if (capres(i) > 0 && d[v] == INT_MAX/2) {
          d[v] = d[u] + 1; Q.push(v);
        }
      }
    }
    return d[t] != INT_MAX/2;
  }
// end

// start: adcb1e473a1ca0dac775c187e0d745c1  -
  ll MFdfs(int u, int t, ll f) {
    if (u == t) return f;
    for(int &i = curAdj[u]; i < adj[u].size(); ++i) {
      int ar = adj[u][i], v = dest[ar];
      if (d[v] != d[u]+1 || capres(ar) == 0) continue;
      ll tmpF = MFdfs(v, t, min(f, capres(ar)));
      if (tmpF) {
        flow[ar] += tmpF;
        flow[inv(ar)] -= tmpF;
        return tmpF;
      }
    }
    return 0;
  }
// end

// start: dff0c442d25c8cd09d3c0319cf61a2b3  -
  ll maxflow(int ini, int end) {
    flow.assign(sz(dest), 0);
    while (MFbfs(ini, end))
      while (MFdfs(ini, end, LLONG_MAX/2));
    ll F = 0;
		for (int a : adj[ini]) F += flow[a];
    return F;
  }
// end

	// Only call after finding maxflow
// start: 354d6065ddad2fe69327181f4c9ff769  -
	vi mincut(int ini) {
		vi ans;
		vector<bool> mark(sz(adj), false);
		mark[ini] = true; ans.pb(ini);
		fu(pos, ans.size()) {
			int x = ans[pos];
			for (int ar : adj[x]) if (capres(ar) > 0) {
				int y = dest[ar];
			 	if (mark[y]) continue;
				ans.pb(y); mark[y] = true;
			}
		}
		return ans;
	}
// end


  //////////////////////////////////////////////////////////////////////////////
  // Min Cost Flow! - O(m^2 * log n * log U)
  // Don't forget to specify the [imb]
  // look at [imb] for feasibility
  //

// start: 07e1e897ed6aff4d535d70dfd5ed2756  -
	vll imb;
  vector<COST> cost, pot;
  ll delta;

  COST rescost(int a) {
    return cost[a] + pot[orig(a)] - pot[dest[a]];
  }
// end

// start: ad4e7abf14bd6c04ef195a42627899b0  -
  bool MCFdijkstra() {
    priority_queue<pair<COST, pair<int, int> > > q;
    vi ent(sz(adj), -2);
    vector<COST> dist(sz(adj), INFINITY); // change to LLONG_MAX/2 if COST = ll
    fu(u, sz(adj)) if (imb[u] >= delta)
      q.push(make_pair(0, make_pair(u, -1)));

// 		start: 543d49eb5b190bf731112bf3b177981b  -
    while (!q.empty()) {
      int u = q.top().second.first, f = q.top().second.second;
      COST d = -q.top().first; q.pop();
      if (ent[u] != -2) continue; dist[u] = d; ent[u] = f;
			for (int a : adj[u]) if (capres(a) >= delta)
        q.push(make_pair(-(dist[u] + rescost(a)), make_pair(dest[a], a)));
    }
//	 	end

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
// end

// start: dc6e22db9a0cd772c88a4c974f483534  -
  COST mincostflow() {
    pot.resize(sz(adj));
    flow.resize(sz(dest));
		// Assumes no capacity bigger or equal than 2*0x40000000
		// i.e. assumes fits in int; 7 0s
		// Increase this limit if necessary but should always be power of 2
    for (delta = 0x40000000; delta > 0; delta /= 2) {
      fu(a, sz(dest)) {
        int u = orig(a), v = dest[a];
				// if COST = ll use rescost(a) < 0 without cmp
        if (capres(a) >= delta && cmp(rescost(a)) < 0) {
          imb[u] -= capres(a);
          imb[v] += capres(a);
          flow[inv(a)] -= capres(a);
          flow[a] += capres(a);
        }
      }
      while (MCFdijkstra());
    }
  	COST C = 0;
    fu(a, sz(dest)) if (flow[a] > 0) C += flow[a] * cost[a];
    return C;
  }
// end


  //////////////////////////////////////////////////////////////////////////////
  // Both Bridges/Articulation points and to Strongly Connected Components
  //

  vi depth;

  //////////////////////////////////////////////////////////////////////////////
  // Bridges and articulation points - O(n+m)
  //

  vector<bool> artp, bridge;
  vi least;

// start: e96781accca10408e2522db91edd6ba6  -
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
// end

// start: f2b161def41bc99ab2546415ba0a682d  -
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
// end

  //////////////////////////////////////////////////////////////////////////////
  // Strongly Connected Components - O(n+m)
  // see [rep] for results
  //

// start: 5518d3135f7ed5734417aaf00196d328  -
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
// end

// start: 66b144601ddc1a01c2b18754d62ded5a  -
  void dfs_compfortcon(int u, int ent) {
    rep[u] = ent;
		for (int a : adj[u]) {
      int v = dest[a];
      if (transp(a) && rep[v] == -1) dfs_compfortcon(v, ent);
    }
  }
// end

// start: 666fb13d2554dee3e99880194d879e18  -
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
// end
};
