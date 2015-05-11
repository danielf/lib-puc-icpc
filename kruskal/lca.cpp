struct LCA {
	// assumes parent[root] = root
	vi h;
	int N;
	vi parent;
	int l;
	int calc_h(int i) {
		if (h[i] != -1) return h[i];
		return h[i] = 1 + calc_h(parent[i]);
	}
	LCA(vi& _parent, int root) {
		parent = _parent;
		N = sz(parent);
		h.resize(N, -1);
		h[root] = 0;
		fu(i, N) h[i] = calc_h(i);
		l = 33 - __builtin_clz(N-1);
		if (N == 1) l = 1;
		parent.resize(N*l);
		FU(i, 1, l) fu(k, N)
			parent[i*N + k] = parent[(i-1)*N + parent[(i-1)*N + k]];
	}
	int get(int a, int b) {
		if (h[a] > h[b]) swap(a, b);
		fd(i, l) if (h[b] - (1<<i) >= h[a]) b = parent[i*N+b];
		if (a == b) return a;
		int ans = -1;
		fd(i, l) {
			int aa = parent[i*N + a];
			int bb = parent[i*N + b];
			if (aa == bb) ans = aa;
			else {
				a = aa;
				b = bb;
			}
		}
		return ans;
	}
};
