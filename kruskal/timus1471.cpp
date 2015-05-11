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
#define FD(i, a, b) for(decltype(a) i = (b)-1; i >= (a); --i)
#define fd(i, n) FD(i, 0, n)

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

vi parent;
vi dist;
void dfs(vector<vector<pair<int, int>>> &graph, int node, int p) {
	for (auto x : graph[node]) if (x.first != p) {
		dist[x.first] = dist[node] + x.second;
		parent[x.first] = node;
		dfs(graph, x.first, node);
	}
}

int main() {
	int n;
	scanf("%d", &n);
	parent = vi(n);
	dist = vi(n, 0);
	vector<vector<pair<int, int>>> graph(n);
	fu(_, n-1) {
		int u, v, w;
		scanf("%d %d %d", &u, &v, &w);
		graph[u].pb({v, w});
		graph[v].pb({u, w});
	}
	dfs(graph, 0, -1);
	LCA l(parent, 0);
	int m;
	scanf("%d", &m);
	while (m--) {
		int a, b;
		scanf("%d %d", &a, &b);
		int lca = l.get(a, b);
		printf("%d\n", dist[a] + dist[b] - 2*dist[lca]);
	}
	return 0;
}

