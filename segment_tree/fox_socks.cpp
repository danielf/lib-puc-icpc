#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;

#define TRACE(x...) x
#define WATCH(x) TRACE(cout << #x << " = " << x << endl)
#define PRINT(x...) TRACE(printf(x))

#define all(v) (v).begin(), (v).end()
#define rall(v) (v).rbegin(), (v).rend()

// If using C++11, change __typeof__ to decltype
#define _for(i, a, b) for (decltype(a) i = (a); i != (b); ++i)
#define foreach(x...) _for(x)
#define forall(i, v) foreach(i, all(v))
// If using C++11, change __typeof__ to decltype
#define FU(i, a, b) for(decltype(a) i = (a); i < (b); ++i)
#define fu(i, n) FU(i, 0, n)

#define mset(c, v) memset(c, v, sizeof(c))
#define mod(a, b) ((((a)%(b))+(b))%(b))
#define pb push_back
#define sz(c) int((c).size())
const int INF = 0x3F3F3F3F; const int NEGINF = 0xC0C0C0C0;
const int NULO = -1; const double EPS = 1e-8;

typedef long long ll;
typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<vi> vvi;

int cmp(double x, double y = 0, double tol = EPS) {
	return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}

struct value {
	ll sum, oddodd, oddeven;
};
const ll MOD = 1000000000;
struct Tlazy {
	int type; // 0 = sum, 1 = set
	ll C, D; // type 0
	ll X; // type 1
	Tlazy() : C(0), D(0), type(0) {}
	Tlazy(ll _C, ll _D) : C(_C), D(_D), type(0) {
		C = ((C%MOD)+MOD)%MOD;
		D = ((D%MOD)+MOD)%MOD;
	}
	Tlazy(ll _X) : X(_X), type(1) {
		X = ((X%MOD)+MOD)%MOD;
	}
};

#define LC 2*node+1
#define RC 2*node+2
#define REC int node = 0, int L = 0, int R = -1
template <typename T, typename T2>
struct segtree_lazy {
	T op(const T& a, const T& b) {
		// This is THE function you want to change.
		value ans;
		ans.sum = (a.sum + b.sum) % MOD;
		ans.oddodd = (a.oddodd + b.oddodd) % MOD;
		ans.oddeven = (a.oddeven + b.oddeven) % MOD;
		return ans;
	}
	void recompute(int node, int L, int R, const T2& upd) {
		// Change this also. This is an example for the pirate.
		if (R - L == 1) { // leaf
			islazy[node] = false;
			ll nv = upd.X;
			if (upd.type == 0) nv = (tree[node].sum + upd.C + upd.D * L) % MOD;
			tree[node].sum = nv;
			tree[node].oddodd = tree[node].oddeven = 0;
			if (nv % 2 && L % 2) tree[node].oddodd = 1;
			if (nv % 2 && L % 2 == 0) tree[node].oddeven = 1;
			return;
		}
		if (!islazy[node]) lazy[node] = Tlazy();
		islazy[node] = true;
		if (upd.type == 1) {
			lazy[node] = upd;
			tree[node].sum = ( ll(R-L) * upd.X) % MOD;
			tree[node].oddodd = tree[node].oddeven = (upd.X % 2)?(R-L)/2:0;
		} else {
			if (lazy[node].type == 2) {
				push(node, L, R);
				islazy[node] = true;
				lazy[node] = Tlazy();
			}
			tree[node].sum = (tree[node].sum + upd.C*(R-L)) % MOD;
			ll temp = (ll(R-L)*ll(L+R-1ll)/2ll) % MOD;
			tree[node].sum = (tree[node].sum + upd.D*temp) % MOD;
			// update the odd part. It depends on the parity of each thing
			if (upd.C % 2 == 0 && upd.D % 2 == 0) { // no parity changes
			} else if (upd.C % 2 == 1 && upd.D % 2 == 0) { // all parities change
				tree[node].oddodd = (R - L)/2 - tree[node].oddodd;
				tree[node].oddeven = (R-L)/2 - tree[node].oddeven;
			} else { // upd.D == 1, some will change, others won't
				// figure out if first V[L] will change parity:
				if ((L % 2) != (upd.C % 2)) // will change.
					tree[node].oddeven = (R-L)/2 - tree[node].oddeven;
				else 
					tree[node].oddodd = (R-L)/2 - tree[node].oddodd;
				lazy[node].C = (lazy[node].C + upd.C) % MOD;
				lazy[node].D = (lazy[node].D + upd.D) % MOD;
			}
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
	void update(int i, int j, const T2& upd) {_update(i, j, upd);}
	T _update(int i, int j, const T2& upd, REC) {
		if (R == -1) R = N;
		if (j <= L || i >= R) return tree[node];
		if (i <= L && R <= j) {
			recompute(node, L, R, upd);
			return tree[node];
		}
		push(node, L, R);
		int m = (L + R)/2;
		return tree[node] = op(_update(i, j, upd, LC, L, m), _update(i, j, upd, RC, m, R));
	}
	// queries the CLOSED interval [i,j]
	T query(int i, int j) { return _query(i, j); }
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

vector<ll> read(ll N, ll m, ll plus) {
	ll S1, S2, XS, YS, ZS;
	scanf("%lld %lld %lld %lld %lld", &S1, &S2, &XS, &YS, &ZS);
	vector<ll> ans(N);
	ans[0] = S1; ans[1] = S2;
	FU(i, 2, N) ans[i] = ((XS*ans[i-2] + YS*ans[i-1] + ZS) % m) + plus;
	return ans;
}


int main() {
	int _41;
	scanf("%d", &_41);
	fu(_42, _41) {
		ll N, M;
		scanf("%lld %lld", &N, &M);
		auto S = read(N, MOD, 0);
		auto O = read(M, 4, 1);
		auto A = read(M, N, 1);
		auto B = read(M, N, 1);
		auto C = read(M, MOD, 0);
		auto D = read(M, MOD, 0);
		segtree_lazy<value, Tlazy> seg(N);
		vector<value> V(N);
		fu(i, N) V[i] = {S[i], (i % 2)*(S[i] % 2), (1 - (i%2))*(S[i]%2)};
		seg.setup(V);
		ll ans = 0;
		fu(i, M) {
			if (O[i] == 1) {
				ll a = A[i]-1;
				ll len = B[i];
				ll c = C[i];
				ll d = D[i];

				ans = (ans + c*len + d*len*(len-1)/2) % MOD;
				// first part
				ll end = min(N, a+len);
				seg.update(a, end, Tlazy(c - a*d, d));
				// second part
				if (end < a+len) {
					len = a+len-end;
					seg.update(0, len, Tlazy(c + (N-a)*d, d));
				}
			} else if (O[i] == 2) {
				ll a = A[i] - 1;
				ll len = B[i];
				ll c = C[i];
				ans = (ans + len*c) % MOD;
				ll end = min(N, a+len);
				ans = (ans + seg.query(a, end).sum) % MOD;
				seg.update(a, end, Tlazy(c));
				if (end < a + len) {
					len = a + len - end;
					ans = (ans + seg.query(0, len).sum) % MOD;
					seg.update(0, len, Tlazy(c));
				}
			} else if (O[i] == 3) {
				ll a = A[i] - 1;
				ll len = B[i];
				ll end = min(N, a+len);
				ans = (ans + seg.query(a, end).sum) % MOD;
				if (end < a + len) {
					len = a + len - end;
					ans = (ans + seg.query(0, len).sum) % MOD;
				}
			} else {
				ll a = A[i] - 1;
				ll len = B[i];
				ll end = min(N, a+len);
				auto q = seg.query(a, end);
				ans = (ans + q.oddodd + q.oddeven) % MOD;
				if (end < a + len) {
					len = a + len - end;
					q = seg.query(0, len);
					ans = (ans + q.oddodd + q.oddeven) % MOD;
				}
			}
		}
		printf("Case #%d: %lld\n", _42+1, ans);
	}
	return 0;
}
