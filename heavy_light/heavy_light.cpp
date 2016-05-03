template<typename T> struct heavy_light {
    tree graph;
    lca links;
    seg_tree<T> st;
    vector<int> preorder, index, jump;

    heavy_light(const tree &_graph, int root) {
        graph = _graph;
        links = lca(graph, root);

        st = seg_tree<T>(graph.N);
        index.resize(graph.N), jump.resize(graph.N);

        dfs(root, root, root);
    }

    void dfs(int loc, int par, int lhv) {
        jump[loc] = lhv;
        index[loc] = preorder.size();
        preorder.push_back(loc);

        vector<int> &ch = graph.adj[loc];
        sort(ch.begin(), ch.end(), [&](int i, int j) {
            return links.size[i] > links.size[j]; });
        if (loc != par) ch.erase(ch.begin());

        for (int c = 0; c < ch.size(); c++)
            dfs(ch[c], loc, c ? ch[c] : lhv);
    }

    void assign(int loc, T value) {
        st.upd(index[loc], value);
    }

    T __sum(int u, int r) {
        T res;
        while (u != r) {
            int go = max(index[r] + 1, index[jump[u]]);
            res = res + st.query(go, index[u]);
            u = links.link[preorder[go]][0];
        }
        return res;
    }

    T sum(int u, int v) {
        int r = links.find(u, v);
        return st.query(index[r], index[r]) + __sum(u, r) + __sum(v, r);
    }
};

