// start: d01861d51feb1f90a505cabfb0c93efb  - whole file
struct lca {
// start: 263a97c0685736ded1e71d952fcb6bbb  -
  int L, N;
  vector<int> depth, size, link;

  lca(){}
  lca(const vvi &graph, int root = 0) {
    N = graph.size();
    for (L = 0; (1 << L) <= N; L++);
    depth.resize(N), size.resize(N);
    link.resize(L*N);
    init(root, root, graph);
  }
// end
// start: 275405b713705a9f4a0d0160a42b2883  -
  void init(int loc, int par, const vvi &graph) {
    link[loc] = par;
    for (int l = 1; l < L; l++)
      link[l*N + loc] = link[(l-1)*N + link[(l-1)*N + loc]];
//   start: e3633f221663c283556635eafb9ae464  -
    for (int nbr : graph[loc]) {
      if (nbr == par) continue;
      depth[nbr] = depth[loc] + 1;
      init(nbr, loc, graph);
      size[loc] += size[nbr];
    }
//   end
    size[loc]++;
  }
// end
// start: 30a02587001c4122b5c47637e8e65ac4  -
  int above(int loc, int dist) {
    for (int l = 0; l < L; l++)
      if ((dist >> l)&1)
        loc = link[l*N + loc];
    return loc;
  }
// end

// start: 29410901c1af44340bf193d864dfe426  -
  int find(int u, int v) {
    if (depth[u] > depth[v]) swap(u, v);
    v = above(v, depth[v] - depth[u]);
    if (u == v) return u;

    for (int l = L - 1; l >= 0; l--) {
      if (link[l*N + u] != link[l*N + v])
        u = link[l*N + u], v = link[l*N + v];
    }

    return link[u];
  }
// end
};
// end
