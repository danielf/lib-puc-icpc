template<typename T> struct heavy_light {
  lca links;
  seg_tree<T> st;
  seg_tree<T> strev;
  vector<int> preorder, index, jump;

  heavy_light(const vvi &graph, int root) : st(graph.size()), strev(graph.size()) {
    links = lca(graph, 0);

    index.resize(graph.size()), jump.resize(graph.size());
    dfs(root, root, root, graph);
  }
  void dfs(int loc, int par, int lhv, const vvi &graph) {
    jump[loc] = lhv;
    index[loc] = preorder.size();
    preorder.push_back(loc);

    vector<int> ch = graph[loc];
    sort(ch.begin(), ch.end(), [&](int i, int j) {
      return links.size[i] > links.size[j]; });
    if (loc != par) ch.erase(ch.begin());

    for (int c = 0; c < ch.size(); c++)
      dfs(ch[c], loc, c ? ch[c] : lhv, graph);
  }
  void upd(int loc, T value) {
    st.upd(index[loc], value);
    strev.upd(index.size() - index[loc] - 1, value);
  }

  T __query(int u, int r, bool upwards) {
    T res = st.identity;
    while (u != r) {
      int go = max(index[r] + 1, index[jump[u]]);
      if (upwards) res = res * strev.query(index.size() - index[u] - 1, index.size() - go - 1);
      else res = st.query(go, index[u]) * res;
      u = links.link[preorder[go]];
    }
    return res;
  }

  T query(int u, int v) {
    int r = links.find(u, v);
    return __query(u, r, true) * st.query(index[r], index[r]) * __query(v, r, false);
  }
};
