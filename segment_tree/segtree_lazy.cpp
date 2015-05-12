#define LC 2*node+1
#define RC 2*node+2
#define REC int node = 0, int L = 0, int R = -1
template <typename T, typename T2>
struct segtree_lazy {
	T op(const T& a, const T& b) {
		// This is THE function you want to change.
		return a + b;
	}
	void recompute(int node, int L, int R, const T2& upd) {
		// Change this also. This is an example for the pirate.
		if (!islazy[node]) lazy[node] = 0;
		islazy[node] = true;
		if (upd == 1) { // 1 means flip
			lazy[node] ^= 1;
			tree[node] = (R - L) - tree[node];
		} else if (upd != 0) { // 2 means set to 0, 3 means set to 1
			lazy[node] = upd;
			tree[node] = (upd - 2)*(R-L);
		}
	}
	int N;
	vector<T> tree;
	vector<T2> lazy;
	vector<bool> islazy;
	segtree_lazy(int _N) : N(_N) {
		int e = 32 - __builtin_clz(N-1);
		if (N == 1) e = 0;
		N = 1<<e;
		tree.resize(1<<(e+1));
		lazy.resize(1<<(e+1));
		islazy.resize(1<<(e+1), false);
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
	void push(int node, int L, int R) {
		if (!islazy[node]) return;
		islazy[node] = false;
		int m = (L+R)/2;
		recompute(LC, L, m, lazy[node]); recompute(RC, m, R, lazy[node]);
	}
	void update(int i, int j, const T2& upd) {_update(i, j+1, upd);}
	T _update(int i, int j, const T2& upd, REC) {
		if (R == -1) R = N;
		if (j <= L || i >= R) return tree[node];
		if (i <= L && R <= j) {
			recompute(node, L, R, upd);
			return tree[node];
		}
		push(node, L, R);
		int m = (L + R)/2;
		return tree[node] = 
			op(_update(i, j, upd, LC, L, m), _update(i, j, upd, RC, m, R));
	}
	// queries the CLOSED interval [i,j]
	T query(int i, int j) { return _query(i, j+1); }
	T _query(int i, int j, REC) {
		if (R == -1) R = N;
		if (i <= L && R <= j) return tree[node];
		push(node, L, R);
		int m = (L + R)/2;
		if (i >= m) return _query(i, j, RC, m, R);
		else if (j <= m) return _query(i, j, LC, L, m);
		return op(_query(i, j, LC, L, m), _query(i, j, RC, m, R));
	}
};
#undef LC
#undef RC
#undef REC
