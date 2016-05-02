template<typename T> struct heavy_light {
    lca links;
    seg_tree<T> st;
    vector<int> preorder, index, jump;

    heavy_light(const vvi &graph, int root) {
        links = lca(graph, root);

        st = seg_tree<T>(graph.size());
        index.resize(graph.size()), jump.resize(graph.size());

        dfs(root, root, root, graph);
    }

    void dfs(int loc, int par, int lhv, vvi &graph) {
        jump[loc] = lhv;
        index[loc] = preorder.size();
        preorder.push_back(loc);

        vector<int> &ch = graph[loc];
        sort(ch.begin(), ch.end(), [&](int i, int j) {
            return links.size[i] > links.size[j]; });
        if (loc != par) ch.erase(ch.begin());

        for (int c = 0; c < ch.size(); c++)
            dfs(ch[c], loc, c ? ch[c] : lhv, graph);
    }

    void upd(int loc, T value) {
        st.upd(index[loc], value);
    }

    T __query(int u, int r) {
        T res;
        while (u != r) {
            int go = max(index[r] + 1, index[jump[u]]);
            res = res + st.query(go, index[u]);
            u = links.link[preorder[go]][0];
        }
        return res;
    }

    T query(int u, int v) {
        int r = links.find(u, v);
        return st.query(index[r], index[r]) + __query(u, r) + __query(v, r);
    }
};

