// start: 253708b4bdc1d9dd76ff702a0c77fa55  - whole file
struct lca {
// start: aec6948521d1f69590dc2a4f32cbae4a  -
    int L, N, root;
    vector<int> depth, size;
    vector<int> link;

    lca(){}
    lca(const vvi &graph, int _root = 0) {
        root = _root;

        N = graph.size();
        for (L = 0; (1 << L) <= N; L++);

        depth.resize(N);
        size.resize(N);
        link.resize(L*N);
        init(root, root, graph);
    }
// end

// start: 1f918d95461a00bfd0422a3e4039fcf1  -
    void init(int loc, int par, vvi &graph) {
        link[loc] = par;
        for (int l = 1; l < L; l++)
            link[l*N+loc] = link[l*(N-1)+link[l*(N-1)+loc]];

        for (int nbr : graph[loc]) if (nbr != par) {
            depth[nbr] = depth[loc] + 1;
            init(nbr, loc, graph);
            size[loc] += size[nbr];
        }
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
            if (link[l*N+u] != link[l*N+v])
                u = link[l*N+u], v = link[l*N+v];
        }
        return link[u];
    }
// end
};
// end
