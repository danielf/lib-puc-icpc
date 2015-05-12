#include "../template.cpp"
#define LC 2*node+1
#define RC 2*node+2
#define REC int node = 0, int L = 0, int R = -1

struct farao {
	ll MS, SB, SE, ST;
	int nMS, nSB, nSE, nST;
};

template <typename T>
struct segtree {
	T op(const T& a, const T& b) {
		T ans;
		ans.SB = max(a.SB, a.ST + b.SB);
		if (ans.SB > a.SB) ans.nSB = a.nST + b.nSB;
		else if (ans.SB > a.ST + b.SB) ans.nSB = a.nSB;
		else ans.nSB = max(a.nSB, a.nST + b.nSB);

		ans.SE = max(b.SE, b.ST + a.SE);
		if (ans.SE > b.SE) ans.nSE = b.nST + a.nSE;
		else if (ans.SE > b.ST + a.SE) ans.nSE = b.nSE;
		else ans.nSE = max(b.nSE, b.nST + a.nSE);
		
		ans.ST = a.ST + b.ST;
		ans.nST = a.nST + b.nST;

		ll op1 = a.MS; int nop1 = a.nMS;
		ll op2 = b.MS; int nop2 = b.nMS;
		ll op3 = a.SE + b.SB; int nop3 = a.nSE + b.nSB;
		ans.MS = max(max(op1, op2), op3);
		ans.nMS = -1;
		if (ans.MS == op1) ans.nMS = max(ans.nMS, nop1);
		if (ans.MS == op2) ans.nMS = max(ans.nMS, nop2);
		if (ans.MS == op3) ans.nMS = max(ans.nMS, nop3);
		return ans;
	}
	int N;
	vector<T> tree;
	segtree(int _N) : N(_N) {
		int e = 33 - __builtin_clz(N-1);
		tree.resize(1<<e);
	}
	// Vector to be in the leaves
	void setup(vector<T>& V, REC) {
		if (R == -1) R = N;
		if (L >= N) return;
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

int main() {
	int T;
	scanf("%d", &T);
	while (T--) {
		int N;
		scanint(N);
		vector<farao> in(N);
		for (auto &f : in) {
			scanf("%lld", &f.ST);
			f.MS = f.SB = f.SE = f.ST;
			f.nMS = f.nSB = f.nSE = f.nST = 1;
		}
		segtree<farao> T(N);
		T.setup(in);
		int Q;
		scanf("%d", &Q);
		fu(_, Q) {
			int a, b;
			scanf("%d %d", &a, &b); a--; b--;
			farao f = T.query(a, b);
			printf("%lld %d\n", f.MS, f.nMS);
		}
	}
	return 0;
}
