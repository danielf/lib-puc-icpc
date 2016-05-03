// start: 27a013644077aafcb1629febb957b19f  - whole file
template<typename T> struct heavy_light {
    lca links;
    seg_tree<T> st;
    vector<int> preorder, index, jump;

// start: 24fcc911babe5ecab99a7f64c5271df4  -
    heavy_light(const vvi &graph, int root) {
        links = lca(graph, 0);

        st = seg_tree<T>(graph.size());
        index.resize(graph.size()), jump.resize(graph.size());
        dfs(root, root, root, graph);
    }
// end
// start: 502ed0eebd868117199e57a1db503363  -
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
// end
// start: f32becde141ccdb1d4a8903f00967a42  -
    void assign(int loc, T value) {
        st.upd(index[loc], value);
    }
// end

// start: ba4373e6e0dbd48fe0833bd485ca42eb  -
    T __sum(int u, int r) {
        T res;
        while (u != r) {
            int go = max(index[r] + 1, index[jump[u]]);
            res = res + st.query(go, index[u]);
            u = links.link[preorder[go]];
        }
        return res;
    }
// end

// start: 120a8a99918de53df6cc595d0ec90889  -
    T sum(int u, int v) {
        int r = links.find(u, v);
        return st.query(index[r], index[r]) + __sum(u, r) + __sum(v, r);
    }
// end
};
// end
