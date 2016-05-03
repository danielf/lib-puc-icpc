struct lca {
    tree graph;
    int L, N, root;
    vector<int> depth, size;
    vector<vector<int>> link;

    lca(){}
    lca(const tree &_graph, int _root = 0) {
        graph = _graph;
        root = _root;

        N = graph.N;
        for (L = 0; (1 << L) <= N; L++);

        depth.resize(N);
        size.resize(N);
        link.resize(N);
        init(root, root);
    }

    void init(int loc, int par) {
        link[loc].resize(L, par);
        for (int l = 1; l < L; l++)
            link[loc][l] = link[link[loc][l-1]][l-1];

        for (int nbr : graph.adj[loc]) {
            if (nbr == par) continue;
            depth[nbr] = depth[loc] + 1;
            init(nbr, loc);
            size[loc] += size[nbr];
        }

        size[loc]++;
    }

    int above(int loc, int dist) {
        for (int l = 0; l < L; l++)
            if ((dist >> l)&1)
                loc = link[loc][l];
        return loc;
    }

    int find(int u, int v) {
        if (depth[u] > depth[v]) swap(u, v);
        v = above(v, depth[v] - depth[u]);
        if (u == v) return u;

        for (int l = L - 1; l >= 0; l--) {
            if (link[u][l] != link[v][l])
                u = link[u][l], v = link[v][l];
        }

        return link[u][0];
    }
};

