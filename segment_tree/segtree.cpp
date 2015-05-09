#define LC 2*node+1
#define RC 2*node+2
#define REC int node = 0, int L = 0, int R = -1
template <typename T>
struct segtree {
	T op(const T& a, const T& b) {
		// This is THE function you want to change.
		return max(a, b);
	}
	int N;
	vector<T> tree;
	segtree(int _N) : N(_N) {
		int e = 32 - __builtin_clz(N-1);
		if (N == 1) e = 0;
		N = 1<<e;
		tree.resize(1<<(e+1));
	}
	// Vector to be in the leaves
	void setup(vector<T>& V, REC) {
		if (R == -1) R = N;
		if (L >= sz(V)) return;
		if (R - L == 1) {
			tree[node] = V[L];
			return;
		}
		int m = (L + R)/2;
		setup(V, LC, L, m); setup(V, RC, m, R);
		tree[node] = op(tree[LC], tree[RC]);
	}
	// Does "V[i] = nv"
	T update(int i, const T& nv, REC) {
		if (R == -1) R = N;
		if (i < L || i >= R) return tree[node];
		if (R - L == 1) return tree[node] = nv;
		int m = (L + R)/2;
		return tree[node] = op(update(i, nv, LC, L, m), update(i, nv, RC, m, R));
	}
	// queries the CLOSED interval [i,j]
	T query(int i, int j) { return _query(i, j+1); }
	T _query(int i, int j, REC) {
		if (R == -1) R = N;
		if (i <= L && R <= j) return tree[node];
		int m = (L + R)/2;
		if (i >= m) return _query(i, j, RC, m, R);
		else if (j <= m) return _query(i, j, LC, L, m);
		return op(_query(i, j, LC, L, m), _query(i, j, RC, m, R));
	}
};
#undef LC
#undef RC
#undef REC
